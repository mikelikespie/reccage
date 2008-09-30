cdef extern from "DistanceFunctions.h":
    ctypedef struct ObjectValueMap
    ctypedef float (*DistanceFunction) (ObjectValueMap *p1, ObjectValueMap *p2)

    DistanceFunction pearsonDistanceOrdered
    #float pearsonDistanceOrdered (ObjectValueMap *p1, ObjectValueMap *p2)


cdef extern from "DataSet.h":
    ctypedef struct c_FKPair "std::pair<float, KeyId>":
        float first
        float second

    ctypedef unsigned int size_t

    ctypedef struct FloatKeyVec:
        c_FKPair get "operator[]" (size_t n)
        size_t c_size "size" ()


    ctypedef KeyId

    
    ctypedef struct c_DataSet "DataSet":
        FloatKeyVec getTopKSimilar (KeyId actor, int k, DistanceFunction df)
        void addOrUpdateValue (KeyId actor, KeyId object, float value)

    c_DataSet *new_DataSet "new DataSet" ()
    void del_DataSet "delete" (c_DataSet *dataSet)


cdef extern from "string.h":
    void c_memcpy(void * a, void * b, size_t size)


cdef class DistanceFunctionContainer:
    cdef DistanceFunction func

    cdef DistanceFunction getFunc(self):
        return self.func

    def __cinit__(self, distfunc = "pearson"):
        if distfunc == "pearson":
            self.func = pearsonDistanceOrdered
        else: raise Exception("Distance function %s is invalid" % pearson)


    def __dealloc(self):
        pass



cdef class DataSet:
    cdef c_DataSet *thisptr

    def __cinit__(self):
        self.thisptr = new_DataSet()

    def __dealloc__(self):
        del_DataSet(self.thisptr)

    def getstuff(self):
        return 

    def getTopKSimilar (self, actor, k, df):
        if type(df) is not DistanceFunctionContainer:
            raise TypeError("a DistanceFunctionContainer is required")
        cdef FloatKeyVec foo = self.thisptr.getTopKSimilar(actor, k, (<DistanceFunctionContainer>df).getFunc())
        cdef size_t size = foo.c_size()
        return [(foo.get(n).first, foo.get(n).second) for n in range(size)]

    def addOrUpdateValue (self, actor, obj, value):
        self.thisptr.addOrUpdateValue(actor, obj, value)


    


