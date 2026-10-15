#include <chrono>
#include <mutex>
#include <atomic>
#include <stdexcept>
#include <thread>
#include <iostream>
#include <vector>
#include <condition_variable>

bool ready = false;
std::condition_variable cv;

class Throtller {
public:
    Throtller(int tokenPerSecond, int maxTokens): _tokenPerSecond(tokenPerSecond), 
                                                  _maxTokens(maxTokens) {};
    ~Throtller() {}

    void Throttle() {
        while(1) {
            if (_tokens <= 0) {
                std::unique_lock lk(_mutex);
                cv.wait(lk, []{return ready;});
            } else {
                std::unique_lock lk(_mutex);
                std::cout << "thread taked tokens";
                ready = true;
                _tokens--;
            }
        }
    }

    void acceptTokens() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        _tokens++;
        std::cout << "token was accept" << std::endl;
        cv.notify_all();
    }

private:
    int _tokenPerSecond;
    int _maxTokens;
    int _tokens;
    std::mutex _mutex;
};

void func() {
    std::cout <<" thread" <<std::this_thread::get_id() << "is working" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main() {
    Throtller throtller(3, 3);

    std::thread worker([&throtller]() {
        throtller.acceptTokens();
    });

    std::vector<std::thread> threadVector;
    for (int i = 0; i <= 5; i++) {
        threadVector.emplace_back([&throtller]() {
            throtller.Throttle();
            func();
        });
    }

    worker.join();

    for (auto& thred : threadVector) {
        thred.join();
    }

    return 0;
}