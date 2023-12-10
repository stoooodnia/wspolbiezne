import threading
import time


lockSuma = threading.Lock()
suma = 0


def f(array, s):
    global suma
    with lockSuma:
        for i in array:
            suma = suma + i
            time.sleep(s)


def sumuj(array):
    global suma
    x = array[:len(array)//2]
    y = array[len(array)//2:]
    t1 = threading.Thread(target=f, args=(x, 1))
    t2 = threading.Thread(target=f, args=(y, 0.7))
    t1.start()
    t2.start()
    t1.join()
    t2.join()
    return suma


# test
def testuj(array):
    global suma
    suma = 0
    w = sumuj(array)
    print(w)
    if w == sum(array):
        print("dobrze")
    else:
        print("zle")


array1 = [1, 2, 3, 4]
array2 = [1, 2, 3, 4, 5]

testuj(array1)
testuj(array2)
