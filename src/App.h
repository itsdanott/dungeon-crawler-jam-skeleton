// Created by o:tone on 2025-Mar-31

#ifndef APP_H
#define APP_H

#include <SDL3/SDL.h>
#include "types.h"

#define APP_TITLE "Dungeon Crawler Jam 2025"
#define APP_VERSION "0.1.0"
#define APP_IDENTIFIER "com.author.dcjam"

#if defined(SDL_PLATFORM_EMSCRIPTEN)
#define APP_WINDOW_WIDTH  360
#define APP_WINDOW_HEIGHT 360
#else
#define APP_WINDOW_WIDTH  640
#define APP_WINDOW_HEIGHT 640
#endif

#define TICK_RATE_IN_MS 16
constexpr float DELTA_TIME = static_cast<float>(TICK_RATE_IN_MS) / static_cast<float>(
    SDL_MS_PER_SECOND);

namespace dcjam {
struct Window {
    SDL_Window*   sdl;
    SDL_GLContext gl_context;
    i32           width, height;
    bool          fullscreen;
};

struct Mouse {
    float pos_x, pos_y;
};

class App {
public:
    bool          init();
    void          cleanup();
    SDL_AppResult process_event(const SDL_Event* event);
    void          iterate();

private:
    bool init_sdl_opengl();
    bool init_internal();
    void cleanup_sdl_opengl() const;
    void cleanup_internal();
    void tick();
    void draw();

    Window window = {
        .sdl = nullptr,
        .gl_context = nullptr,
        .width = APP_WINDOW_WIDTH,
        .height = APP_WINDOW_HEIGHT,
        .fullscreen = false,
    };
    Mouse mouse     = {};
    u64   last_tick = 0;
    bool  has_focus = false;
};
} //dcjam

#endif //APP_H
