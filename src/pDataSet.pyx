cdef inline stringOrNone(char* value):
    if value is NULL: return None
    return value

cdef extern from "DistanceFunctions.h":
    ctypedef struct ObjectValueMap
    ctypedef float (*DistanceFunction) (ObjectValueMap *p1, ObjectValueMap *p2)

    DistanceFunction pearsonDistanceOrdered



cdef extern from "ConcurrentDataSetPool.h":
    ctypedef struct c_FKPair "std::pair<float, KeyId>":
        float first
        float second

    ctypedef unsigned int size_t

    ctypedef struct FloatKeyVec:
        c_FKPair get "operator[]" (size_t n)
        size_t c_size "size" ()


    ctypedef unsigned long KeyId

    ctypedef struct c_DataSet "ConcurrentDataSetPool":
        FloatKeyVec getTopKSimilar (KeyId actor, int k, DistanceFunction df)
        void addOrUpdateValue (KeyId actor, KeyId object, float value)
        void removeValue (KeyId actor, KeyId object)
        void removeActor (KeyId actor)
    c_DataSet *new_DataSet "new ConcurrentDataSetPool" (int nThreads)
    void del_DataSet "delete" (c_DataSet *dataSet)

    ctypedef struct c_StringDataSet "ConcurrentStringDataSetPool":
        FloatKeyVec getTopKSimilar (char * actor, int k, DistanceFunction df)
        void addOrUpdateValue (char * actor, char * object, float value)
        void removeValue (char * actor, char * object)
        void removeActor (char * actor)
        char * lookupActor(KeyId id)
    c_StringDataSet *new_StringDataSet "new ConcurrentStringDataSetPool" (c_DataSet d)
    void del_StringDataSet "delete" (c_StringDataSet *stringDataSet)



cdef class DistanceFunctionContainer:
    cdef DistanceFunction func

    cdef DistanceFunction getFunc(self):
        return self.func

    def __cinit__(self, distfunc = "pearson"):
        if distfunc == "pearson":
            self.func = pearsonDistanceOrdered
        else: raise Exception("Distance function %s is invalid" % distfunc)


    def __dealloc(self):
        pass



cdef class DataSet:
    cdef c_DataSet *thisptr

    def __cinit__(self, threads = 1):
        self.thisptr = new_DataSet(threads)

    def __dealloc__(self):
        del_DataSet(self.thisptr)

    def getstuff(self):
        return 

    def getTopKSimilar (self, actor, k = -1, df = DistanceFunctionContainer()):
        if type(df) is not DistanceFunctionContainer:
            raise TypeError("a DistanceFunctionContainer is required")
        cdef FloatKeyVec foo = self.thisptr.getTopKSimilar(actor, k, (<DistanceFunctionContainer>df).getFunc())
        cdef size_t size = foo.c_size()
        return [(foo.get(n).first, int(foo.get(n).second)) for n in range(size)]

    def addOrUpdateValue (self, actor, obj, value):
        self.thisptr.addOrUpdateValue(actor, obj, value)

    def removeActor(self, actor):
        self.thisptr.removeActor(actor)

    def removeValue(self, actor, obj):
        self.thisptr.removeValue(actor, obj)

cdef class StringDataSet:
    cdef c_DataSet *dsptr
    cdef c_StringDataSet *thisptr

    def __cinit__(self, threads = 1):
        self.dsptr = new_DataSet(threads)
        self.thisptr = new_StringDataSet(self.dsptr[0])

    def __dealloc__(self):
        del_StringDataSet(self.thisptr)
        #del_DataSet(self.dsptr)

    def getstuff(self):
        return 

    def getTopKSimilar (self, actor, k = -1, df = DistanceFunctionContainer()):
        if type(df) is not DistanceFunctionContainer:
            raise TypeError("a DistanceFunctionContainer is required")
        cdef FloatKeyVec foo = self.thisptr.getTopKSimilar(actor, k, (<DistanceFunctionContainer>df).getFunc())
        cdef size_t size = foo.c_size()
        return [(foo.get(n).first, \
            self.thisptr.lookupActor(int(foo.get(n).second))) for n in range(size)]

    def addOrUpdateValue (self, actor, obj, value):
        self.thisptr.addOrUpdateValue(actor, obj, value)

    def removeActor(self, actor):
        self.thisptr.removeActor(actor)

    def removeValue(self, actor, obj):
        self.thisptr.removeValue(actor, obj)

    def lookupActor(self, actor):
        return stringOrNone(<char *>self.thisptr.lookupActor(<unsigned int>int(actor)))

