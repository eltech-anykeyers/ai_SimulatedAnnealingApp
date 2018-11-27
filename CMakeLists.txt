cmake_minimum_required(VERSION 3.1.0)

# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

project(ai_simulated_annealing)

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)

include_directories(
    include
)

# Headers
set(HEADERS_LIST
    include/simulated_annealing_app.hpp
    include/simulated_annealing_widget.hpp
)

# Sources
set(SOURCES_LIST
    src/main.cpp
    src/simulated_annealing_app.cpp
    src/simulated_annealing_widget.cpp
)

# Tell CMake to create the helloworld executable
add_executable(${PROJECT_NAME} ${SOURCES_LIST} ${HEADERS_LIST})

# Use the Widgets module from Qt 5
qt5_use_modules(${PROJECT_NAME} Widgets)
