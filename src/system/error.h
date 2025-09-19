#pragma once

#include "log.h"

#define GM_THROW(message) \
    std::stringstream ss;\
    ss << message;\
    std::string message_string = ss.str();\
    throw ::Game::Error(message_string, GM_TAG)

#define GM_ASSERT_THROW(expression, message) \
    if (expression) {\
        /* Continue */\
    } else {\
        std::stringstream ss;\
        ss << "Assertion failed: " << #expression;\
        std::string msg(message);\
        if (!msg.empty()) {\
            ss << " (" << msg << ")";\
        }\
        throw Error(ss.str(), GM_TAG);\
    }

#define GM_EXECUTE_THROW(expression) \
    try {\
        expression;\
    } catch (const Error& e) { \
        throw Error(#expression, GM_TAG, std::make_shared<Error>(e));\
    } catch (const std::exception& e) {\
        throw Error(#expression, GM_TAG, std::make_shared<Error>(e.what()));\
    }

namespace Game {
    struct StacktraceEntry {
        std::string message;
        std::string tag;
    };

    class Error final : public std::exception {
    private:
        std::string message;
        std::string tag;
        std::shared_ptr<Error> previousError = nullptr;

    public:
        Error(const std::string& message, const std::string& tag = "", const std::shared_ptr<Error>& previousError = nullptr);

        const char* what() const noexcept override;

        std::vector<StacktraceEntry> getStacktrace() const;

        void printStacktrace() const;

        static void printStacktrace(const Error& error);
    };

    std::ostream& operator<<(std::ostream& os, const Error& error);
}
