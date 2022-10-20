cmake_minimum_required(VERSION 3.10)

set(GAME tsl)

project(${GAME}
  VERSION 0.1.0
)

find_package(gf REQUIRED)

set(GAME_DATADIR "./data" CACHE PATH "Game Data Dir")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/code/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

add_executable(${GAME}
  code/main.cpp
  code/bits/GameHub.cpp
  code/bits/StartScene.cpp
  code/bits/TitleEntity.cpp
  code/bits/MenuScene.cpp
  # and the rest of your sources
)
target_link_libraries(${GAME}
  PRIVATE
    gf::graphics
    gf::net


)
