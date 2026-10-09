#include <chrono>
#include <condition_variable>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <iostream>
#include <vector>

class Throttler {
public:
    Throttler(int tokensPerSecond, int maxTokens);
    ~Throttler();

    void Throttle() {
        std::lock_guard<std::mutex> lk(_mutex);
        while (actualTokens <= 0) {
            
        };
    }

private:
    int tokenPerSecond;
    int actualTokens;
    int maxTokens;
    std::mutex _mutex;
};

void someFunc() {
    std::cout << "thread is working" << std::this_thread::get_id()<< std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
}

int main() {

    std::vector<std::thread> clientThread;
    if ()
    return 0;
}