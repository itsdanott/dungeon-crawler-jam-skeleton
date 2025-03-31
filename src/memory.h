// Created by o:tone on 2025-Mar-31

#ifndef MEMORY_H
#define MEMORY_H

#if !defined(__LEAK_DETECTION__)
#define MEM_malloc SDL_malloc
#define MEM_free SDL_free
#define MEM_new new
#else
#define MEM_malloc malloc
#define MEM_free free
#if defined(__MSVC_CRT_LEAK_DETECTION__)
#define MEM_new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif
#endif

#endif //MEMORY_H
