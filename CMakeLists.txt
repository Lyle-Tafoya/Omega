cmake_minimum_required(VERSION 3.5.0)
project(Omega)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 13)
    message(FATAL_ERROR "GCC version must be 13 or higher")
  endif()
  set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -pedantic -g")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  message(WARNING "Clang cannot be used to build Omega if MULTI_USER_SYSTEM is defined")
  set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -pedantic -g")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
  string(REPLACE "/W3" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS_DEBUG "/W3 /Zi")
endif()

if(WIN32)
  if(NOT DEFINED PDC_GL_BUILD)
    set(PDC_GL_BUILD ON)
  endif()
  if(PDC_GL_BUILD MATCHES ON)
    set(PDC_WINDOW_TYPE "gl")
    include_directories(
      ${CMAKE_BINARY_DIR}/_deps/src/PDCursesMod/gl
      ${CMAKE_BINARY_DIR}/installed/Release/include/SDL2
      ${CMAKE_BINARY_DIR}/installed/Release/include/SDL2_ttf
    )
    link_directories(${CMAKE_BINARY_DIR}/installed/Release/lib)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DPDC_GL_BUILD")
  else()
    set(PDC_GL_BUILD "OFF")
    set(PDC_WINDOW_TYPE "wincon")
  endif()
  include(ExternalProject)
  ExternalProject_Add(
    PDCursesMod
    PREFIX ${CMAKE_BINARY_DIR}/_deps
    GIT_REPOSITORY https://github.com/Lyle-Tafoya/PDCursesMod.git
    GIT_TAG v4.3.7
    INSTALL_DIR ${CMAKE_BINARY_DIR}/installed
    CMAKE_ARGS
      -DPDC_BUILD_SHARED=OFF
      -DPDC_SDL2_BUILD=OFF
      -DPDC_SDL2_DEPS_BUILD=${PDC_GL_BUILD}
      -DPDC_GL_BUILD=${PDC_GL_BUILD}
      -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
      -DCMAKE_BUILD_TYPE=Release
      -DCMAKE_CONFIGURATION_TYPES=Release
    BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --config Release
    INSTALL_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --config Release --target install
    UPDATE_COMMAND ""
  )
  include_directories(${CMAKE_BINARY_DIR}/_deps/src/PDCursesMod)
  link_directories(${CMAKE_BINARY_DIR}/installed/Release/lib/${PDC_WINDOW_TYPE})
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
  if(PDC_GL_BUILD MATCHES "ON")
    set_target_properties(omega PROPERTIES
      WIN32_EXECUTABLE TRUE
    )
    set(RELEASE_LIBS
      pdcursesstatic
      SDL2main
      SDL2-static
      SDL2_ttf-static
      zlibstatic
      freetype
      winmm
      setupapi
      version
      imm32
      msvcrt
    )
    set(DEBUG_LIBS
      pdcursesstatic
      SDL2main
      SDL2-static
      SDL2_ttf-static
      zlibstatic
      freetype
      winmm
      setupapi
      version
      imm32
      msvcrtd
    )
  else()
    set(RELEASE_LIBS
      pdcursesstatic
      winmm
      msvcrt
    )
    set(DEBUG_LIBS
      pdcursesstatic
      winmm
      msvcrtd
    )
  endif()
  target_link_libraries(omega
    $<$<CONFIG:Debug>:${DEBUG_LIBS}>
    $<$<CONFIG:Release>:${RELEASE_LIBS}>
  )
else()
  find_package(Curses REQUIRED)
  target_link_libraries(omega PRIVATE ${CURSES_LIBRARIES})
endif()
