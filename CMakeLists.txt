cmake_minimum_required(VERSION 2.8)

# Project name without spaces or special characters
project("DrawAttack")

set(APP_TITLE "DrawAttack")
set(APP_DESCRIPTION "Multiplayer drawing battle game.")
set(APP_AUTHOR "Cruel")
set(APP_ICON ${PROJECT_SOURCE_DIR}/res/app/icon.png)
set(BANNER_IMAGE ${PROJECT_SOURCE_DIR}/res/app/banner.png)
set(BANNER_SOUND ${PROJECT_SOURCE_DIR}/res/app/banner.wav)

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
	${PROJECT_SOURCE_DIR}/src/RoundedLine.cpp
	${TWEEN_ENGINE_SOURCE}
)

set(SHADER_AS nihstro)
FILE(GLOB_RECURSE SHADER_FILES ${PROJECT_SOURCE_DIR}/res/shaders/[^.]*)

##################################################
# Don't usually need to change anything below
##################################################
set(CPP3DS_ARM_FLAGS "-g -O2 -fno-rtti -fno-exceptions -march=armv6k -mtune=mpcore -mfloat-abi=hard")
set(CPP3DS_EMU_FLAGS "-g -O0")

set(CPP3DS_ARM_LIBS cpp3ds-window cpp3ds-graphics cpp3ds-audio cpp3ds-network cpp3ds-system gl3ds ctru freetype png z vorbisenc vorbisfile vorbis ogg)
set(CPP3DS_EMU_LIBS cpp3ds-emu sfml-graphics sfml-window sfml-system sfml-network sfml-audio GLEW GL jpeg freetype vorbisenc vorbisfile vorbis ogg)

add_subdirectory(${CPP3DS}/cmake/template_arm build/arm)
#add_subdirectory(${CPP3DS}/cmake/template_emu build/emu)
