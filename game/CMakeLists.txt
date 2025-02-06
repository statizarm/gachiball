set(INCLUDES_DIR include)

set(
    SOURCES
    src/main.cpp
)

add_executable(gachiball ${SOURCES})

target_include_directories(
  gachiball
  PUBLIC
  ${CMAKE_CURRENT_BINARY_DIR}/include
)
