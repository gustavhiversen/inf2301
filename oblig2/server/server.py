from socket import*
import config as cg
from Crypto.Cipher import AES
from Crypto import Random
from Crypto.PublicKey import RSA
import base64, sys

class Server():
    def __init__(self):
        """Initialize a TCP-server,
        generates a symmetric key"""
        self.socket = socket(AF_INET, SOCK_STREAM)
        self.socket.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
        self.iv = Random.new().read(AES.block_size)
        self.asymmetric_public_key = None
        self.encrypted_key = None
        self.symmetric_key = Random.new().read(16)
    def run(self, port, host):
        """Sets up a socket and waits for a client to connect."""
        self.socket.bind((host, port))
        self.socket.listen(1)
        while 1:
            connectionSocket, addr = self.socket.accept()
            self.asymmetric_public_key = connectionSocket.recv(4096)
            self.asymmetric_public_key = RSA.importKey(self.asymmetric_public_key.decode())
            self.encrypt_key()
            print("waiting on client")
            self.msg = connectionSocket.recv(4096)
            self.parser(connectionSocket)
            connectionSocket.send(self.iv + b'\n\n' + self.encrypted_key + b'\n\n' + self.final)
            connectionSocket.close()
    def encrypt_key(self):
        """Encrypts the symmetric key using the public key"""
        self.encrypted_key = self.asymmetric_public_key.encrypt(self.symmetric_key,32)
        self.encrypted_key = self.encrypted_key[0]
    def parser(self, connectionSocket):
        """Parses the request, and encrypts requested file"""
        msg = self.msg.decode()
        spl_msg = msg.split(" ")
        if (spl_msg[0] == 'GET'):
            try:
                file = open(spl_msg[1], 'rb')
                f = file.read()
                cipher = AES.new(self.symmetric_key, AES.MODE_CFB, self.iv)
                self.final = cipher.encrypt(f)
            except:
                cipher = AES.new(self.symmetric_key, AES.MODE_CFB, self.iv)
                f = b'404'
                self.final = cipher.encrypt(f)
        else:
            cipher = AES.new(self.symmetric_key, AES.MODE_CFB, self.iv)
            f = b'404'
            self.final = cipher.encrypt(f)
server = Server()
server.run(cg.PORT, cg.HOST)
