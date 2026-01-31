#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>

volatile sig_atomic_t gSignalStatus = 0;

void signalHandler(int sig) { gSignalStatus = sig; }

int main() {
    signal(SIGINT, signalHandler);
    bool running = true;
    auto last_tick = std::chrono::steady_clock::now();
    std::cout << "Tracker is running... (Press ctr+c to stop!) \n";
    while (running) {
        if (gSignalStatus != 0) {
            running = false;
        }
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<double> delta = now - last_tick;
        std::cout << delta.count() << std::endl;

        last_tick = std::chrono::steady_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }

    std::cout << "END" << std::endl;

    return 0;
}