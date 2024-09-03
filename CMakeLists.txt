cmake_minimum_required(VERSION 3.16)

project(Animation_with_DynamicCubeMap)

# Add source files
set(
    SOURCES
    main.cpp
    src/Camera.cpp
    src/Cube.cpp
    src/Shader.cpp
    src/Tokenizer.cpp
    src/Window.cpp
    src/Joint.cpp
    src/Skeleton.cpp
    src/Skin.cpp
    src/Keyframe.cpp
    src/Channel.cpp
    src/Animation.cpp
    src/Character.cpp
)

# Add header files
set(
    HEADERS
    include/core.h
    include/Camera.h
    include/Cube.h
    include/Shader.h
    include/Tokenizer.h
    include/Window.h
    include/DOF.h
    include/Joint.h
    include/Skeleton.h
    include/Skin.h
    include/Keyframe.h
    include/Channel.h
    include/Animation.h
    include/Character.h
    include/Material.h
    include/stb_image.h
    include/Sphere.h
)

set(
    IMGUI_HEADERS
    include/imgui/imconfig.h
    include/imgui/imgui.h
    include/imgui/imgui_impl_glfw.h
    include/imgui/imgui_impl_opengl3.h
    include/imgui/imgui_internal.h
    include/imgui/imstb_rectpack.h
    include/imgui/imstb_textedit.h
    include/imgui/imstb_truetype.h
)
set(
    IMGUI_SOURCES
    include/imgui/imgui.cpp
    include/imgui/imgui_demo.cpp
    include/imgui/imgui_draw.cpp
    include/imgui/imgui_impl_glfw.cpp
    include/imgui/imgui_impl_opengl3.cpp
    include/imgui/imgui_tables.cpp
    include/imgui/imgui_widgets.cpp
)

# Require GL
find_package(OpenGL REQUIRED)

# Add include directories
include_directories(
    include
)

# Add library directories
link_directories(
    lib
)

# Add imgui directories


# Add executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${IMGUI_HEADERS} ${IMGUI_SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glew32s.lib glfw3)

# Move assets to .exe
add_custom_target(CopyShaders ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${PROJECT_SOURCE_DIR}/shaders"
	"${CMAKE_BINARY_DIR}/Debug/shaders"
)
add_dependencies(Animation_with_DynamicCubeMap CopyShaders)

# Project 1
# Move skel files to the build directory
add_custom_target(CopySkel ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/skelFile"
    "${CMAKE_BINARY_DIR}/Debug/skelFile"
)
add_dependencies(${PROJECT_NAME} CopySkel)

# Project 2
# Move skin files to the build directory
add_custom_target(CopySkin ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/skinFile"
    "${CMAKE_BINARY_DIR}/Debug/skinFile"
)
add_dependencies(${PROJECT_NAME} CopySkin)

# Project 3
# Move anim files to the build directory
add_custom_target(CopyAnim ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/animFile"
    "${CMAKE_BINARY_DIR}/Debug/animFile"
)
add_dependencies(${PROJECT_NAME} CopyAnim)

# Project 5
# Move cubemap files to the build directory
add_custom_target(CopyCubemap ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/Cubemap"
    "${CMAKE_BINARY_DIR}/Debug/Cubemap"
)
add_dependencies(${PROJECT_NAME} CopyCubemap)

# Project 5
# Move Texture files to the build directory
add_custom_target(CopyTexture ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/Texture"
    "${CMAKE_BINARY_DIR}/Debug/Texture"
)
add_dependencies(${PROJECT_NAME} CopyTexture)