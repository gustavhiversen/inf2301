from socket import *
import os.path
from HTTP_message import*
import json

class ServerSocket:
    def __init__(self):
        self.s = socket(AF_INET, SOCK_STREAM)#TCP socket
        self.s.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
    def set_up(self, host, port):
        """Set up the server"""
        self.s.bind((host, port))
        self.s.listen(5)
        while True:
            connectSocket , addr = self.s.accept()
            sentence = connectSocket.recv(2048)#Waits on a http message from a client
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
        request.create_req()
        http = 'HTTP/1.1 200 OK\n Content-Type: text/html\n\n'
        final = b''
        idl = request.req[1].split("/")
        if(idl[1] == 'message'):
            if (request.req[0] == 'GET'):
                final = self.get(request)
            if (request.req[0] == 'PUT'):
                final = self.put(request)
            if (request.req[0] == 'POST'):
                final = self.post(request)
            if (request.req[0] == 'DELETE'):
                self.delete(request)
        connectSocket.send(http.encode())
        connectSocket.send(final)
        connectSocket.shutdown(SHUT_WR)
        connectSocket.close()
    def get(self, request):
        """Http response with all of the messages in a single JSON array"""
        textr = open("mes", 'r')
        stri = textr.read()
        if (len(stri) == 0):
            """creates new list if there are none in the document"""
            liste = []
        else:
            liste = json.loads(stri)
        final = json.dumps(liste)
        textr.close()
        final = final.encode()
        return final
    def post(self, request):
        """Messages from request is added to mes, with unique id"""
        textr = open('mes', 'r')
        stri = textr.read()
        if (len(stri) == 0):
            """creates new list if there are none in the document"""
            liste = []
        else:
            liste = json.loads(stri)
        i = 0
        test = 1
        while(test == 1):
            test = 0
            for node in liste:
                if node["id"] == i:
                    test = 1
            if test == 1:
                i += 1
        data_to_dic = json.loads(request.data)
        dic = {}
        dic["id"] = i
        dic["data"] = data_to_dic["data"]
        liste.append(dic)
        stri = json.dumps(liste)
        final = request.data.encode()
        textr.close()
        textw = open('mes', 'w')
        textw.write(stri)
        return final
    def put(self, request):
        """Message from request is added to mes with given id and data,
        if id allready exist a then the data is replaced"""
        textr = open('mes', 'r')
        stri = textr.read()
        if (len(stri) == 0):
            """creates new list if there are none in the document"""
            liste = []
        else:
            liste = json.loads(stri)
        dic = json.loads(request.data)
        for node in liste:
            if node["id"] == dic["id"]:
                break
        if len(liste) != 0:
            if node["id"] == dic["id"]:
                node["data"] = dic["data"]
            else:
                liste.append(dic)
        else:
            liste.append(dic)
        stri = json.dumps(liste)
        textr.close()
        textw = open("mes", "w")
        textw.write(stri)
        final = request.data.encode()
        textw.close()
        return final
    def delete(self, request):
        """Message with same id as the request is removed from mes"""
        textr = open('mes', 'r')
        stri = textr.read()
        if (len(stri) == 0):
            """creates new list if there are none in the document"""
            liste = []
        else:
            liste = json.loads(stri)
        dic = json.loads(request.data)
        for node in liste:
            if node["id"] == dic["id"]:
                liste.remove(node)
        textr.close()
        textw = open('mes', 'w')
        stri = json.dumps(liste)
        textw.write(stri)
        textw.close()
server = ServerSocket()
server.set_up('localhost', 8080)
