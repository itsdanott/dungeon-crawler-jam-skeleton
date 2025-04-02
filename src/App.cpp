// Created by o:tone on 2025-Mar-31

#include "App.h"

#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <battery/embed.hpp>
#include "gfx/gfx.h"
#include "logging.h"

namespace dcjam {
bool App::init() {
    return init_sdl_opengl() && init_internal();
}

void App::cleanup() {
    cleanup_internal();
    cleanup_sdl_opengl();
}

SDL_AppResult App::process_event(const SDL_Event* event) {
    switch (event->type) {
    case SDL_EVENT_QUIT: return SDL_APP_SUCCESS;
    case SDL_EVENT_WINDOW_RESIZED:
        log_msg(
            "Window Resized: callback data: %dx%d",
            event->window.data1, event->window.data2
        );
        window.width  = event->window.data1;
        window.height = event->window.data2;
    //TODO: re-implement viewport re-generation
        break;

    case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
        window.fullscreen = true;
        break;

    case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
        window.fullscreen = false;
        break;

    case SDL_EVENT_WINDOW_FOCUS_GAINED:
        has_focus = true;
        last_tick = SDL_GetTicks();
        break;

    case SDL_EVENT_WINDOW_FOCUS_LOST:
        has_focus = false;
        break;

    case SDL_EVENT_KEY_DOWN:
        if (event->key.key == SDLK_ESCAPE)
            return SDL_APP_SUCCESS;

    default: return SDL_APP_CONTINUE;
    }

    return SDL_APP_CONTINUE;
}

void App::iterate() {
    if (!has_focus) return;
    const u64 now       = SDL_GetTicks();
    u64       time_diff = now - last_tick;
    SDL_GetMouseState(&mouse.pos_x, &mouse.pos_y);
    while (time_diff >= TICK_RATE_IN_MS) {
        tick();
        last_tick += TICK_RATE_IN_MS;
        time_diff = now - last_tick;
    }

    draw();

    //placeholder for actual viewport clearing implementation
    glViewport(0, 0, window.width, window.height);
    glClearColor(0, 1, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_GL_SwapWindow(window.sdl);
}

bool App::init_sdl_opengl() {
    if (!SDL_SetAppMetadata(APP_TITLE, APP_VERSION, APP_IDENTIFIER)) {
        log_error("Failed to set SDL AppMetadata: %s", SDL_GetError());
        return false;
    }

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        log_error("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }
#if !defined(SDL_PLATFORM_EMSCRIPTEN)
    //make 1:1 aspect ratio fit the display for desktop
    const SDL_DisplayID    display              = SDL_GetPrimaryDisplay();
    const SDL_DisplayMode* display_mode         = SDL_GetDesktopDisplayMode(display);
    const i32              smaller_display_size = static_cast<i32>(
        static_cast<float>(SDL_min(
            display_mode->w, display_mode->h
        )) * 0.9f);

    window.width  = smaller_display_size;
    window.height = smaller_display_size;
#endif

    window.sdl = SDL_CreateWindow(
        APP_TITLE,
        window.width, window.height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY
    );

    if (!window.sdl) {
        log_error("Failed to create Window: %s", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gfx::GL_VERSION_MAJOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gfx::GL_VERSION_MINOR);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, gfx::GL_CONTEXT_PROFILE);

    window.gl_context = SDL_GL_CreateContext(window.sdl);
    SDL_GL_MakeCurrent(window.sdl, window.gl_context);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress))) {
        log_error("Failed to load OpenGL via Glad: %s", SDL_GetError());
        return false;
    }

    last_tick = SDL_GetTicks();

    return true;
}

bool App::init_internal() {
    log_msg("Txt loaded via embed: %s", b::embed<"test.txt">().str().c_str());
    return true;
}

void App::cleanup_sdl_opengl() const {
    SDL_GL_DestroyContext(window.gl_context);
    if (window.sdl) {
        SDL_DestroyWindow(window.sdl);
    }
}

void App::cleanup_internal() {}

void App::tick() {}
void App::draw() {}
} // dcjam
