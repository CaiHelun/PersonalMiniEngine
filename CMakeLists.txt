# 声明cmake的最低版本
cmake_minimum_required(VERSION 3.5)

# 声明项目名称
project(OpenGLBaseEngine)

# 引入第三方库
find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE IMGUIFILES
${PROJECT_SOURCE_DIR}/IMGUI/include/*.h
${PROJECT_SOURCE_DIR}/IMGUI/source/*.c*
)
source_group(TREE ${PROJECT_SOURCE_DIR}/ FILES ${IMGUIFILES})

add_library(IMGUI STATIC ${IMGUIFILES})

target_include_directories(IMGUI PUBLIC
    ${PROJECT_SOURCE_DIR}/IMGUI/include
)

# 包含项目头文件
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/glad
    ${PROJECT_SOURCE_DIR}/include/SDL2
    ${PROJECT_SOURCE_DIR}/include/assimp
)

set(SDL2_PATH ${PROJECT_SOURCE_DIR}/ThirdParty/SDL2/lib/x64)
add_library(SDL2 SHARED IMPORTED)
set_target_properties(SDL2 PROPERTIES
    IMPORTED_IMPLIB ${SDL2_PATH}/SDL2.lib
)

set(ASSIMP_PATH ${PROJECT_SOURCE_DIR}/ThirdParty/assimp)
add_library(ASSIMP SHARED IMPORTED)
set_target_properties(ASSIMP PROPERTIES
    IMPORTED_IMPLIB ${ASSIMP_PATH}/assimp-vc142-mt.lib
)

target_link_libraries(IMGUI
    SDL2
    ASSIMP
    OpenGL::GL
)

include_directories(${PROJECT_SOURCE_DIR}/Engine/glm)

file(GLOB_RECURSE ENGINEFILES
${PROJECT_SOURCE_DIR}/Engine/*.h
${PROJECT_SOURCE_DIR}/Engine/*.c*
${PROJECT_SOURCE_DIR}/Engine/Shader/*.glsl
${PROJECT_SOURCE_DIR}/Engine/Image/*.h
${PROJECT_SOURCE_DIR}/Engine/Image/*.c*
)
source_group(TREE ${PROJECT_SOURCE_DIR}/ FILES ${ENGINEFILES})
# 生成可执行文件
add_executable(${PROJECT_NAME} ${ENGINEFILES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE IMGUI)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                       "${SDL2_PATH}/SDL2.dll" "${ASSIMP_PATH}/assimp-vc142-mt.dll"
                       $<TARGET_FILE_DIR:${PROJECT_NAME}>)

# # 链接第三方库
# target_link_libraries(${PROJECT_NAME}
#     PUBLIC
#     ${PROJECT_SOURCE_DIR}/ThirdParty/SDL2/lib/x64/SDL2main.lib
#     ${PROJECT_SOURCE_DIR}/ThirdParty/SDL2/lib/x64/SDL2.dll
#     OpenGL::GL
# )