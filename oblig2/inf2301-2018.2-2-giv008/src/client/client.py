from socket import*
from Crypto.Cipher import AES
from Crypto import Random
from Crypto.PublicKey import RSA
import sys
class Client():
    def __init__(self):
        """Initialize a TCP-socket,
        generates asymmetric private and public keys"""
        self.socket = socket(AF_INET, SOCK_STREAM)
        self.asymmetric_private_key = RSA.generate(2048)
        self.asymmetric_public_key = self.asymmetric_private_key.publickey()
        self.msg = None
        self.symmetric_key = None
        self.iv = None
    def set_up(self):
        """Sets up connection to a server, sends public key to the server,
        waits to receive key vector and encrypted message"""
        host = sys.argv[1]
        port = sys.argv[2]
        self.socket.connect((host, int(port)))
        print("sending publickey")
        self.socket.send(self.asymmetric_public_key.exportKey())
        self.msg = input('input:')
        self.file_type = self.msg.split(' ')[1]
        self.socket.send(self.msg.encode())
        self.recv = self.socket.recv(8192)
        self.recv = self.recv.split(b'\n\n', 2)#iv = recv[0], key = recv[1], msg = recv[2]
        self.decrypt_key()
        self.decrypt_msg()
        self.socket.close()
    def decrypt_key(self):
        """Decrypts the symmetric key with the private key"""
        self.symmetric_key = self.asymmetric_private_key.decrypt(self.recv[1])
    def decrypt_msg(self):
        """Decrypts message using the symmetric key"""
        cipher = AES.new(self.symmetric_key, AES.MODE_CFB, self.recv[0])
        text = cipher.decrypt(self.recv[2])
        if text == b'404':
            print(temp.decode() + "something went wrong")
        else:
            try:
                file = open(self.file_type, 'wb+')
                file.write(text)
            except:
                print("No file type given")   
client = Client()
client.set_up()
