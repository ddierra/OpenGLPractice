cmake_minimum_required(VERSION 3.10)
project(Practice)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
    src/main.cpp
    third_party/glad/src/glad.c
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    third_party/glfw/include
    third_party/glad/include
)

# Link GLFW
target_link_directories(${PROJECT_NAME} PRIVATE
    third_party/glfw/build/src
)
target_link_libraries(${PROJECT_NAME} glfw3)

# Platform-specific links
if (WIN32)
    target_link_libraries(${PROJECT_NAME} opengl32)
elseif(APPLE)
    find_library(OpenGL_LIBRARY OpenGL)
    target_link_libraries(${PROJECT_NAME} ${OpenGL_LIBRARY})
else()
    target_link_libraries(${PROJECT_NAME} GL)
endif()
