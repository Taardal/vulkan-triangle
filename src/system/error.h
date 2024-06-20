#pragma once

#include "log.h"

#define DD_THROW(message) throw Error(message, DD_TAG())

#define DD_ASSERT_THROW(expression) \
    if (expression) {\
        /* Continue */\
    } else {\
        throw Error(std::string("Assertion failed: ") + #expression, DD_TAG());\
    }

#define DD_EXECUTE_THROW(expression) \
    try {\
        expression;\
    } catch (const Error& e) { \
        throw Error(#expression, DD_TAG(), std::make_shared<Error>(e));\
    } catch (const std::exception& e) {\
        throw Error(#expression, DD_TAG(), std::make_shared<Error>(e.what()));\
    }

namespace dd {
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
