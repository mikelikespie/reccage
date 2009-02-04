import socket
import cjson
import re

# TODO make handle errors better
class NetstringClient:
    BUFFER_SIZE = 2048
    NUMBER = re.compile('(\d*)(:?)')

    def __init__(self, host='localhost', port=8007):
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.__buff = ''
        self.host = host
        self.port = port
        self.connected = False

    def connect(self):
        self.s.connect((self.host, self.port))
        self.connected = True

    def disconnect(self):
        self.s.close()
        self.connected = False

    def _checkConnected(self):
        if not self.connected:
            raise Exception('Not connected')

    def send(self, data):
        self._checkConnected()

        str = cjson.encode(data)
        self.s.send('%d:%s,' % (len(str), str))

    def recv(self):
        self._checkConnected()

        col = -1;
        while col == -1:
            col = self.__buff.find(':')
            if col == -1:
                self._needMore()

        length = int(self.__buff[:col])

        print 'length: ', length

        self.__buff = self.__buff[col+1:]


        while len(self.__buff) < length:
            self._needMore()

        str = self.__buff[:length-1]

        self.__buff = self.__buff[length:]

        if len(self.__buff) == 0:
            self._needMore()

        if self.__buff[0] == ',':
            self.__buff = self.__buff[1:]
        else:
            raise Exception('Net string not terminated by ",":\n%s' % self.__buff)

        data = cjson.decode(str)
        return data

    def _needMore(self):
        self.__buff +=  self.s.recv(self.BUFFER_SIZE)

class ReccageClient(NetstringClient):
    
    def __exchange(self, data):
        self.send(data)
        return self.recv()

    def add(self, actor, object, value):
        return self.__exchange({'method':'add', 'actor':str(actor), 'object':str(object), 'value':float(value)})

    def del(self, actor, object):
        return self.__exchange({'method':'del', 'actor':str(actor), 'object':str(object)})

    def getRecs(self, actor, topK = -1):
        return self.__exchange({'method':'add', 'actor':str(actor), 'object':str(object), 'value':float(value)})


        
