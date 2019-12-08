#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>


using namespace std::chrono_literals;

struct pcout : public std::stringstream {
    static std::mutex cout_mutex;

    ~pcout() {
        std::lock_guard<std::mutex> l {cout_mutex};
        std::cout << rdbuf();
    }
};

std::mutex pcout::cout_mutex;

std::mutex q_mutex;
bool production_stopped {false};
std::queue<size_t> q;

std::condition_variable go_produce;
std::condition_variable go_consume;

static void producer(size_t id, size_t items, size_t stock)
{
    for(size_t i = 0; i < items; ++i) {
        std::unique_lock<std::mutex> lock(q_mutex);
        go_produce.wait(lock, [&] { return q.size() < stock; });

        q.push(id * 100 + i);

        pcout{} << "   Producer " << id << " --> item "
                << std::setw(3) << q.back() << '\n';

        go_consume.notify_all();
        std::this_thread::sleep_for(90ms);
    }

    pcout{} << "EXIT: Producer " << id << '\n';
}

static void consumer(size_t id)
{
    while (!production_stopped || !q.empty()) {
        std::unique_lock<std::mutex> lock(q_mutex);

        if (go_consume.wait_for(lock, 1s, [] { return !q.empty(); })) {
            pcout{} << "                  item "
                    << std::setw(3) << q.front() << " --> Consumer "
                    << id << '\n';
            q.pop();
            go_produce.notify_all();
            std::this_thread::sleep_for(130ms);
        }
    }

    pcout{} << "EXIT: Consumer " << id << '\n';
}

int main()
{
        std::vector<std::thread> workers;
        std::vector<std::thread> consumers;

        for(size_t i = 0; i < 5; ++i) {
            workers.emplace_back(producer, i, 15, 5);
        }

        //std::cin.get();

        for(size_t i = 0; i < 5; ++i) {
            consumers.emplace_back(consumer, i);
        }

        for(auto &t : workers) { t.join(); }
        production_stopped = true;
        for(auto &t : consumers) { t.join(); }
}