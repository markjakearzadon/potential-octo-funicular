#include <atomic>
#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>
#include <vector>

std::atomic<bool> running(true);

void signalHandler(int) {
    running = false;
}

void worker(double duty_cycle) {
    using namespace std::chrono;
    const auto busy_time = duration<double>(duty_cycle);  // e.g., 0.5 seconds
    const auto idle_time =
        duration<double>(1.0 - duty_cycle);  // e.g., 0.5 seconds

    while (running) {
        auto start = high_resolution_clock::now();

        // Busy loop for busy_time
        while (duration<double>(high_resolution_clock::now() - start) <
                   busy_time &&
               running) {
            // Do nothing, just burn CPU cycles
        }

        // Sleep for idle_time
        std::this_thread::sleep_for(idle_time);
    }
}

int main() {
    std::signal(SIGINT, signalHandler);
    std::cout << "[INFO] Press Ctrl+C to stop.\n";

    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0)
        num_threads = 4;  // Fallback if unknown

    std::cout << "[INFO] Spawning " << num_threads << " threads...\n";

    double duty_cycle = 0.5;  // 50% CPU
    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (unsigned int i = 0; i < num_threads; i++) {
        threads.emplace_back(worker, duty_cycle);
    }

    for (auto &t : threads) {
        if (t.joinable())
            t.join();
    }

    std::cout << "\n[INFO] Program exiting.\n";
    return 0;
}
