# Pong (ncurses, C)

- Build: `make`
- Run: `./pong`
- Tests: `make tests`

Controls:
- Left/Right arrows to move
- q to quit

Architecture:
- `src/physics.*`: Simulation, emits events (no UI deps)
- `src/render.*`: ncurses UI, reacts to physics events, countdown
- `src/input.*`: non-blocking input
- `src/ai.*`: bot movement
- `src/config.h`: tuning params (speeds, UI timings, terminal size)
- `src/main.c`: fixed timestep loop, orchestrates modules

Config highlights:
- `BALL_INITIAL_SPEED`, `SPEED_PER_POINT`, `BOT_BASE_ACCELERATION`, `PLAYER_ACCELERATION`
- UI: `FLASH_FRAMES`, `COUNTDOWN_STEPS`, `COUNTDOWN_DELAY_MS`

Determinism/Testability:
- `physics_seed(unsigned int)` and `physics_set_random_provider(...)` to control RNG.


