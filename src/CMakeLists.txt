
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

add_library          (stripper SHARED ${stripper_src})
target_link_libraries(stripper PRIVATE dkcomp ${imglib} xxhash)

find_package(OpenMP)
if (OpenMP_C_FOUND)
    target_link_libraries(stripper PRIVATE OpenMP::OpenMP_C)
endif()

