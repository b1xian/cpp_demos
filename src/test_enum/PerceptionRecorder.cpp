#include "PerceptionRecorder.h"


namespace trunk {
namespace perception {

std::vector<ErrorInfo> PerceptionRecorder::errors_;
std::vector<PerceptionRecord> PerceptionRecorder::records_;
const std::string PerceptionRecorder::logger_name("PerceptionRecorder");
const std::string PerceptionRecorder::log_path(
        "/home/sheldon/work_space/code/dev_code/深入应用CPP11/src/test_enum/vision_front_" +
        TimeUtil::GetDateTimeStr("%Y%m%d-%H%M%S") + ".record");

int PerceptionRecorder::Init() {
    static std::once_flag init_spd_flag;
    std::call_once(init_spd_flag, [&] {
        Logger::CreateLogger(logger_name, log_path);
        VLOGI_LOGGER(logger_name, "init PerceptionRecorder~");
    });
}

void PerceptionRecorder::MarkError(ErrorCode err_code) {
    errors_.emplace_back(ErrorInfo(err_code));
}

void PerceptionRecorder::MarkErrorIf(bool cond, ErrorCode err_code) {
    if (cond) MarkError(err_code);
}

void PerceptionRecorder::WriteRecord() {
    std::string record_str{""};
    for (auto record : records_) {
        record_str += "\n" + record.ToString();
    }
    VLOGI_LOGGER(logger_name, "{}", record_str);
    records_.clear();

    if (!errors_.empty()) {
        record_str.clear();
        record_str += "\nERR_CODE:[";
        for (ErrorInfo err_info : errors_) {
            record_str += err_info.ToString() + " ";
        }
        record_str += "]";
    }
    VLOGE_LOGGER(logger_name, "{}", record_str);
    errors_.clear();
}

 //static init
#define INIT_RECORDER_RESULT_VAR(file, line) Init_recorder_result_##file_##line
#define INIT_RECORDER()                                       \
    static int INIT_RECORDER_RESULT_VAR(__FILE__, __LINE__) = \
                    PerceptionRecorder::Init();
INIT_RECORDER()
#undef INIT_RECORDER
#undef INIT_RECORDER_RESULT_VAR
}  // namespace perception
}  // namespace trunk