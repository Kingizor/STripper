cmake_minimum_required(VERSION 3.10)

project(STripper)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# add_compile_options(-Wall -Werror -Wextra -Wno-unknown-pragmas)


find_package(PkgConfig QUIET)
if (PKGCONFIG_FOUND)
    pkg_check_modules(GTK gtk+-3.0)
    if (GTK_FOUND)
        option(BUILD_GTK3 "" ON)
    endif()
endif()

if (WIN32)
    option(BUILD_WIN32 "" ON)
endif()

option(BUILD_GTK3  "Build the GTK3  interface" OFF)
option(BUILD_WIN32 "Build the Win32 interface" OFF)
option(BUILD_CLI   "Build the CLI   interface" ON)
option(SYSTEM_XXHASH "Use the system-provided xxhash library" OFF)

set(PNG_LIB "PNG (lodepng)" CACHE STRING "PNG (lodepng)")
set_property(CACHE PNG_LIB PROPERTY STRINGS
    "PNG (lodepng)(system)"
    "PNG (lodepng)"
    "PNG (libpng)(system)"
    "BMP"
)




# decide which image lib to use
find_package(lodepng QUIET)
find_package(PNG     QUIET)

if (LODEPNG_FOUND AND PNG_LIB STREQUAL "PNG (lodepng)(system)")
    message(STATUS "using system lodepng")
    set(imglib lodepng)
    add_compile_options(-DLODEPNG)
elseif (PNG_FOUND AND PNG_LIB STREQUAL "PNG (libpng)(system)")
    message(STATUS "using system libpng")
    set(imglib png)
    add_compile_options(-DLIBPNG)
elseif (PNG_LIB STREQUAL "PNG (lodepng)")
    message(STATUS "using external lodepng")
    set(imglib lodepng)
    add_compile_options(-DLODEPNG)
    include_directories(ext/lodepng)
    add_subdirectory   (ext/lodepng)
endif()

find_package(xxhash QUIET)
if (NOT XXHASH_FOUND OR NOT SYSTEM_XXHASH)
    message(STATUS "using external xxhash")
    include_directories(ext/xxhash)
    add_subdirectory   (ext/xxhash)
else()
    message(STATUS "using system xxhash")
endif()

include_directories(ext/dkcomp)
add_subdirectory   (ext/dkcomp)

include_directories(src)
add_subdirectory   (src)



set(gui_common src/panel.c src/panel.h src/gui.h)

if (BUILD_GTK3)
    add_executable            (stripper_gtk src/gtk.c ${gui_common})
    target_compile_definitions(stripper_gtk PRIVATE -DGTK3_UI)
    target_link_libraries     (stripper_gtk PUBLIC stripper)
    target_link_libraries     (stripper_gtk PUBLIC ${GTK_LIBRARIES})
    target_include_directories(stripper_gtk PUBLIC ${GTK_INCLUDE_DIRS})
    target_link_directories   (stripper_gtk PUBLIC ${GTK_LIBRARY_DIRS})
    target_compile_options    (stripper_gtk PUBLIC ${GTK_CFLAGS_OTHER})
endif()

if (BUILD_WIN32)
    add_executable(stripper_win WIN32 src/win.c src/win.rc ${gui_common})
    target_compile_definitions(stripper_win PRIVATE -DWIN32_UI)
    target_link_libraries     (stripper_win PUBLIC stripper)
endif()

if (BUILD_CLI)
    add_executable       (stripper_cli src/cli.c)
    target_link_libraries(stripper_cli PUBLIC stripper)
endif()
