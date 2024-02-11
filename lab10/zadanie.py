import math
import time
from multiprocessing import Pool


def is_prime(candidate, primes):
    for prime in primes:
        if prime * prime > candidate:
            break
        if candidate % prime == 0:
            return False
    return True


def generate_primes(limit):
    primes = []
    for num in range(2, limit + 1, 2):
        if is_prime(num, primes):
            primes.append(num)
    return primes


def find_special_primes_range(start, end, primes):
    special_primes_set = []
    for number in range(start, end + 1):
        for prime in primes:
            if number % prime == 0:
                break
            if prime * prime > number:
                if is_prime(number + 2, primes):
                    special_primes_set.append((number, number + 2))
                break
        if is_prime(number + 2, primes):
            special_primes_set.append((number, number + 2))
    return special_primes_set


def find_special_primes_parallel(start, end, primes, num_processes):
    with Pool(num_processes) as pool:
        components = []
        part_size = (end - start) // num_processes
        for i in range(num_processes):
            components.append(
                [start + i * part_size, start + (i + 1) * part_size, primes])
        parallel_solution = pool.starmap(find_special_primes_range, components)
    return parallel_solution


if __name__ == '__main__':
    start_range = 10000000
    end_range = 20000000
    precomputed_primes = generate_primes(end_range)

    num_processes = 4

    print("Start direct approach")
    start_time_direct = time.time()
    solution_direct = find_special_primes_range(
        start_range, end_range, precomputed_primes)
    end_time_direct = time.time() - start_time_direct
    print("Direct approach time: ", end_time_direct)

    print("Start parallel processing")
    start_time_parallel = time.time()
    special_primes_parallel = find_special_primes_parallel(
        start_range, end_range, precomputed_primes, num_processes)
    end_time_parallel = time.time() - start_time_parallel
    print("Parallel processing time: ", end_time_parallel)
