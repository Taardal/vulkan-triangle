#ifdef DD_ENABLE_ASSERT
#include <iomanip>
namespace dd {
    void print_assert_error(std::string_view tag, std::string_view expression, std::string_view message) {
        i8 key_value_space = 11;
        std::cerr << std::left;
        std::cerr << "--------------------------------------------------------------------------------------------------------------" << std::endl;
        std::cerr << "[ASSERT] " << expression << std::endl;
        std::cerr << "--------------------------------------------------------------------------------------------------------------" << std::endl;
        std::cerr << std::setw(key_value_space) << "Tag:" << tag << std::endl;
        std::cerr << std::setw(key_value_space) << "Message:" << message << std::endl;
        std::cerr << "" << std::endl;
    }
}
#endif
