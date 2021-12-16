# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.15)

project ("Eksamen" VERSION 1.0)

# executable file creation
add_executable(${PROJECT_NAME})

# Add subdirectory with dependencies
add_subdirectory(external)

# Additional source files added here
target_sources(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/source/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/source/main.h 
    "source/window.cpp" 
    "source/window.h" 
    "source/game.cpp" 
    "source/game.h" 
    "source/Shader.cpp" 
    "source/Shader.h" 
    "source/RenderHandler.cpp" 
    "source/RenderHandler.h" 
     "source/Tunnel.cpp" 
     "source/Tunnel.h" 
    "source/ImguiHandler.h" 
    "source/ImguiHandler.cpp"
    "source/Block.h" 
    "source/Block.cpp"
     "source/TextureLoader.cpp" 
     "source/TextureLoader.h"
    ${CMAKE_CURRENT_LIST_DIR}/source/imgui/imconfig.h
    ${CMAKE_CURRENT_LIST_DIR}/source/imgui/imgui.cpp
    ${CMAKE_CURRENT_LIST_DIR}/source/imgui/imgui_demo.cpp
    ${CMAKE_CURRENT_LIST_DIR}/source/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/source/imgui/imgui.h
    ${CMAKE_CURRENT_LIST_DIR}/source/imgui/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/source/imgui/imgui_impl_glfw.h
    ${CMAKE_CURRENT_LIST_DIR}/source/imgui/imgui_impl_opengl3.cpp
    ${CMAKE_CURRENT_LIST_DIR}/source/imgui/imgui_impl_opengl3.h
    ${CMAKE_CURRENT_LIST_DIR}/source/imgui/imgui_internal.h
    ${CMAKE_CURRENT_LIST_DIR}/source/imgui/imgui_widgets.cpp )

# Include directories
target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/source
    ${CMAKE_SOURCE_DIR}/stb/include
    )

# Linking to the external libraries
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    glad::glad
    glfw
    glm
    tinyobjloader
    stbimage::stbimage)

    # Copying the blockout.vs file to current binary directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/blockout.vs
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/shaders)

# Copying the blockout.fs file to current binary directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/blockout.fs
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/shaders)

# Copying the picture file to current binary directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/grid_tile.png
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/textures)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/active_block_tile.png
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/textures)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/layer_1.png
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/textures)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/layer_10_blue.png
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/textures)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/layer_9_red.png
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/textures)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/layer_8_yellow.png
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/textures)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/layer_7_white.png
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/textures)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/layer_6_orange.png
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/textures)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/layer_5_dblue.png
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/textures)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/layer_4_pink.png
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/textures)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/layer_3_babyblue.png
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/textures)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/layer_2_ubrown.png
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/textures)



# Copy resources to binary directory
add_custom_target(
    copy_shaders ALL
    ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/resources
    COMMAND ${CMAKE_COMMAND} -E echo "Copied resource directory to binary directory ${CMAKE_CURRENT_BINARY_DIR}"
)