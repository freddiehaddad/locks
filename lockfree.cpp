#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

// Beginning implementation of a lock free queue.
// The initial solution contains two methods:
//     - enqueue
//     - tail
class LockFree {
  public:
    // Construct a LockFree object with specified capacity.
    LockFree(size_t capacity) : _capacity(capacity), _tail(0) {
    }

    // Simulate adding a value by incrementing a pointer.
    bool enqueue(std::size_t item) {
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
    std::atomic_size_t _tail;
};

// Lock free data structure for recording thread job results.
class ResultsFree {
  public:
    // Construct a results to hold results from num_threads.
    ResultsFree(std::size_t num_results) {
        _results.resize(num_results);
    }

    // Record results from a thread.
    void record(const std::string &result) {
        std::size_t index = _index++;
        _results[index]   = result;
    }

    // Print the results to standard output.
    void print() const {
        for (auto &result : _results) {
            std::cout << result << std::endl;
        }
    }

  private:
    // Pointer to next index for record.
    std::atomic_size_t _index;
    // Container holding the results.
    std::vector<std::string> _results;
};

// Thread worker function.
void do_work(LockFree &buffer, ResultsFree &results) {
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

    ResultsFree results(num_threads);
    LockFree    buffer(buffer_size);

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
