#include "run.h"

int main() {
    Game::AppConfig config{
        .title = "Game",
    };
    try {
        Game::run(config);
    } catch (const Game::Error& e) {
        GM_LOG_CRITICAL("Fatal error");
        e.printStacktrace();
        return EXIT_FAILURE;
    } catch (const std::exception& e) {
        GM_LOG_CRITICAL("Fatal error: {}", e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
