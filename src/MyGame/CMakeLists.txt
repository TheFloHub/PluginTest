project(MyGame)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(${PROJECT_NAME} 
src/main.cpp
src/InputManager.h
src/InputManager.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ./src ${OPENGL_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES} glfw GLEW::GLEW Core)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif(MSVC)