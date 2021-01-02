#include <functional>
#include <chrono>
#include <future>
#include <cstdio>

class CallBackTimer
{
public:
    CallBackTimer()
    :_execute(false)
    {}

    void start(int interval, std::function<void(void)> func)
    {
        _execute = true;
        std::thread([this, interval, func]() {
            while (_execute)
            {
                func();
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(interval));
            }
        }).detach();
    }

    void stop()
    {
        _execute = false;
    }

private:
    bool            _execute;
};