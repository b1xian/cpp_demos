#ifndef TRUNK_VISION_PERCEPTION_RECORDER_H
#define TRUNK_VISION_PERCEPTION_RECORDER_H

#include <sstream>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "ErrorCode.h"
#include "Log.h"
#include "PerceptRecordType.h"
#include "TimeUtil.h"

#define MARK_ERROR(...) PerceptionRecorder::MarkError(__VA_ARGS__)
#define MARK_ERROR_IF(cond, ...) PerceptionRecorder::MarkErrorIf(cond, __VA_ARGS__)

#define RECORD(...) PerceptionRecorder::Record(__VA_ARGS__)
#define RECORD_IF(cond, ...) PerceptionRecorder::RecordIf(cond, __VA_ARGS__)

namespace trunk {
namespace perception {

struct PerceptionRecord {
    uint64_t thread_id;     // thread id
    PerceptRecordType record_type;     // 错误码
    std::string desc;     // 错误码

    PerceptionRecord() = default;
    PerceptionRecord(PerceptRecordType record_type, const std::string& desc) :
                     record_type(record_type), desc(desc) {
        std::ostringstream oss;
        oss << std::this_thread::get_id();
        std::string stid = oss.str();
        thread_id = std::stoull(stid);
    }

    std::string ToString() {
        return record_type_desc_map.at(record_type) +
               "(" + std::to_string(thread_id) + ") " + desc;
    }
};

// 记录严重错误信息
struct ErrorInfo {
    uint64_t record_time;   // 发生时间
    uint64_t thread_id;     // thread id
    ErrorCode err_code;     // 错误码

    ErrorInfo() = default;
    ErrorInfo(ErrorCode err_code) : err_code(err_code) {
        record_time = TimeUtil::GetTimeInNanoSecond();

        std::ostringstream oss;
        oss << std::this_thread::get_id();
        std::string stid = oss.str();
        thread_id = std::stoull(stid);
    }

    std::string ToString() {
        return "{" + std::to_string(record_time) + " " + std::to_string(thread_id) + " " +
               std::to_string(err_code) + " " + err_code_desc_map_.at(err_code) + "}";
    }
};


class PerceptionRecorder {
public:
    PerceptionRecorder() = default;
    ~PerceptionRecorder() = default;
    static int Init();

    static void MarkError(ErrorCode err_code);
    static void MarkErrorIf(bool cond, ErrorCode err_code);

    template <typename... Args>
    static void Record(PerceptRecordType record_type, Args&&... args) {
        records_.emplace_back(PerceptionRecord(record_type, fmt::v8::format(args...)));
    }

    template <typename... Args>
    void RecordIf(PerceptRecordType record_type, Args&&... args) {
        Record(record_type, args...);
    }

    static void WriteRecord();

public:
    const static std::string logger_name;

private:
    static std::vector<ErrorInfo> errors_;
    static std::vector<PerceptionRecord> records_;
    const static std::string log_path;

};


}  // namespace perception
}  // namespace trunk

#endif // TRUNK_VISION_PERCEPTION_RECORDER_H
