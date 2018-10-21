project(Core)

find_package(eigen3 REQUIRED)

add_library(${PROJECT_NAME} SHARED
include/public/Core/CoreDll.h
include/public/Core/MathTypes.h
include/public/Core/Component.h
src/Component.cpp
include/public/Core/SceneObject.h
src/SceneObject.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ./include/public PRIVATE ./include/private)
target_link_libraries(${PROJECT_NAME} PUBLIC Eigen3::Eigen)

target_compile_definitions(${PROJECT_NAME} PRIVATE EXPORT_CORE_API)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX) 
else(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif(MSVC)