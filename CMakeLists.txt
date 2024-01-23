# 声明cmake的最低版本
cmake_minimum_required(VERSION 3.5)

# 声明项目名称
project(OpenGLBaseEngine)

# 引入第三方库
find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 17)

# 包含项目头文件
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/imgui)
include_directories(${PROJECT_SOURCE_DIR}/include/glad)
include_directories(${PROJECT_SOURCE_DIR}/include/SDL2)

add_library(IMGUI STATIC
    ${PROJECT_SOURCE_DIR}/ThirdParty/imgui/imgui.cpp
    ${PROJECT_SOURCE_DIR}/ThirdParty/imgui/imgui_demo.cpp
    ${PROJECT_SOURCE_DIR}/ThirdParty/imgui/imgui_draw.cpp
    ${PROJECT_SOURCE_DIR}/ThirdParty/imgui/imgui_impl_opengl3.cpp
    ${PROJECT_SOURCE_DIR}/ThirdParty/imgui/imgui_impl_sdl.cpp
    ${PROJECT_SOURCE_DIR}/ThirdParty/imgui/imgui_tables.cpp
    ${PROJECT_SOURCE_DIR}/ThirdParty/imgui/imgui_widgets.cpp
)

set(SDL2_PATH ${PROJECT_SOURCE_DIR}/ThirdParty/SDL2/lib/x64)
add_library(SDL2 SHARED IMPORTED)
set_target_properties(SDL2 PROPERTIES
    IMPORTED_IMPLIB ${SDL2_PATH}/SDL2.lib
)

target_link_libraries(IMGUI
    SDL2
    OpenGL::GL
)

# 生成可执行文件
add_executable(${PROJECT_NAME} 
    ./main.cpp 
    ./glad.c
)

target_link_libraries(${PROJECT_NAME} PRIVATE IMGUI)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                       "${SDL2_PATH}/SDL2.dll"
                       $<TARGET_FILE_DIR:${PROJECT_NAME}>)

# # 链接第三方库
# target_link_libraries(${PROJECT_NAME}
#     PUBLIC
#     ${PROJECT_SOURCE_DIR}/ThirdParty/SDL2/lib/x64/SDL2main.lib
#     ${PROJECT_SOURCE_DIR}/ThirdParty/SDL2/lib/x64/SDL2.dll
#     OpenGL::GL
# )