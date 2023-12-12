#include <unistd.h>
#include <unordered_map>

#include "PerceptionRecorder.h"
#include <spdlog/fmt/bundled/ranges.h>

#include <sys/syscall.h>
#define gettid() syscall(__NR_gettid)

using namespace trunk::perception;

void test_thread_id() {
    std::ostringstream oss;
    oss << std::this_thread::get_id();
    std::string stid = oss.str();
    VLOGI_CONSOLE("sub p_id: {0}\n", syscall(__NR_gettid));

    spdlog::flush_every(std::chrono::milliseconds(1));
    //spdlog::flush_every()
    std::this_thread::sleep_for(std::chrono::seconds(60));
    std::this_thread::sleep_for(std::chrono::microseconds(60));
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    std::this_thread::sleep_for(std::chrono::nanoseconds (60));
}

int main() {
    MARK_ERROR(ErrorCode::INIT_CAMERA_DRIVER_FAILED);

    int fps = 10;
    MARK_ERROR_IF(15 < fps < 25, ErrorCode::CAMERA_FPS_ABNORMAL);

    uint64_t time_delay = 90;
    MARK_ERROR_IF(time_delay > 50, ErrorCode::CAMERA_DELAY_TOO_LONG);

    RECORD(PerceptRecordType::CAMERA_DRIVER, "fps:{0}, delay:{1}", fps, time_delay);

    int matched_radar_obj_num = 0;
    double match_radar_interval = 0.2;
    double match_radar_loc = 15;
    MARK_ERROR_IF(matched_radar_obj_num < 1, ErrorCode::RADAR_MATCH_FAILED);
    PerceptionRecorder::Record(PerceptRecordType::RADAR_MATCH, "interval:{0}, loc:{1}",
                               match_radar_interval, match_radar_loc);

    PerceptionRecorder::WriteRecord();

    std::vector<int> v = {1, 2, 3};
    fmt::print("{0}\n", v);

    std::cout << fmt::v8::format(" {d} ", 1);


    return 0;
}