#include "app.h"

int main() {
    try {
        Game::run();
    } catch (const Game::Error& e) {
        std::cerr << "Error" << std::endl;
        e.printStacktrace();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
