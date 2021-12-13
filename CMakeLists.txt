# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.15)

project ("Eksamen" VERSION 0.3)

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
    "source/Externs.h" 
    "source/RenderHandler.cpp" 
    "source/RenderHandler.h" 
    "source/Menu.cpp" 
    "source/Menu.h" 
    "source/Overlay.cpp" 
    "source/Overlay.h" "source/Cube.cpp" "source/Cube.h" 
    "source/ImguiHandler.h" "source/ImguiHandler.cpp"
    "source/Block.h" "source/Block.cpp"
    
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

    # Copying the triangle.vs file to current binary directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/triangle.vs
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/shaders)

# Copying the triangle.fs file to current binary directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/triangle.fs
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/shaders)

         # Copying the triangle.vs file to current binary directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/light_cube.vs
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/shaders)

# Copying the triangle.fs file to current binary directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/light_cube.fs
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/shaders)

                         # Copying the pacman.png file to current binary directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/grid_tile.png
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/textures)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/active_block_tile.png
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/textures)



     # Enable C++ 17
target_compile_features(
    ${PROJECT_NAME}
    PRIVATE
    cxx_std_17
)

# Copy resources to binary directory
add_custom_target(
    copy_shaders ALL
    ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/resources
    COMMAND ${CMAKE_COMMAND} -E echo "Copied resource directory to binary directory ${CMAKE_CURRENT_BINARY_DIR}"
)