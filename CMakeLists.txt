cmake_minimum_required(VERSION 3.10)
project(MouseTracker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include paths
include_directories(
        ${PROJECT_SOURCE_DIR}/include
)

# OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# X11 for global mouse tracking
find_package(X11 REQUIRED)
include_directories(${X11_INCLUDE_DIR})

# Source files
set(SOURCES
        src/main.cpp
        src/kalman_filter.cpp
)

# Create executable
add_executable(MouseTracker ${SOURCES})

# Link OpenCV and X11
target_link_libraries(MouseTracker ${OpenCV_LIBS} ${X11_LIBRARIES})
