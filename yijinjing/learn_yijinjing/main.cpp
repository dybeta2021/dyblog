#include <iostream>
#include <algorithm>
#include <ctime>
#include <thread>
#include "mini_yijinjing/journal/JournalReader.h"
#include "mini_yijinjing/journal/JournalWriter.h"
#include "mini_yijinjing/journal/Timer.h"

yijinjing::JournalWriterPtr writer;
yijinjing::JournalReaderPtr reader;
constexpr uint64_t kLoopTimes = 100000UL;
struct TestData
{
    double last_price;
    double open_price;
    double highest_price;
    double lowest_price;
    int64_t local_timestamp_us;
};


int main() {
    std::cout << "Hello, World!" << std::endl;

    writer = yijinjing::JournalWriter::create(".", "BM_yijinjing", "writer");
    reader = yijinjing::JournalReader::create(".", "BM_yijinjing", yijinjing::getNanoTime(), "reader");

    // write
    TestData tick{};
    for (uint64_t i = 0; i < 5000000; ++i) {
        timespec ts{};
        clock_gettime(CLOCK_MONOTONIC, &ts);
        tick.local_timestamp_us = ts.tv_nsec + ts.tv_sec * 1000000000UL;
        writer->write_data(tick, 0, 0);
    }


    // reader
    std::vector<uint64_t> time_cost;
    yijinjing::FramePtr frame;
    for (uint64_t i = 0; i < 5000000; ++i) {
        do {
            frame = reader->getNextFrame();
        } while (!frame);
        auto* tick_r = reinterpret_cast<TestData*>(frame->getData());
        timespec ts{};
        clock_gettime(CLOCK_MONOTONIC, &ts);
        time_cost.emplace_back(ts.tv_nsec + ts.tv_sec * 1000000000UL - tick_r->local_timestamp_us);
    }

    sort(time_cost.begin(), time_cost.end());

    uint64_t sum = 0;
    for (auto v : time_cost) {
        sum += v;
    }
    printf("mean:%lu, min:%lu, 25th:%lu, 50th:%lu 75th:%lu max:%lu\n", sum / kLoopTimes, time_cost[0],
           time_cost[time_cost.size() / 4], time_cost[time_cost.size() / 2],
           time_cost[time_cost.size() * 3 / 4], *time_cost.rbegin());
    return 0;
}
