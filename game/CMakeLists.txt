set(INCLUDES_DIR include)

set(
    INCLUDES
    ${INCLUDES_DIR}/gachiball.hpp
)

set(
    SOURCES
    src/gachiball.cpp
    src/main.cpp
)

add_executable(gachiball ${SOURCES} ${INCLUDES})

target_include_directories(
  gachiball
  PUBLIC
  ${INCLUDES_DIR}
)
