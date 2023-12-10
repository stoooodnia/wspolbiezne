# utworzenie dwóch wątków współbieżnych wykonującyh
# funkcję f
import threading
import time

def f(arg,name,s):
    for i in range(arg):
        print(name,'i=',i)
        time.sleep(s)

t1 = threading.Thread(target = f, args = (5,'t1',1))
t2 = threading.Thread(target = f, args = (5,'t2',0.7))
t1.start()
t2.start()
t1.join()
t2.join()
print ("koniec")

