// Created by o:tone on 2025-Apr-02

#ifndef LOG_H
#define LOG_H

#include <SDL3/SDL.h>

namespace dcjam {
inline void log_msg(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    SDL_LogMessageV(
        SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, fmt, args
    );
    va_end(args);
}

inline void log_warning(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    SDL_LogMessageV(
        SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, fmt, args
    );
    va_end(args);
}

inline void log_error(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    SDL_LogMessageV(
        SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, fmt, args
    );
    va_end(args);
}
}

#endif //LOG_H
