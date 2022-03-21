
set(PNG_LIB "PNG (lodepng)" CACHE STRING "PNG (lodepng)")
set_property(CACHE PNG_LIB PROPERTY STRINGS
    "PNG (lodepng)(system)"
    "PNG (lodepng)"
    "PNG (libpng)(system)"
    "BMP"
)
option(SYSTEM_XXHASH "Use the system-provided xxhash library" OFF)

# decide which image lib to use
find_package(lodepng QUIET)
find_package(PNG QUIET)

if (LODEPNG_FOUND AND PNG_LIB STREQUAL "PNG (lodepng)(system)")
    set(imglib lodepng)
    add_compile_options(-DLODEPNG)
elseif (PNG_FOUND AND PNG_LIB STREQUAL "PNG (libpng)(system)")
    set(imglib png)
    add_compile_options(-DLIBPNG)
elseif (PNG_LIB STREQUAL "PNG (lodepng)")
    set(imglib lodepng)
    add_compile_options(-DLODEPNG)
    include_directories(lodepng)
    add_subdirectory   (lodepng)
endif()

find_package(xxhash QUIET)
if (NOT USE_SYSTEM_XXHASH OR NOT XXHASH_FOUND)
    include_directories(xxhash)
    add_subdirectory   (xxhash)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dkcomp)
add_subdirectory   (dkcomp)

set(stripper_src
    spec1.c level1.c
    spec2.c level2.c raw_bit2.c
    spec3.c level3.c raw_bit3.c
    dkc_gbc.c dkl.c
    dkc_gba.c     gba_misc.c    gba_misc.h
    dkc2_gba.c dkc2_decomp.c dkc2_decomp.h
    kos.c jc.c
    rom.c rom.h
    misc.c misc.h
    bitplane.c bitplane.h
    png.c main.h
)

add_library(stripper SHARED ${stripper_src})
target_link_libraries(stripper PUBLIC dkcomp)
target_link_libraries(stripper PUBLIC ${imglib} xxhash)

target_include_directories(stripper PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

find_package(OpenMP)
if (OpenMP_C_FOUND)
    target_link_libraries(stripper PUBLIC OpenMP::OpenMP_C)
endif()

