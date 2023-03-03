cmake_minimum_required(VERSION 3.10)

project(tsl
  VERSION 0.1.0
)

include(GNUInstallDirs)

find_package(gf REQUIRED COMPONENTS graphics net)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/modules")
  
if (UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -O3")
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(SFML2 REQUIRED sfml-audio>=2.1)
elseif (WIN32)
  find_package(SFML 2.1 REQUIRED COMPONENTS audio)
  set(SFML2_LIBRARIES "sfml-audio")
endif ()

set(TSL_DATA_DIR "${CMAKE_INSTALL_PREFIX}/share/games/tsl" CACHE PATH "data path")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/code/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if(NOT MSVC)
  add_definitions(-Wall -Wextra -g -pedantic)
else ()
  add_definitions(/DNOMINMAX /W4 /utf-8 /permissive-)
endif()

#client

add_executable(client_tsl
  code/client.cc
  code/bits/client/GameHub.cc
  code/bits/client/AudioManager.cc
  code/bits/client/CommonScene.cc
  code/bits/client/DownloadScene.cc
  code/bits/client/DownloadEntity.cc
  code/bits/client/StartScene.cc
  code/bits/client/StartEntity.cc
  code/bits/client/MenuScene.cc
  code/bits/client/MenuEntity.cc
  code/bits/client/ConnectionScene.cc
  code/bits/client/ConnectionEntity.cc
  code/bits/client/RulesScene.cc
  code/bits/client/RulesEntity.cc
  code/bits/client/SettingsScene.cc
  code/bits/client/SettingsEntity.cc
  code/bits/client/Singletons.cc
  code/bits/client/WaitingScene.cc
  code/bits/client/WaitingEntity.cc
  code/bits/client/GameScene.cc
  code/bits/client/PlayerInfo.cc
  
  code/bits/common/Network.cc
  code/bits/common/GamePhase.cc
  code/bits/common/Deck.cc
  code/bits/common/Card.cc

  vendor/imgui/imgui.cpp
  vendor/imgui/imgui_demo.cpp
  vendor/imgui/imgui_draw.cpp
  vendor/imgui/imgui_widgets.cpp

  vendor/gf-imgui/imgui_impl_gf.cc
)

target_include_directories(client_tsl
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/gf-imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    ${CMAKE_CURRENT_SOURCE_DIR}/code/bits
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(client_tsl
  PRIVATE
    ${GF_LIBRARIES}
    ${SFML2_LIBRARIES}
)

install(
  TARGETS client_tsl
  RUNTIME DESTINATION bin
)

#server

add_executable(server_tsl
  code/server.cc

  code/bits/server/AnnexFonctionServer.cc
  
  code/bits/common/Network.cc
  code/bits/common/GamePhase.cc
  code/bits/common/Deck.cc
  code/bits/common/Card.cc
)

target_include_directories(server_tsl
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    ${CMAKE_CURRENT_SOURCE_DIR}/code/bits
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(server_tsl
  PRIVATE
		${GF_LIBRARIES}
)

install(
  TARGETS server_tsl
  RUNTIME DESTINATION bin
)

# data

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/tsl"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games"
)