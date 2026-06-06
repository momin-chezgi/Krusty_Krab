#include "Utility/IDGenerator.h"

#include <atomic>
#include <chrono>
#include <iomanip>
#include <random>
#include <sstream>

namespace {
std::string hexBlock(std::mt19937_64& generator, int width)
{
    std::uniform_int_distribution<unsigned long long> distribution(0, 15);
    std::ostringstream out;
    for (int i = 0; i < width; ++i) {
        out << std::hex << distribution(generator);
    }
    return out.str();
}
}

ID_tp IDGenerator::uuid()
{
    static std::atomic<unsigned long long> counter{0};
    static std::mt19937_64 generator(
        static_cast<unsigned long long>(
            std::chrono::high_resolution_clock::now().time_since_epoch().count()
        )
    );

    const auto sequence = counter++;
    std::ostringstream out;
    out << hexBlock(generator, 8) << "-"
        << hexBlock(generator, 4) << "-"
        << hexBlock(generator, 4) << "-"
        << hexBlock(generator, 4) << "-"
        << hexBlock(generator, 8)
        << std::hex << std::setw(4) << std::setfill('0') << (sequence & 0xffff);

    return out.str();
}
