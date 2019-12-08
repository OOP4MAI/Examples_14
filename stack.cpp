#include <iostream>
#include <thread>
#include <exception>
#include <stack>
#include <mutex>
#include <memory>
#include <vector>
#include <sstream>



class empty_stack : std::exception{
};

struct pcout : public std::stringstream {
    static std::mutex cout_mutex;

    ~pcout() {
        std::lock_guard<std::mutex> l {cout_mutex};
        std::cout << rdbuf();
    }
};
std::mutex pcout::cout_mutex;

template <typename T>
class thread_safe_stack
{
private:
    std::stack<T> data;
    mutable std::mutex m;

public:
    thread_safe_stack(void){};

    thread_safe_stack(const thread_safe_stack &other){
        std::lock_guard<std::mutex> lock(other.m);
        data = other.data;
    };

    void push(T new_value){
        std::lock_guard<std::mutex> lock(m); // try to comment
        data.push(new_value);
    }

    std::shared_ptr<T> pop(){
        std::lock_guard<std::mutex> lock(m);
        if (data.empty())
            throw empty_stack();
        std::shared_ptr<T> const res(new T(data.top()));
        data.pop();
        return res;
    }

    void pop(T &value){
        std::lock_guard<std::mutex> lock(m); // try to comment
        if (data.empty())
            throw empty_stack();
        value = data.top();
        data.pop();
    }

    bool empty() const{
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }
};

void foo(thread_safe_stack<std::string> *stack, int number)
{
    for (int i = 0; i < 1000; i++)
        stack->push(std::string("some string"));
    try{
        for (int i = 0; i < 1000; i++){
            std::string val;
            stack->pop(val);
        }
    }
    catch (...){
        pcout{} << "Oppps!" << std::endl;
    }

    pcout{} << "Thread " << number << (stack->empty() ? " is empty" : " is not empty") << "\n";
}

int main(int argc, char *argv[]){
    thread_safe_stack<std::string> stack;
    std::vector<std::thread> threads;

    for (int i = 0; i < 100; i++)
        threads.push_back(std::thread(foo, &stack, i));

    for (auto &tt : threads)
        tt.join();
    
    pcout{} << "Done \n";

    return 0;
}