# utworzenie dwóch wątków współbieżnych wykonującyh
# funkcję f 
# synchronizacja barierą
import threading
import time

def f(arg,name,s,bar):
    for i in range(arg):
        print(name,'i=',i)
        time.sleep(s)
    print ("koniec pracy "+name)
    bar.wait()
    print ("koniec czekania "+name)

b = threading.Barrier(3)
t1 = threading.Thread(target = f, args = (5,'t1',1,b))
t2 = threading.Thread(target = f, args = (2,'t2',0.7,b))
t1.start()
t2.start()
print ("koniec pracy ")
b.wait()
# t1.join()
# t2.join()
print ("koniec czekania")

