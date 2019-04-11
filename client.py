import sys
import socket
import struct

def send_uint32(sock: socket.socket, n: int) -> int:
    data = struct.pack('!I', n)
    return sock.send(data)

sock = socket.socket()

port = int(sys.argv[1])

sock.connect(('127.0.0.1', port))

with open('client.log', 'w') as fp:
    for line in sys.stdin:
        line = line.strip()
        msg_len = len(line)
        send_uint32(sock, msg_len)
        fp.write(f'{msg_len} {line}\n')
        fp.flush()
        sock.send(line.encode('utf-8'))
