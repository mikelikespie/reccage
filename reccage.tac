import cjson
from twisted.web import server, resource
from twisted.internet import reactor
from twisted.application import internet, service
from lib.libreccage import StringDataSet

import psycopg2 as dbapi



encodejson = cjson.encode
decodejson = cjson.decode

#errors
NO_METHOD=1
UNKNOWN_METHOD=2
MISSING_ARGUMENT=3
UNKNOWN_EXCEPTION=4


class DataSetHandler():
    def __init__(self, conn,  nThreads = 1):
        self.dataSet = StringDataSet(nThreads)
        self.conn = conn

    def _genError(self, code, message):
        return {'status':'fail',
                'error': {'code':int(code),
                          'message':message}}

    def _genSuccess(self, payload):
        return {'status':'success',
                'payload':payload}

    #TODO change this to do it with exceptions
    def _checkArgs(self, method, req_args, args):
        '''
        Return None if valid, otherwise returns an error message
        '''
        for arg in req_args:
            if arg not in args:
                return self._genError(MISSING_ARGUMENT,\
                        'method "%s" requires argument "%s"' % (method, arg))
        return None

        

    def _add(self, args):
        r = self._checkArgs('add', ['actor', 'object', 'value'], args)
        if r is not None: return r

        actor = args['actor']
        object=args['object']
        value = float(args['value'])
        self.dataSet.addOrUpdateValue(actor, object, value)



        
        #TODO optimize this with a UDF
        cur = self.conn.cursor()
        cur.execute('''select 1 from actors where name = %s and object = %s limit 1''', (actor, object))
        if cur.rowcount == 0:
            print 'pew pewa'
            cur.execute('''insert into actors values (%s, %s, %s)''', (actor, object, value))
            print 'pew pewb'
        else:
            print 'pew pewu'
            cur.execute('''UPDATE actors SET value = %s where name = %s and object = %s''', (value, actor, object))
        cur.close()
        print 'pew pew'

        return self._genSuccess('successfully added (%s,%s) with value %s' %\
                          (args['actor'], args['object'], args['value']))

    def _del(self, args):
        r = self._checkArgs('del', ['actor', 'object'], args)
        if r is not None: return r

        actor = args['actor']
        object=args['object']

        self.dataSet.removeValue(actor, object)

        cur = self.conn.cursor()
        cur.execute('''DELETE FROM actors WHERE name = %s AND object = %s''', (actor, object))
        cur.close()
    
        return self._genSuccess('successfully removed (%s,%s)' %\
                          (args['actor'], args['object']))

    def _getTopSimilar(self, args):
        r = self._checkArgs('getTopSimilar', ['actor'], args)
        if r is not None: return r

        topK = -1
        if 'topK' in args: topK = int(args['topK'])

        ret = self.dataSet.getTopKSimilar(args['actor'], topK)
        return self._genSuccess(ret)

    def _getRecs(self, args):
        r = self._checkArgs('getRecs', ['actor'], args)
        if r is not None: return r

        topK = -1
        if 'topK' in args: topK = int(args['topK'])

        ret = self.dataSet.getRecs(args['actor'], topK)
        return self._genSuccess(ret)
        

    def handleData(self, args):
        if 'method' not in args:
            return self._genError(NO_METHOD, 'request requires method')
        method = args['method']
        #try:
        if method == 'add':
            return self._add(args)
        elif method == 'del':
            return self._del(args)
        elif method == 'getTopSimilar':
            return self._getTopSimilar(args)
        elif method == 'getRecs':
            return self._getRecs(args)
        else:
            return self._genError(UNKNOWN_METHOD, 'unknown method "%s"' % method)
#        except Exception:
#            self.conn.rollback()
#            return self._genError(UNKNOWN_EXCEPTION, 'Unknown exception')

    def catchup(self):
        n = 1
        out = open("/home/mike/reccage/out", "w")
        print "doing catchup"
        cur = self.conn.cursor()
        cur.execute("""begin""")
        cur.execute("""declare c1 cursor for select * from actors limit 100000""")
        while True:
            cur.execute("""fetch 100000 from c1""")
            if cur.rowcount == 0: break
            for row in cur:
                self.dataSet.addOrUpdateValue(row[0], row[1], row[2])
                if n % 100000 == 0:
                    print n
                n += 1
        cur.execute("""commit""")
        cur.close()
            
class WebHandler(resource.Resource):
    isLeaf = True

    def __init__(self, handler):
        self.handler = handler

    def render_GET(self, request):
        return self.renderMe(request)

    def render_POST(self, request):
        return self.renderMe(request)

    def renderMe(self, request):
        request.setHeader("Content-type", "text/javascript")
        nargs = {}
        for k,v in request.args.items(): nargs[k] = v[0] #we only want the first of each args
        return encodejson([self.handler.handleData(nargs)]) + '\n'


port = 8007#int(sys.argv[1])
port2 = 8080#int(sys.argv[1])

conn =dbapi.connect("dbname='mike' user='mike' host='localhost'")
dsh = DataSetHandler(conn, 2)
dsh.catchup()
site = server.Site(WebHandler(dsh)) #TODO change 4 to an argument

myService = service.MultiService()
#internet.TCPServer(port, factory).setServiceParent(myService)
internet.TCPServer(port2, site).setServiceParent(myService)

application = service.Application( "reccage" )


myService.setServiceParent(application)

