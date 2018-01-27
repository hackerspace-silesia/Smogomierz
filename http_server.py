# based on https://github.com/cswiger/wipy/blob/master/http-server.py

import usocket as socket

CONTENT = """\
HTTP/1.0 200 OK

Hello #{} from MicroPython!

"""

ai = socket.getaddrinfo("0.0.0.0",8080)
addr = ai[0][4]

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(addr)
s.listen(5)
counter=0

while True:
    res = s.accept()
    client_s = res[0]
    client_addr = res[1]
    print("Client address:", client_addr)
    print("Client socket:", client_s)
    print("Request:")
    req = client_s.recv(4096)
    print(req)
    client_s.send(bytes(CONTENT.format(counter), "ascii"))
    client_s.close()
    parts = req.decode('ascii').split(' ')
    if parts[1] == '/exit':
      break
    counter += 1

