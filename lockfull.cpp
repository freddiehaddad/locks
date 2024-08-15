#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>

// Beginning implementation of a lock full queue.
// The initial solution contains two methods:
//     - enqueue
//     - tail
class LockFull {
  public:
    // Construct a LockFull object with specified capacity.
    LockFull(std::size_t capacity) : _capacity(capacity), _tail(0) {
    }

    // Simulate adding a value by incrementing a pointer.
    bool enqueue(std::size_t item) {
        std::lock_guard<std::mutex> lock(_mutex);
        return ++_tail <= _capacity;
    }

    // Returns the position of the tail.
    std::size_t tail() const {
        return _tail;
    }

  private:
    // Capacity of the queue.
    const std::size_t _capacity;
    // Pointer to the tail element.
    std::size_t _tail;
    // Lock for synchronization.
    std::mutex _mutex;
};

// Lock full data structure for recording thread job results.
class ResultsFull {
  public:
    // Construct a results to hold results from num_threads.
    ResultsFull(std::size_t num_results) : _index(0) {
        _results.resize(num_results);
    }

    // Record results from a thread.
    void record(const std::string &result) {
        std::lock_guard<std::mutex> lock(_mutex);
        _results[_index++] = result;
    }

    // Print the results to standard output.
    void print() const {
        std::lock_guard<std::mutex> lock(_mutex);
        for (auto &result : _results) {
            std::cout << result << std::endl;
        }
    }

  private:
    // Pointer to next index for record.
    std::size_t _index;
    // Container holding the results.
    std::vector<std::string> _results;
    // Lock for synchronization.
    mutable std::mutex _mutex;
};

// Thread worker function.
void do_work(LockFull &buffer, ResultsFull &results) {
    // capture start time
    auto start = std::chrono::high_resolution_clock::now();

    // do some work
    std::size_t count = 0;
    while (buffer.enqueue(count)) {
        count++;
    }

    // capture end time
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    // generate a result
    std::stringstream ss;
    ss << "Thread " << std::setw(5) << std::right << std::this_thread::get_id()
       << " processed " << count << " ints in " << std::fixed
       << std::setprecision(10) << std::right << duration.count()
       << " seconds.";
    results.record(ss.str());
}

// program entry point
int main() {
    const std::size_t num_threads = 6;
    const std::size_t buffer_size = 200000000;

    ResultsFull results(num_threads);
    LockFull    buffer(buffer_size);

    // capture start time
    auto start = std::chrono::high_resolution_clock::now();
    {
        // spawn thrads to do work and join when finished
        std::vector<std::jthread> pool;
        for (std::size_t i = 0; i < num_threads; i++) {
            pool.emplace_back(
                std::jthread(do_work, std::ref(buffer), std::ref(results)));
        }
    }
    // capture end time
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    results.print();

    std::cout << buffer_size << " operations performed across " << num_threads
              << " threads in " << std::fixed << std::setprecision(10)
              << duration.count() << " seconds." << std::endl;

    return 0;
}
