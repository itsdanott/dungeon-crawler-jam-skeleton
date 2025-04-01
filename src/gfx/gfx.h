// Created by o:tone on 2025-Apr-01

#ifndef GFX_H
#define GFX_H

#include <SDL3/SDL.h>

namespace dungeoncrawlerjam2025::gfx {
#if !defined(SDL_PLATFORM_EMSCRIPTEN)
inline constexpr int  GL_VERSION_MAJOR   = 3;
inline constexpr int  GL_VERSION_MINOR   = 3;
inline constexpr auto GLSL_VERSION       = "#version 330 core";
inline constexpr auto GL_CONTEXT_PROFILE =
#if !defined(SDL_PLATFORM_MACOS)
    SDL_GL_CONTEXT_PROFILE_CORE;
#else
    SDL_GL_CONTEXT_PROFILE_COMPATIBILITY;
#endif

#else
inline constexpr int  GL_VERSION_MAJOR   = 3;
inline constexpr int  GL_VERSION_MINOR   = 0;
inline constexpr auto GLSL_VERSION       = "#version 330 es";
inline constexpr auto GL_CONTEXT_PROFILE = SDL_GL_CONTEXT_PROFILE_ES;
#endif
}
#endif //GFX_H
