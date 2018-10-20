project(Core)

add_library(${PROJECT_NAME} SHARED
include/public/Core/SceneObject.h
src/SceneObject.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ./include/public PRIVATE ./include/private)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif(MSVC)