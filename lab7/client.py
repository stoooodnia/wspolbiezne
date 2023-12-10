import socket
import struct

while True:
    client_num = input("Podaj swój numer: ")
    if not client_num.isdigit():
        print("Podaj poprawny numer")
        continue

    print(client_num)
    client_id = int(client_num) + 5001

    serwerAdresPort = ("127.0.0.1", 5001)
    klientAdresPort = ("127.0.0.1", client_id)
    bufSize = 1024

    UDPClientSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        UDPClientSocket.bind(klientAdresPort)
        break
    except OSError:
        print("Port zajety")


while True:
    client_input = input("Kamien: 1, Papier: 2, Nozyce: 3, Koniec: 0 \n")
    try:
        if int(client_input) > 3 or int(client_input) < 0:
            print("Podaj poprawna liczbe")
            continue
        komANP = struct.pack('!i', int(client_input))
        UDPClientSocket.sendto(komANP, serwerAdresPort)
        if int(client_input) == 0:
            print("Wyjscie")
            break
        odpNP = UDPClientSocket.recvfrom(bufSize)
        odp = struct.unpack('!i', odpNP[0])
        if odp[0] == -1:
            print("Przeciwnik wyszedl")
        else:
            print("Punkty: " + str(odp[0]))
    except ValueError:
        print("Zły input!!!")
