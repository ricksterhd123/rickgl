cmake_minimum_required(VERSION 3.17)
project(rickgl C)

# Setup libraries
include(FetchContent)

FetchContent_Declare(cglm GIT_REPOSITORY https://github.com/recp/cglm.git GIT_TAG master)
FetchContent_MakeAvailable(cglm)

# Setup assets
file(COPY "${CMAKE_SOURCE_DIR}/assets" DESTINATION "${CMAKE_BINARY_DIR}")

# Setup executable

add_executable(rickgl "src/main.c")
target_include_directories(rickgl PUBLIC "src")
set_property(TARGET rickgl PROPERTY C_STANDARD 99)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static library")


if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
    set(CMAKE_EXECUTABLE_SUFFIX .html)

    target_compile_options(rickgl
    PUBLIC "--use-port=contrib.glfw3")

    target_link_options(rickgl 
    PUBLIC "--use-port=contrib.glfw3"
    PUBLIC "-sMIN_WEBGL_VERSION=2"
    PUBLIC "-sMAX_WEBGL_VERSION=2"
    PUBLIC "-sFULL_ES3=1"
    PUBLIC "-sFORCE_FILESYSTEM"
    PUBLIC "-sSINGLE_FILE")

    set_target_properties(rickgl PROPERTIES LINK_FLAGS "--embed-file ${CMAKE_SOURCE_DIR}/assets@/assets")
    target_link_libraries(rickgl PRIVATE cglm)
else()
    FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG master)
    FetchContent_Declare(glad GIT_REPOSITORY https://github.com/SasLuca/MyGlad.git GIT_TAG master)

    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(glfw)
    FetchContent_MakeAvailable(glad)

    target_link_libraries(rickgl PRIVATE cglm glad glfw)
endif()
