cmake_minimum_required(VERSION 3.6)
project (black_telephone)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(cmake_cxx_standard 17)

set(BLACK_TELEPHONE_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/engine/src/window.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/engine/src/database.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/engine/src/texture.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/engine/src/shader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/engine/src/timing.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/engine/src/sprite.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/engine/src/frame.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/engine/src/renderer.cpp
	)

add_executable(black_telephone main.cpp ${BLACK_TELEPHONE_SRCS})

set_property(TARGET black_telephone PROPERTY CXX_STANDARD 17)
set_property(TARGET black_telephone PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(black_telephone
	opengl32.lib
	${CMAKE_CURRENT_SOURCE_DIR}/dep/glfw/lib/glfw3.lib
	)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/dep
	${CMAKE_CURRENT_SOURCE_DIR}/engine
	${CMAKE_CURRENT_SOURCE_DIR}/utility
	${CMAKE_CURRENT_SOURCE_DIR}/game
	)

set_target_properties(black_telephone PROPERTIES COMPILE_FLAGS "/MD")