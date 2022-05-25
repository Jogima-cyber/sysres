from cmd import PROMPT
import socket
import sys

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
#sent = sock.sendto(b'\n', ("127.0.0.1", 26999))
print("uid : ", end='')
name = input()
sent = sock.sendto(bytes(name+'\n', "ISO-8859-1"), ("127.0.0.1", 26999))
while True:
    print("cmd : ", end='')
    cmd = input()
    sent = sock.sendto(bytes(name+'\n'+cmd+'\n', "ISO-8859-1"), ("127.0.0.1", 26999))
    while True:
        buf, raddr = sock.recvfrom(4096)
        c = buf.decode("ISO-8859-1")
        if c == '@': 
            print("\n", end='')
            break
        print(c, file=sys.stderr, end='')
