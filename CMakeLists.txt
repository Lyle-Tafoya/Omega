cmake_minimum_required(VERSION 3.0.0)
project(Omega)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_STANDARD 23)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -pedantic -g")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DPDC_DLL_BUILD /D_CRT_SECURE_NO_WARNINGS")
  set(CMAKE_CXX_FLAGS_DEBUG "/Zi")
endif()

if(WIN32)
  include(ExternalProject)
  ExternalProject_Add(
    PDCursesMod
    PREFIX ${CMAKE_BINARY_DIR}/_deps
    GIT_REPOSITORY https://github.com/Bill-Gray/PDCursesMod
    GIT_TAG master
    INSTALL_COMMAND ""
    CMAKE_ARGS
      -DPDC_SDL2_BUILD=OFF
      -DPDC_SDL2_DEPS_BUILD=OFF
      -DPDC_GL_BUILD=OFF
  )
  include_directories(${CMAKE_BINARY_DIR}/_deps/src/PDCursesMod)
  link_directories(${CMAKE_BINARY_DIR}/_deps/src/PDCursesMod-build/wincon)
endif()

add_executable(omega
  src/abyss.cpp
  src/aux1.cpp
  src/aux2.cpp
  src/aux3.cpp
  src/char.cpp
  src/city.cpp
  src/clrgen.cpp
  src/command1.cpp
  src/command2.cpp
  src/command3.cpp
  src/country.cpp
  src/effect1.cpp
  src/effect2.cpp
  src/effect3.cpp
  src/env.cpp
  src/etc.cpp
  src/file.cpp
  src/gen1.cpp
  src/gen2.cpp
  src/guild1.cpp
  src/guild2.cpp
  src/house.cpp
  src/iinit.cpp
  src/interactive_menu.cpp
  src/inv.cpp
  src/item.cpp
  src/itemf1.cpp
  src/itemf2.cpp
  src/itemf3.cpp
  src/lev.cpp
  src/minit.cpp
  src/mmelee.cpp
  src/mmove.cpp
  src/mon.cpp
  src/move.cpp
  src/movef.cpp
  src/mspec.cpp
  src/mstrike.cpp
  src/mtalk.cpp
  src/omega.cpp
  src/priest.cpp
  src/save.cpp
  src/scr.cpp
  src/scrolling_buffer.cpp
  src/site1.cpp
  src/site2.cpp
  src/spell.cpp
  src/time.cpp
  src/trap.cpp
  src/util.cpp
  src/village.cpp
)

add_custom_command(
  TARGET omega POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/lib
    $<TARGET_FILE_DIR:omega>/lib
)

if(WIN32)
  add_dependencies(omega PDCursesMod)
  ExternalProject_Get_Property(PDCursesMod BINARY_DIR)
  add_custom_command(
    TARGET omega POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_BINARY_DIR}/_deps/src/PDCursesMod-build/wincon/pdcurses.dll
    $<TARGET_FILE_DIR:omega>
  )
  target_link_libraries(omega pdcurses)
else()
  find_package(Curses REQUIRED)
  target_link_libraries(omega PRIVATE ${CURSES_LIBRARIES})
endif()
