import re
import matplotlib.pyplot as plt

def process_log(log_file):
    data_tuples = []

    with open(log_file, 'r') as file:
        log_content = file.read()

    pattern = r"Filename: (\d+)_random_MiB\.bin.*?Elapsed Time (\d+) ms.*?Elapsed Time (\d+) ms"
    matches = re.findall(pattern, log_content, re.DOTALL)

    for match in matches:
        filesize, encode_time, decode_time = map(int, match)
        data_tuples.append((filesize, encode_time, decode_time))

    return data_tuples

def calculate_averages(data_tuples):
    avg_tuples = []

    size_to_times = {}
    for data_tuple in data_tuples:
        filesize, encode_time, decode_time = data_tuple

        if filesize not in size_to_times:
            size_to_times[filesize] = {"encode_times": [], "decode_times": []}

        size_to_times[filesize]["encode_times"].append(encode_time)
        size_to_times[filesize]["decode_times"].append(decode_time)

    for filesize, times in size_to_times.items():
        encode_times = times["encode_times"]
        decode_times = times["decode_times"]

        avg_encode_time = sum(encode_times) / len(encode_times) if encode_times else None
        avg_decode_time = sum(decode_times) / len(decode_times) if decode_times else None

        avg_tuples.append((filesize, avg_encode_time, avg_decode_time))

    return avg_tuples

def plot_data(avg_tuples):
    file_sizes, avg_encode_times, avg_decode_times = zip(*avg_tuples)

    plt.plot(file_sizes, avg_encode_times, label='Average Encode Time')
    plt.plot(file_sizes, avg_decode_times, label='Average Decode Time')

    plt.xlabel('Filesize (MiB)')
    plt.ylabel('Time (ms)')
    plt.title('Average Encode and Decode Times')
    plt.legend()
    plt.grid(True)
    plt.show()

log_file = "log.txt"
data_tuples = process_log(log_file)
avg_tuples = calculate_averages(data_tuples)
plot_data(avg_tuples)

