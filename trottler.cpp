#pragma once

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <stdexcept>
#include <thread>

class Throttler {
public:
    Throttler(int tokensPerSecond, int maxTokens);
    ~Throttler();

    void Throttle();
};