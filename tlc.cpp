#include <map>
#include <thread>
#include <vector>
#include <iostream>
#include <mutex>
#include <string>

class TLS {
private:
    std::map<int, int> _tls;
public:

    void Put(int id, int value) {
        _tls[id] = value;
    }

    int Get(int id) {
        return _tls[id];
    }
};

TLS tls_global;
std::mutex mtx;

thread_local int tl_id{};

void Print(const char* str){
    std::lock_guard<std::mutex> lock(mtx);    
    std::cout << str <<" tl_id:" << tl_id << std::endl;
}

void Foo(int i) {
    std::string str("Value: ");
    tl_id = i;
    str+= std::to_string(tls_global.Get(i));
    Print(str.c_str());

}

int main() {
    std::vector<std::thread*> threads;
    tl_id = 1000;

    for (int i = 0; i < 500; i++) tls_global.Put(i, 1000 + i);

    for (int i = 0; i < 500; i++) 
        threads.push_back(new std::thread(Foo, i));
    for (auto tt : threads) {
        tt->join();
        delete tt;
    }
    return 0;
}

