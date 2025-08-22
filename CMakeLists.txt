cmake_minimum_required(VERSION 3.10)

project(STripper)

add_subdirectory(ext/dkcomp)
add_subdirectory(src)

set(CLI_SRC src/cli.c)
set(WIN_SRC src/win.c src/panel.c)
set(GTK_SRC src/gtk.c src/panel.c)

add_executable(stripper_cli ${CLI_SRC})
target_link_libraries(stripper_cli PUBLIC stripper)

if(WIN32)
  add_compile_options(-DWIN32_UI)
  add_executable(stripper_win ${WIN_SRC})
  set_target_properties(stripper_win PROPERTIES WIN32_EXECUTABLE TRUE)
  target_link_libraries(stripper_win PUBLIC stripper)
else()
  find_package(PkgConfig REQUIRED)
  if(PkgConfig_FOUND)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
  endif()

  if(GTK3_FOUND)
    add_compile_options(-DGTK3_UI)
    add_executable(stripper_gtk ${GTK_SRC})
    target_include_directories(stripper_gtk PRIVATE ${GTK3_INCLUDE_DIRS})
    target_link_libraries (stripper_gtk PUBLIC stripper)
    target_link_libraries (stripper_gtk PRIVATE ${GTK3_LIBRARIES})
    target_compile_options(stripper_gtk PRIVATE ${GTK3_CFLAGS_OTHER})
  endif()
endif()
