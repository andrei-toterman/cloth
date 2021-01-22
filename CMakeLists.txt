cmake_minimum_required(VERSION 3.17)
project(cloth)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/ball.cpp
        src/ball.hpp
        src/cloth.cpp
        src/cloth.hpp
        src/constraint.cpp
        src/constraint.hpp
        src/particle.cpp
        src/particle.hpp
        src/shader.cpp
        src/shader.hpp
        src/state.hpp
        src/vertex.hpp
        )

cmake_policy(SET CMP0072 NEW)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} glfw GLEW::glew OpenGL::GL)
