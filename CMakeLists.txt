cmake_minimum_required(VERSION 2.8)

# Project name without spaces or special characters
project("DrawAttack")

set(APP_TITLE "DrawAttack")
set(APP_DESCRIPTION "Multiplayer drawing battle game.")
set(APP_AUTHOR "Cruel")
set(APP_ICON ${PROJECT_SOURCE_DIR}/res/app/icon.png)

# CIA values
set(APP_UNIQUE_ID 0x7718)
set(APP_PRODUCT_CODE ${PROJECT_NAME})
set(BANNER_IMAGE ${PROJECT_SOURCE_DIR}/res/app/banner.png)
set(BANNER_SOUND ${PROJECT_SOURCE_DIR}/res/app/banner.wav)
set(CIA_CONFIG ${PROJECT_SOURCE_DIR}/res/app/cia.rsf)

# Include cpp3ds cmake module
set(CPP3DS $ENV{CPP3DS})
if(NOT CPP3DS)
	message(FATAL_ERROR "You need to set the env variable CPP3DS before compiling.")
endif()
set(CMAKE_MODULE_PATH "${CPP3DS}/cmake" ${CMAKE_MODULE_PATH})
include(cpp3ds)

include_directories(
	${PROJECT_SOURCE_DIR}/external/tween-engine/include
)

FILE(GLOB_RECURSE TWEEN_ENGINE_SOURCE ${PROJECT_SOURCE_DIR}/external/tween-engine/src/*.cpp)
set(SOURCE_FILES
	${PROJECT_SOURCE_DIR}/src/main.cpp
	${PROJECT_SOURCE_DIR}/src/DrawAttack.cpp
	${PROJECT_SOURCE_DIR}/src/DrawingBoard.cpp
	${PROJECT_SOURCE_DIR}/src/TweenTransformable.cpp
	${PROJECT_SOURCE_DIR}/src/SpeechBubble.cpp
	${PROJECT_SOURCE_DIR}/src/Notification.cpp
	${PROJECT_SOURCE_DIR}/src/ServerListItem.cpp
	${PROJECT_SOURCE_DIR}/src/ServerList.cpp
	${PROJECT_SOURCE_DIR}/src/ScoreBoard.cpp
	${PROJECT_SOURCE_DIR}/src/ChatLog.cpp
	${PROJECT_SOURCE_DIR}/src/RichText.cpp
	${PROJECT_SOURCE_DIR}/src/GUI/Button.cpp
	${PROJECT_SOURCE_DIR}/src/GUI/NinePatch.cpp
	${PROJECT_SOURCE_DIR}/src/Keyboard/Keyboard.cpp
	${PROJECT_SOURCE_DIR}/src/Keyboard/tinyxml2.cpp
	${PROJECT_SOURCE_DIR}/src/States/StateStack.cpp
	${PROJECT_SOURCE_DIR}/src/States/State.cpp
	${PROJECT_SOURCE_DIR}/src/States/TitleState.cpp
	${PROJECT_SOURCE_DIR}/src/States/ServerSelectState.cpp
	${PROJECT_SOURCE_DIR}/src/States/PlayState.cpp
	${PROJECT_SOURCE_DIR}/src/States/PauseState.cpp
	${PROJECT_SOURCE_DIR}/src/States/TransitionMessageState.cpp
	${PROJECT_SOURCE_DIR}/src/States/NameSelectState.cpp
	${PROJECT_SOURCE_DIR}/src/States/LoadingState.cpp
	${PROJECT_SOURCE_DIR}/src/States/ColorPickerState.cpp
	${PROJECT_SOURCE_DIR}/src/Network/Client.cpp
	${PROJECT_SOURCE_DIR}/src/Network/Player.cpp
	${PROJECT_SOURCE_DIR}/src/Network/NetworkEvents.cpp
	${TWEEN_ENGINE_SOURCE}
)
set(SERVER_SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/server.cpp
    ${PROJECT_SOURCE_DIR}/src/Network/Server.cpp
    ${PROJECT_SOURCE_DIR}/src/Network/ServerConfig.cpp
    ${PROJECT_SOURCE_DIR}/src/Network/Player.cpp
    ${PROJECT_SOURCE_DIR}/src/Network/NetworkEvents.cpp
	${PROJECT_SOURCE_DIR}/src/Keyboard/tinyxml2.cpp
)

set(SHADER_AS nihstro)
FILE(GLOB_RECURSE SHADER_FILES ${PROJECT_SOURCE_DIR}/res/shaders/[^.]*)

# Server target
include_directories(${CPP3DS}/include)
link_directories(${CPP3DS}/lib)
add_executable(${PROJECT_NAME}-server ${SERVER_SOURCE_FILES})
set_target_properties(${PROJECT_NAME}-server PROPERTIES COMPILE_DEFINITIONS "EMULATION")
set_target_properties(${PROJECT_NAME}-server PROPERTIES COMPILE_FLAGS "-std=c++11")
target_link_libraries(${PROJECT_NAME}-server cpp3ds-emu sfml-system)

##################################################
# Don't usually need to change anything below
##################################################
set(CPP3DS_ARM_FLAGS "-g -O2 -fno-rtti -fno-exceptions -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft")
set(CPP3DS_EMU_FLAGS "-g -O0")

set(CPP3DS_ARM_LIBS cpp3ds-window cpp3ds-graphics cpp3ds-audio cpp3ds-network cpp3ds-system gl3ds ctru freetype png z vorbisenc vorbisfile vorbis ogg bz2)
set(CPP3DS_EMU_LIBS cpp3ds-emu sfml-graphics sfml-window sfml-system sfml-network sfml-audio openal GLEW GL jpeg freetype vorbisenc vorbisfile vorbis ogg)

add_subdirectory(${CPP3DS}/cmake/template_arm build/arm)
add_subdirectory(${CPP3DS}/cmake/template_emu build/emu)
