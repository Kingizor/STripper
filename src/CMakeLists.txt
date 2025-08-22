set(STRIPPER_SRC 
 bitplane.c dkc2_gba.c dkc_gba.c dkc_gbc.c
 dkl.c gba_misc.c jc.c kos.c level1.c level2.c
 level3.c misc.c png.c raw_bit2.c
 raw_bit3.c rom.c spec1.c spec2.c spec3.c
)
find_package(PNG REQUIRED)
find_package(OpenMP)
add_library(stripper ${STRIPPER_SRC})
target_link_libraries(stripper PUBLIC dkcomp)
target_link_libraries(stripper PRIVATE PNG::PNG)

if(OpenMP_C_FOUND)
  target_link_libraries(stripper PRIVATE OpenMP::OpenMP_C)
endif()
target_include_directories(stripper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
