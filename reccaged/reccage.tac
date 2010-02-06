
#TODO convert output to use logger
import json

from zope.interface import implements
from twisted.web import server, resource
from twisted.internet import reactor
from twisted.application import internet, service
from twisted.internet.protocol import Factory
from twisted.protocols.basic import NetstringReceiver
from reccage import StringDataSet
import sys
from twisted.python import usage


def encodejson(*args, **kwargs):
    return json.dumps(*args, **kwargs) + '\n'
decodejson = json.loads

#errors
NO_METHOD=1
UNKNOWN_METHOD=2
MISSING_ARGUMENT=3
UNKNOWN_EXCEPTION=4
INVALID_JSON=5
INVALID_DATA=6

class Error(Exception):
    def __init__(*args):
        Exception.__init__(*args)
        if not self.args:
            self.args = (self.errcode, self.value)

    @property
    def return_val(self):
        code, message = self.args
        return {'status':'fail',
                'error': {'code':int(code),
                          'message':message}}

class NoMethodError(Error):
    value = 'request requires method'
    errcode = NO_METHOD

class UnknownMethodError(Error):
    errcode = UNKNOWN_METHOD

    def __init__(self, method_name):
        self.args = (self.errcode, 'unknown method %s' % method_name)

class MissingArgumentsError(Error):
    errcode = MISSING_ARGUMENT

    def __init__(self, method, arg_name):
        self.args = (self.errcode, 'method "%s" requires argument "%s"' % (method, arg_name))

class UnknownExceptionError(Error):
    errcode = UNKNOWN_EXCEPTION
    value = 'Unknown exception'

class InvalidJSONError(Error):
    errcode = INVALID_JSON
    value = 'Invalid JSON'

class InvalidDataError(Error):
    errcode = INVALID_DATA
    value = 'Invalid Data'


class DataSetHandler():
    def __init__(self,  nThreads = 1):
        self.dataSet = StringDataSet(nThreads)

    def genError(self, code, message):
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
                raise MissingArgumentsError(method, arg)

    def _add(self, args):
        self._checkArgs('add', ['actor', 'object', 'value'], args)

        actor = args['actor']
        object=args['object']
        value = float(args['value'])
        self.dataSet.addOrUpdateValue(actor, object, value)

        return self._genSuccess('successfully added (%s,%s) with value %s' %\
                          (args['actor'], args['object'], args['value']))

    def _del(self, args):
        self._checkArgs('del', ['actor', 'object'], args)

        actor = args['actor']
        object=args['object']

        self.dataSet.removeValue(actor, object)
    
        return self._genSuccess('successfully removed (%s,%s)' %\
                          (args['actor'], args['object']))

    def _getTopSimilar(self, args):
        self._checkArgs('getTopSimilar', ['actor'], args)

        topK = -1
        if 'topK' in args: topK = int(args['topK'])

        ret = self.dataSet.getTopKSimilar(args['actor'], topK)
        return self._genSuccess(ret)

    def _getRecs(self, args):
        self._checkArgs('getRecs', ['actor'], args)

        topK = -1
        if 'topK' in args: topK = int(args['topK'])

        ret = self.dataSet.getRecs(args['actor'], topK)
        return self._genSuccess(ret)

    def handleData(self, args):
        if 'method' not in args:
            raise NoMethodError()

        method = args['method']

        if method == 'add':
            return self._add(args)
        elif method == 'del':
            return self._del(args)
        elif method == 'getTopSimilar':
            return self._getTopSimilar(args)
        elif method == 'getRecs':
            return self._getRecs(args)
        else:
            raise UnknownMethodError(method)




class PersistentDataSetHandler(DataSetHandler):
    """
    This wraps persistent dataset handler with some extras to store itself
    """

    def __init__(self, conn,  nThreads = 1):
        PersistentDataSetHandler.__init__(self, nThreads)
        self.conn = conn


    def _del(self, args):
        ret = DataSetHandler._del(self, args)

        cur = self.conn.cursor()
        cur.execute('''DELETE FROM actors WHERE name = %s AND object = %s''', (actor, object))
        cur.close()

        return ret

    def _add(self, args):
        ret = DataSetHandler._add(self, args)

        #TODO optimize this with a UDF... also isn't transactionally safe
        cur = self.conn.cursor()
        cur.execute('''select 1 from actors where name = %s and object = %s limit 1''', (actor, object))
        if cur.rowcount == 0:
            cur.execute('''insert into actors values (%s, %s, %s)''', (actor, object, value))
        else:
            cur.execute('''UPDATE actors SET value = %s where name = %s and object = %s''', (value, actor, object))
        cur.close()

        return ret

    def handleData(self, args):
        try:
            DataSetHandler.handleData(self, args)
        except Exception, e:
            self.conn.rollback()
            raise


    def catchup(self):
        n = 1
        cur.execute("create table actors (actor varchar(512), object varchar(512), value double)")
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

    # TODO FIXME
    # the handling of data should be done in a deferred.  we shouldn't let this block
    def renderMe(self, request):
        try:
            request.setHeader("Content-type", "text/javascript")
            nargs = {}
            for k,v in request.args.items(): nargs[k] = v[0] #we only want the first of each args
            return encodejson([self.handler.handleData(nargs)])
        #these our the errors we defined
        except Error, e:
            return encodejson([e.return_val])
        #we don't know what these are
        except Exception, e:
            sys.stderr.write('%s' % e)
            #TODO change errors to exception handling
            return encodejson([UnknownExceptionError().return_val])


class ReccageProtocol(NetstringReceiver):

    def __init__(self, handler):
        self.handler = handler
        print "Made connection"

    # TODO FIXME
    # the handling of data should be done in a deferred.  we shouldn't let this block
    def stringReceived(self, data):
        print 'got string'
        try:
            try:
                nargs = decodejson(data)
                if type(nargs) != dict:
                    raise InvalidDataError()
                else:
                    data = self.handler.handleData(nargs)

                self.sendString(encodejson(e.return_val))
            except cjson.DecodeError, e:
                raise InvalidJSONError()
        except Error, e:
            self.sendString(encodejson(e.return_val))

    def connectionLost(self, reason):
        print "Closing connection:\n%s" % reason

class ReccageFactory(Factory):
    protocol = ReccageProtocol
    def __init__(self, handler):
        self.handler = handler

    def buildProtocol(self, addr):
        p = self.protocol(self.handler)
        p.factory = self
        return p

print sys.argv


port = 8007#int(sys.argv[1])
port2 = 8080#int(sys.argv[1])

dsh = None

num_threads = 2

if False:
    import psycopg2 as dbapi
    conn = dbapi.connect("dbname='mike' user='mike' host='localhost'")
    dsh = PersistentDataSetHandler(conn, num_threads)
    dsh.catchup()
else:
    dsh = DataSetHandler(num_threads)

site = server.Site(WebHandler(dsh)) #TODO change 4 to an argument

factory = ReccageFactory(dsh)

myService = service.MultiService()
internet.TCPServer(port, factory).setServiceParent(myService)
internet.TCPServer(port2, site).setServiceParent(myService)

application = service.Application( "reccage" )


myService.setServiceParent(application)

