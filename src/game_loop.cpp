#include "game_loop.h"
#include "system/time.h"

namespace Game {
    void game_loop(App& app, const GameLoopConfig& config) {

        // Update game at fixed timesteps to have game systems update at a predictable rate
        constexpr f64 timestep_sec = 1.0 / 60.0;
        constexpr f64 timestep_ms = timestep_sec * 1000.0;

        // How far the game time is behind the app time
        f64 game_lag_ms = 0.0;

        // Use the duration of the last game loop cycle to increment game time lag
        TimePoint last_cycle_start_time = Time::now();

        while (app.running) {
            TimePoint cycle_start_time = Time::now();
            f64 last_cycle_duration_ms = Time::as<Microseconds>(cycle_start_time - last_cycle_start_time).count() / 1000.0;

            // If the last cycle lasted too long, assume that we have resumed from a breakpoint or similar
            // and override the duration to the target timestep to avoid big spikes in game systems.
            if (last_cycle_duration_ms > 1000.0) {
                last_cycle_duration_ms = timestep_ms;
            }

            last_cycle_start_time = cycle_start_time;
            game_lag_ms += last_cycle_duration_ms;

            //
            // EVENTS
            //

            glfwPollEvents();

            if (glfwWindowShouldClose(app.window)) {
                stop(app);
            }

            //
            // UPDATE
            //

            // Update game time at fixed timesteps to have game systems update at a predictable rate.
            // Whenever the game time lags behind the app time by one-or-more timesteps, tick the game
            // time forwards until it's caught up.
            while (game_lag_ms >= timestep_ms) {
                config.on_update(app, timestep_sec);
                game_lag_ms -= timestep_ms;
            }

            //
            // RENDER
            //

            config.on_render(app);
        }
    }
}