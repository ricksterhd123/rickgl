cmake_minimum_required(VERSION 3.17)
project(rickgl C)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static library")

# Setup libraries
include(FetchContent)

FetchContent_Declare(stb GIT_REPOSITORY https://github.com/nothings/stb.git GIT_TAG master)
FetchContent_MakeAvailable(stb)

FetchContent_Declare(cglm GIT_REPOSITORY https://github.com/recp/cglm.git GIT_TAG master)
FetchContent_MakeAvailable(cglm)

FetchContent_Declare(nuklear GIT_REPOSITORY https://github.com/Immediate-Mode-UI/Nuklear.git GIT_TAG master)
FetchContent_MakeAvailable(nuklear)

include_directories(
    ${stb_SOURCE_DIR}
    ${cglm_SOURCE_DIR}/include
    ${nuklear_SOURCE_DIR}
)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten" OR ${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "Disable Assimp's internal ZLIB")
    set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "Build all Assimp's importers")
    set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Build Assimp's test suite")
    set(ASSIMP_NO_EXPORT ON CACHE BOOL "Disable Assimp's export")
    set(ASSIMP_BUILD_IRRXML OFF CACHE BOOL "Build Assimp's IRRXML")
    set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "Build Assimp obj importer")
    set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "Build Assimp's IRRXML")

    FetchContent_Declare(assimp GIT_REPOSITORY https://github.com/assimp/assimp.git GIT_TAG master)
    FetchContent_MakeAvailable(assimp)
endif()

# Setup executable
add_executable(rickgl "src/main.c")
target_include_directories(rickgl PUBLIC "src")
set_property(TARGET rickgl PROPERTY C_STANDARD 99)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
    set_target_properties(rickgl PROPERTIES OUTPUT_NAME "index")
    set(CMAKE_EXECUTABLE_SUFFIX .html)

    target_compile_options(rickgl
    PUBLIC "--use-port=contrib.glfw3")

    target_link_options(rickgl 
    PUBLIC "--use-port=contrib.glfw3"
    PUBLIC "-sMIN_WEBGL_VERSION=2"
    PUBLIC "-sMAX_WEBGL_VERSION=2"
    PUBLIC "-sFULL_ES3=1"
    PUBLIC "-sUSE_ZLIB=1"
    PUBLIC "-sALLOW_MEMORY_GROWTH"
    PUBLIC "-sFORCE_FILESYSTEM"
    PUBLIC "-sSINGLE_FILE")

    set_target_properties(rickgl PROPERTIES LINK_FLAGS "--embed-file ${CMAKE_SOURCE_DIR}/assets@/assets")
    target_link_libraries(rickgl PRIVATE assimp)
else()
    file(COPY "${CMAKE_SOURCE_DIR}/assets" DESTINATION "${CMAKE_BINARY_DIR}")

    FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG master)
    FetchContent_Declare(glad GIT_REPOSITORY https://github.com/SasLuca/MyGlad.git GIT_TAG master)

    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(glfw)
    FetchContent_MakeAvailable(glad)

    target_link_libraries(rickgl PRIVATE assimp glad glfw)
endif()
