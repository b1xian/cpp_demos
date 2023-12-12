#ifndef TRUNK_VISION_PERCEPTION_TIME_UTIL_H
#define TRUNK_VISION_PERCEPTION_TIME_UTIL_H

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace trunk {
namespace perception {

class TimeUtil {

public:
static inline double GetTimeInMilliSecond() {
    auto time_now = std::chrono::high_resolution_clock::now();
    return time_now.time_since_epoch().count();
}

static inline uint64_t GetTimeInNanoSecond() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec * 1000000000 + ts.tv_nsec;
}


static inline std::string GetDateTimeStr(const std::string& date_fmt) {
    auto time=std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), date_fmt.c_str());
    return ss.str();
}

static inline std::pair<int, double> NanoSecondSplit(const int64_t& time_stamp) {
    return std::make_pair<uint32_t, double>(uint32_t(time_stamp * 1e-9), time_stamp % (uint64_t)1e+9 * 1e-9);
}

};

} // perception
} // trunk
#endif //TRUNK_VISION_PERCEPTION_TIME_UTIL_H
