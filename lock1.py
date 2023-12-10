# użycie współdzielonej zmiennej chronionej semaforem (Lock)

import threading

lockSuma = threading.Lock()
suma = 0


def f1(x):
    global suma
    with lockSuma:
        suma = suma+x*x


def f2(y):
    global suma
    lockSuma.acquire()
    suma = suma+10*y
    lockSuma.release()


def oblicz(x, y):
    # wylicza x*x + 10*y, każdy składnik sumy w osobnym wątku
    t1 = threading.Thread(target=f1, args=(x,))
    t2 = threading.Thread(target=f2, args=(y,))
    t1.start()
    t2.start()
    t1.join()
    t2.join()
    return suma


# test
x = 2
y = 10
w = oblicz(x, y)

if w == x*x+10*y:
    print("dobrze")
else:
    print("zle")
