#!/usr/bin/python3
import socket
import argparse

IP = "192.168.0.xx"
PORT = 1701
parser = argparse.ArgumentParser()
parser.add_argument("--r", help="display a square of a given number", type=int, default=0)
parser.add_argument("--g", help="display a square of a given number", type=int, default=0)
parser.add_argument("--b", help="display a square of a given number", type=int, default=0)
parser.add_argument("--cmd", help="display a square of a given number", type=str, default=None)

args = parser.parse_args()

Message = "r:{}:g:{}:b:{}:cmd:{}".format(args.r,args.g,args.b,args.cmd).encode('utf-8')

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(Message, (IP, PORT))
UDP_Port = sock.getsockname()[1]
print("Sending from local port {}".format(UDP_Port))

while True:
    data, addr = sock.recvfrom(UDP_Port) # buffer size is 1024 bytes\
    if len(data) > 0:
        print("received message: {}".format(data.decode('utf-8')))
        break