# QuickSort z wywołaniami rekursywnymi w osobnych wątkach
# i pomiary czasu  
import time
import random
import math
import sys
import threading 
from timeit import default_timer as timer

sys.setrecursionlimit(10 ** 9)
M=5000 # rozmiar tablicy
G=3 # ograniczenie poziomów rekursji realizowanych wielowątkowo

# zwykły QuickSort
def quickSort(A, p, r):
    if p < r:
        q = partition(A, p, r)
        quickSort(A, p, q)
        quickSort(A, q + 1, r)


def partition(A, p, r):
    k = random.randint(p, r)
    A[k], A[r] = A[r],A[k]
    i = p - 1
    x=A[r]
    for j in range(p, r + 1):
        if A[j] <= x:
            i = i + 1
            A[i], A[j] = A[j], A[i]
    if i < r:
        return i
    else:
        return i - 1

def quickSortWW(a,p,r,g):
  # quicksort z wywołaniami rekursywnymi w osobnych wątkach dla g>0 
  # a dla g=0 działa jak wersja jednowątkowa
  if g>0:
    if p<r:
      q=partition(a,p,r)
      t1 = threading.Thread(target = quickSortWW, args = (a,p,q,g-1))
      t2 = threading.Thread(target = quickSortWW, args = (a,q+1,r,g-1))
      t1.start()
      t2.start()
      t1.join()
      t2.join()
  else :
      quickSort(a,p,r)
  print ("skończył poziom ", g) 

def sprawdz(A,p,r):
  # sprawdzenie, czy tablica jest posortowana
  for i in range (p,r):
    if A[i]>A[i+1]:
       print("nie posortowane!\n")
       return
  print("posortowane\n")
  return

# wypelnienie tablic losowymi elementami
a = []
b = []

for i in range (0,M):
    x = random.randint(1, M)
    a.append(x)
    b.append(x)

# testy
start = timer()
quickSort(b,0,M-1)
stop = timer()
print(round((stop - start), 6))
sprawdz(b,0,M-1)

start = timer()
quickSortWW(a,0,M-1,G)
stop = timer()
print(round((stop - start), 6))
sprawdz(a,0,M-1)




