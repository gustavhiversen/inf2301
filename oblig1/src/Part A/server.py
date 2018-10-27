from socket import *
import os.path
from HTTP_message import *
import json

class ServerSocket:
    def __init__(self):
        self.socket = socket(AF_INET, SOCK_STREAM)#TCP socket
        self.socket.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)

    def set_up(self, host, port):
        """Sets up the server"""
        self.socket.bind((host, port))
        self.socket.listen(5)
        while True:
            connectSocket , addr = self.socket.accept()
            sentence = connectSocket.recv(2048)
            sentence = sentence.decode()
            self.file(sentence)
            self.parser(connectSocket)

    def file(self, sentence):
        """Creates file for the request"""
        req = open("python.py" , 'w+')
        req.write(sentence)
        req.close()


    def parser(self, connectSocket):
        """Parses the request"""
        request = HTTP_message()
        request.create_message()
        content_length = b''
        http = 0
        final = b''
        typ = request.req[1].split(".")
        if (request.req[0] == 'GET'):
            if(request.req[1] == '/' or request.req[1] == '/index.html'): #special case to open the index.html file
                http = 'HTTP/1.1 200 OK\n Content-Type: text/html\n'
                index = open("./index.html", 'rb')
                content_length = 'Content-Length:' + str(os.path.getsize("./index.html")) + '\n'#Should have 2 newlines, but onlye works with 1
                content_length = content_length.encode()
                final = index.read()
            else:
                try: #Checks if the file exist
                    e = open("." + request.req[1], 'rb')
                    content_length = 'Content-Length:' + str(os.path.getsize("." + request.req[1]))+ '\n\n'
                    content_length = content_length.encode()
                    final = e.read()
                except:
                    error = request.req[1] + ' 404 Not Found\n'
                    final = error.encode()
        elif (request.req[0] == 'POST'):
            test = open('.'+request.req[1], 'w')
            stri = request.data.split('=')
            textr = open('testpage.html', 'rb')
            final = textr.read()
            test.write(stri[1])
            test.close()
        if(http == 0):
            http = 'HTTP/1.1 200 OK\n Content-Type: text/html\n'
        connectSocket.send(http.encode())
        connectSocket.send(content_length)
        connectSocket.send(final)
        connectSocket.shutdown(SHUT_WR)
        connectSocket.close()

server = ServerSocket()
server.set_up('localhost', 8080)
