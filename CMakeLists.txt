cmake_minimum_required(VERSION 3.10)

set(GAME tsl)

project(${GAME}
  VERSION 0.1
)

include(GNUInstallDirs)

find_package(gf REQUIRED COMPONENTS graphics net)
#find_package(SFML REQUIRED COMPONENTS audio)

if (UNIX)
	message(STATUS ">>> Unix")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -O3")
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(SFML2 REQUIRED sfml-audio>=2.1)
elseif (WIN32)
	message(STATUS ">>> Win")
	find_package(SFML 2.1 REQUIRED COMPONENTS audio)
	set(SFML2_LIBRARIES "sfml-audio")
endif ()

# set(GAME_DATADIR "${CMAKE_INSTALL_PREFIX}/share/games/${GAME}" CACHE PATH "Game Data Dir")
# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/code/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

if(NOT MSVC)
	message(STATUS ">>> MSCV")
  	add_definitions(-Wall -Wextra -g -pedantic)
else ()
	message(STATUS ">>> NOT MSCV")
  	add_definitions(/DNOMINMAX /W4 /utf-8 /permissive-)
endif()

add_executable(client_tsl
	client/AudioManager.cpp
	client/Singletons.cpp

	client/client.cpp
	client/Network.cpp
	
	model/Piece.cpp
	model/Plateau.cpp
	model/Case.cpp
	model/Card.cpp
	model/Deck.cpp
	model/CardFunction.cpp
	model/CardFunctionChooseCases.cpp
	model/GamePhase.cpp
	
	client/GameHub.cpp
	client/GameData.cpp
	client/Scene/StartScene.cpp		
	client/Scene/MenuScene.cpp
	client/Scene/PlaySelectScene.cpp
	client/Scene/GameScene.cpp
	client/Scene/RulesScene.cpp
	client/Scene/QuitScene.cpp
	client/Scene/DownloadScene.cpp
	client/Scene/SettingsScene.cpp
	client/Scene/ZoomCardScene.cpp

	client/Entity/BoardEntity.cpp
	client/Entity/TableBoardEntity.cpp
	client/Entity/PlayTitleEntity.cpp
	client/Entity/TitleEntity.cpp
	client/Entity/RulesEntity.cpp
	client/Entity/MainEntity.cpp
	client/Entity/PoseEntity.cpp
	client/Entity/CardEntity.cpp
	client/Entity/SettingsEntity.cpp

	)

add_executable(server_tsl
	server/server.cpp
	model/Plateau.cpp
	model/Case.cpp
	model/Card.cpp
	model/Deck.cpp
	model/Piece.cpp
	model/CardFunction.cpp
	model/CardFunctionChooseCases.cpp
	server/AnnexFctServer.cpp
	model/GamePhase.cpp
	
)

target_link_libraries(client_tsl
	PRIVATE
		gf::graphics
		gf::net
		${SFML2_LIBRARIES}
)

target_link_libraries(server_tsl
	PRIVATE
		gf::net
)

install(
  TARGETS server_tsl client_tsl
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

# tools

# add_executable(tiles code/tiles.cc)
# target_link_libraries(tiles gf::core)
