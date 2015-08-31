cmake_minimum_required(VERSION 2.8)

# Project name without spaces or special characters
project("Pictionary3DS")

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

# Assign all sources, resources, and assets of our project.
# You may prefer not to use GLOB and instead define each file explicitly.
# See: http://stackoverflow.com/questions/1027247/best-way-to-specify-sourcefiles-in-cmake
FILE(GLOB_RECURSE TWEEN_ENGINE_SOURCE ${PROJECT_SOURCE_DIR}/external/tween-engine/src/*.cpp)
set(SOURCE_FILES
	${PROJECT_SOURCE_DIR}/src/main.cpp
	${PROJECT_SOURCE_DIR}/src/Pictionary.cpp
	${PROJECT_SOURCE_DIR}/src/DrawingBoard.cpp
	${PROJECT_SOURCE_DIR}/src/TweenTransformable.cpp
	${TWEEN_ENGINE_SOURCE}
)
FILE(GLOB_RECURSE AUDIO_FILES ${PROJECT_SOURCE_DIR}/res/audio/[^.]*)
FILE(GLOB_RECURSE IMAGE_FILES ${PROJECT_SOURCE_DIR}/res/images/[^.]*)
FILE(GLOB_RECURSE SHADER_FILES ${PROJECT_SOURCE_DIR}/res/shaders/[^.]*)
set(RESOURCE_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-res.cpp)

compile_shaders(SHADER_OUTPUT ${PROJECT_SOURCE_DIR}/res ${SHADER_FILES})

# This embeds all resource files into cpp3ds binaries.
# Make sure you use compiled shaders here. (SHADER_OUTPUT instead of SHADER_FILES)
compile_resources(
	"resources"
	"${PROJECT_SOURCE_DIR}/res"
	${RESOURCE_OUTPUT}
	${AUDIO_FILES} ${IMAGE_FILES} ${SHADER_OUTPUT}
)

##################################################
# Don't usually need to change anything below
##################################################
set(CPP3DS_ARM_FLAGS "-Wall -pedantic -O0 -fno-rtti -fno-exceptions -march=armv6k -mtune=mpcore -mfloat-abi=hard")
set(CPP3DS_EMU_FLAGS "-g -Wall -pedantic -O0")

set(CPP3DS_ARM_LIBS cpp3ds gl3ds ctru freetype png z)
set(CPP3DS_EMU_LIBS cpp3ds-emu sfml-graphics sfml-window sfml-system GLEW GL jpeg freetype)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_custom_target(${PROJECT_NAME}-res ALL DEPENDS ${RESOURCE_OUTPUT} ${SHADER_OUTPUT})
add_subdirectory(${CPP3DS}/cmake/template_arm arm)
add_subdirectory(${CPP3DS}/cmake/template_emu emu)
