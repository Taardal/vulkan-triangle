#pragma once

#include "app.h"

namespace Game {
    struct GameLoopConfig {
        std::function<void(App& app, f64 timestep)> on_update;
        std::function<void(App& app)> on_render;
    };

    void game_loop(App& app, const GameLoopConfig& config);
}