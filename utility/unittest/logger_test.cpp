#include "logger_checkpoints.h"
#include "helpers.h"
#include <thread>

using namespace beam;

struct XXX {
    int z = 333;
};

std::ostream& operator<<(std::ostream& os, const XXX& xxx) {
    os<< "XXX={" << xxx.z << "}";
    return os;
}

void test_logger_1() {
    LoggerConfig c;
    c.fileLevel = LOG_LEVEL_WARNING;
    c.filePrefix = "Zzzzz";
    auto logger = Logger::create (c);

    LOG_CRITICAL() << "Let's die";
    LOG_ERROR() << "Not so bad at all, here is " << format_timestamp("%y-%m-%d.%T", local_timestamp_msec());
    LOG_WARNING() << "Don't be afraid: " << 223322223;
    XXX xxx;
    LOG_INFO() << xxx;
    LOG_DEBUG() << "YYY";
    LOG_VERBOSE() << "ZZZ";
}

void test_ndc_1() {
    LoggerConfig c;
    c.fileLevel = LOG_LEVEL_WARNING;
    c.filePrefix = "Zzzzz";
    auto logger = Logger::create (c);
    CHECKPOINT_CREATE (6);
    CHECKPOINT_ADD() << "ssss" << 333 << 555;
    CHECKPOINT_CREATE (6);
    CHECKPOINT_ADD() << "zzz" << 777 << 888;
    std::string zzz("Blablabla");
    CHECKPOINT_ADD() << &zzz; // constraint: objects captured by ptr in checkpoints
    CHECKPOINT (3333, 44444, 5555, 66666, 77777, 88888);
    LOG_ERROR() << FlushAllCheckpoints();
}

void test_ndc_2(bool exc)
{
    LoggerConfig c;
    auto logger = Logger::create (c);
    {
        std::thread::id threadId = std::this_thread::get_id();
        CHECKPOINT("WorkerThread:", &threadId); // constraint: objects captured by ptr in checkpoints
        CHECKPOINT("Processing I/O");
        {
            CHECKPOINT("Request from client ID:", 246);
            {
                if (exc) throw("xxx");
                CHECKPOINT("Sending a file. Path:", "C:\\Blablabla.bin");
                {
                    LOG_ERROR() << "Can't open file. Error:" << EACCES << FlushAllCheckpoints();
                }
            }
        }
    }
}

int main() {
    test_logger_1();
    test_ndc_1();
    test_ndc_2(false);
    try {
        test_ndc_2(true);
    }
    catch(...) {}
}
