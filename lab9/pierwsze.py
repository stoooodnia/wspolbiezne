import math
import threading


class Barrier:
    def __init__(self, num_threads):
        self.num_threads = num_threads
        self.count = 0
        self.mutex = threading.Lock()
        self.barrier = threading.Condition(self.mutex)

    def wait(self):
        with self.mutex:
            self.count += 1
            if self.count == self.num_threads:
                self.count = 0
                self.barrier.notify_all()
            else:
                self.barrier.wait()


def isPrimary(numb):
    s = math.ceil(math.sqrt(numb))
    for i in range(2, s+1):
        if numb % i == 0:
            return False
    return True


def find_primary(l, r, barrier):
    primary_numbers = []
    for i in range(l, r+1):
        if isPrimary(i):
            primary_numbers.append(i)
    barrier.wait()
    return primary_numbers


def find_primary_in_sublist(l, r, barrier, result_list):
    partial_primary = find_primary(l, r, barrier)
    result_list.extend(partial_primary)


def parallel_primary_finding(l, r, num_threads):
    chunk_size = math.floor((r-l+1) / num_threads)
    list_len = r-l+1
    partial_primary = []

    barrier = Barrier(num_threads)

    threads = []
    for i in range(num_threads):
        start = l + i * chunk_size
        end = l + (i + 1) * chunk_size if i < num_threads - 1 else r
        thread = threading.Thread(target=find_primary_in_sublist, args=(
            start, end, barrier, partial_primary))
        thread.start()
        threads.append(thread)

    for thread in threads:
        thread.join()

    partial_primary.sort()
    return partial_primary


if __name__ == "__main__":
    l = 2
    r = 20
    num_threads = 4

    result = parallel_primary_finding(l, r, num_threads)

    # test
    print(result)
