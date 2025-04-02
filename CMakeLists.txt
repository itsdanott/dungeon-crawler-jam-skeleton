cmake_minimum_required(VERSION 3.26)
project(dungeon_crawler_jam_2025)
set(CMAKE_CXX_STANDARD 20)

# OUTPUT DIRECTORIES *********************************************************************
if (EMSCRIPTEN)
    set(OUT_DIR "${CMAKE_BINARY_DIR}/../bin-Emscripten")
else ()
    set(OUT_DIR "${CMAKE_BINARY_DIR}/../bin-$<CONFIGURATION>")
endif ()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUT_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUT_DIR}")

# SUB CMAKE DIRECTORIES ******************************************************************
add_subdirectory(third-party/SDL EXCLUDE_FROM_ALL)
add_subdirectory(third-party/glad)

# HTML ***********************************************************************************
if (EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html" CACHE INTERNAL "")
endif ()

# FETCH BATTERY::EMBED *******************************************************************
include(FetchContent)
FetchContent_Declare(
        battery-embed
        GIT_REPOSITORY https://github.com/batterycenter/embed.git
        GIT_TAG        "v1.2.19"
)
FetchContent_MakeAvailable(battery-embed)

# EXECUTABLE *****************************************************************************
add_executable(dungeon_crawler_jam_2025
        src/main.cpp
        src/App.cpp
        src/App.h
        src/App.cpp
        src/App.h
        src/types.h
        src/memory.h
        src/gfx/gfx.h
        src/logging.h
)

# EMBED ASSETS ***************************************************************************
add_subdirectory(assets)
#now we're embedding via proxy target to avoid polluting this cmake file + VCS
#b_embed(dungeon_crawler_jam_2025 assets/test.txt) # but this would be the way to embed

# TARGET *********************************************************************************
target_compile_options(dungeon_crawler_jam_2025 PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)
target_compile_features(dungeon_crawler_jam_2025 PRIVATE cxx_std_20)
target_link_libraries(dungeon_crawler_jam_2025 PRIVATE
    SDL3::SDL3
    glad
)

# DEBUG VS RELEASE ***********************************************************************
option(LEAK_DETECTION "Activate Leak detection" OFF)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(__DEBUG__)
    if (LEAK_DETECTION)
        add_compile_definitions("__LEAK_DETECTION__")
    endif ()
    SET(B_EMBED_SILENCE_DEVMODE_WARNING ON CACHE BOOL "Override Embed Warning" FORCE)
else()
    SET(B_PRODUCTION_MODE ON CACHE BOOL "Override Embed Production Mode" FORCE)
endif ()

# EMSCRIPTEN *****************************************************************************
if (EMSCRIPTEN)
    SET(B_PRODUCTION_MODE ON CACHE BOOL "we don't have hot reloading in web" FORCE)

    set_target_properties(dungeon_crawler_jam_2025 PROPERTIES
        SUFFIX ".html"
        OUTPUT_NAME "index"
    )

    set(EMSCRIPTEN_LINK_FLAGS
        "-std=c++20"
        "-sMAX_WEBGL_VERSION=2"
        "--shell-file=${CMAKE_SOURCE_DIR}/source_assets/web/shell_minimal.html"
        # if we'll have large files this can be used again, for now we are fine with embed
        # "--use-preload-plugins"
        #        "--preload-file=${CMAKE_SOURCE_DIR}/assets@/assets"
        "-sSTACK_SIZE=1000000" # 1 MB Stack size
    )
    string(JOIN " " LINK_FLAGS_STR ${EMSCRIPTEN_LINK_FLAGS})

    set_target_properties(dungeon_crawler_jam_2025 PROPERTIES
            LINK_FLAGS "${LINK_FLAGS_STR}"
    )
endif ()

# COMPILER SPECIFIC***********************************************************************

# MINGW **********************************************************************************
if (CMAKE_SYSTEM_NAME STREQUAL "Windows" AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message("MinGW")
    target_link_libraries(dungeon_crawler_jam_2025 PRIVATE -static -pthread)
endif ()

# MSVC ***********************************************************************************
# we use MSVC CRT to detect memory leaks on Windows
if (CMAKE_SYSTEM_NAME STREQUAL "Windows" AND CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message("MSVC")
    if (LEAK_DETECTION)
        add_compile_definitions("__MSVC_CRT_LEAK_DETECTION__")
    endif ()
endif ()