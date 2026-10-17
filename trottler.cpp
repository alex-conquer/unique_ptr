#include <chrono>
#include <mutex>
#include <atomic>
#include <stdexcept>
#include <thread>
#include <iostream>
#include <vector>
#include <condition_variable>

class Throtller {
public:
    Throtller(int tokenPerSecond, int maxTokens): _tokens(tokenPerSecond), 
                                                  _maxTokens(maxTokens) {};
    
    ~Throtller() {}

    bool Throttle() {
        
        std::unique_lock lk(_mutex);
        if (_tokens <= 0) {
            std::cout << "waiting" << std::endl;
            cv.wait(lk, [this]{ return _tokens > 0 || !_running; });
            if (!_running) {
                return false;
            }
        }
        std::cout << "thread taked tokens" << std::endl;
        _tokens--;
        cv.notify_one();
        return true;
    }

    void acceptTokens() {
        while (_running) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::unique_lock lk(_mutex);
            cv.wait(lk, [this]{return _tokens < _maxTokens;});
            _tokens++;
            std::cout << "token was accept" << std::endl;
            cv.notify_all();
        }
    }

    void stop() {
        _running = false;
        cv.notify_all();
    }

private:
    int _maxTokens;
    int _tokens;

    std::mutex _mutex;
    std::condition_variable cv;
    std::atomic<bool> _running{true};
};

void func() {
    std::cout <<" thread  " <<std::this_thread::get_id() << "  is working" << std::endl;
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
            if (!throtller.Throttle()) {
                return;
            }
            func();
        });
    }
    throtller.stop();
    worker.join();

    for (auto& thred : threadVector) {
        thred.join();
    }


    return 0;
}