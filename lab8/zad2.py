import threading


def sum_sublist(sublist, result_lock, result_list):
    partial_sum = sum(sublist)

    with result_lock:
        result_list.append(partial_sum)


def parallel_sum(data, num_threads):

    if num_threads == 0:
        print("num_threads must be greater than 0")
        return 0

    list_len = len(data)
    chunk_size = list_len // num_threads

    partial_sums = []

    result_lock = threading.Lock()

    threads = []
    for i in range(num_threads):
        start = i * chunk_size
        # for last element list_len is used cuz it may be shorter than chunk_size
        end = (i + 1) * chunk_size if i < num_threads - 1 else list_len
        sublist = data[start:end]

        thread = threading.Thread(target=sum_sublist, args=(
            sublist, result_lock, partial_sums))
        thread.start()
        threads.append(thread)

    for thread in threads:
        thread.join()

    total_sum = sum(partial_sums)

    return total_sum


if __name__ == "__main__":

    large_list = list(range(1, 1000001))
    num_threads = 4

    result = parallel_sum(large_list, num_threads)
    # test
    if result == sum(large_list):
        print("dobrze")
    else:
        print("zle")
