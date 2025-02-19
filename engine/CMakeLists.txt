include(FetchContent)

FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad
    GIT_TAG        v2.0.6
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)

FetchContent_GetProperties(glad)
if(NOT glad_POPULATED)
    message("Fetching glad")
    FetchContent_MakeAvailable(glad)

    add_subdirectory("${glad_SOURCE_DIR}/cmake" glad_cmake)
    glad_add_library(glad REPRODUCIBLE EXCLUDE_FROM_ALL LOADER API gl:core=4.6 EXTENSIONS GL_ARB_bindless_texture GL_EXT_texture_compression_s3tc)
endif()

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

set(INCLUDES_DIR include)
set(
    INCLUDES
    ${INCLUDES_DIR}/body.hpp
    ${INCLUDES_DIR}/camera.hpp
    ${INCLUDES_DIR}/engine.hpp
    ${INCLUDES_DIR}/event.hpp
    ${INCLUDES_DIR}/event_dispatcher.hpp
    ${INCLUDES_DIR}/game.hpp
    ${INCLUDES_DIR}/input_engine.hpp
    ${INCLUDES_DIR}/input_event.hpp
    ${INCLUDES_DIR}/mesh.hpp
)
set(
    SOURCES
    src/camera.cpp
    src/engine.cpp
    src/event_dispatcher.cpp
    src/game.cpp
    src/input_engine.cpp
    src/mesh.cpp
)

add_library(engine ${SOURCES} ${INCLUDES})

target_include_directories(
  engine
  PUBLIC
  include
)

target_link_libraries(
  engine
  PUBLIC glfw
  PUBLIC OpenGL::GL
  PRIVATE glad
)
