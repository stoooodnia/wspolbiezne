import math
import threading


def isPrimary(numb):
    s = math.ceil(math.sqrt(numb))
    for i in range(2, s+1):
        if numb % i == 0:
            return False
    return True


def find_primary(l, r):
    primary_numbers = []
    for i in range(l, r+1):
        if isPrimary(i):
            primary_numbers.append(i)
    return primary_numbers


def find_primary_in_sublist(l, r, barrier, result_list, result_lock):
    print("początek pracy " + threading.current_thread().name)
    partial_primary = find_primary(l, r)
    with result_lock:
        result_list.extend(partial_primary)
    barrier.wait()
    print("koniec czekania " + threading.current_thread().name)


def parallel_primary_finding(l, r, num_threads, barrier):
    chunk_size = math.floor((r-l+1) / num_threads)
    partial_primary = []
    result_lock = threading.Lock()
    print("początek pracy")
    for i in range(num_threads):
        start = l + i * chunk_size
        end = l + (i + 1) * chunk_size if i < num_threads - 1 else r
        thread = threading.Thread(target=find_primary_in_sublist, args=(
            start, end, barrier, partial_primary, result_lock))
        thread.start()
    print("koniec pracy")
    barrier.wait()
    print("koniec czekania")
    partial_primary = list(set(partial_primary))  # usuwanie duplikatów
    partial_primary.sort()
    return partial_primary


if __name__ == "__main__":
    l = 2
    r = 100
    num_threads = 5
    bar = threading.Barrier(num_threads+1)

    result = parallel_primary_finding(l, r, num_threads, bar)

    # test
    print(result)
