from socket import *
import json

class HTTP_message:
    def __init__(self):
        """Creates a object of the HTTP response"""
        self.req = []
        self.head = {}
        self.data = ''

    def create_message(self):
        """Reads the request in python.py and adds the request to req, headers to head and body to data"""
        file = open("python.py", 'r')
        with open("python.py", 'r') as file:
            tmp = file.readline()
            self.req = tmp.split(" ")
            line = file.readline()
            l = line.split(" ")
            while (l[0] != '\n'):
                liste = line.split(" ", 1)
                self.head[liste[0]] = liste[1]
                line = file.readline()
                l = line.split(" ")
            for line in file:
                self.data += line
            self.head['Content-Length'] = len(self.data)
