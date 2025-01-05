############################################### CMAKE SETTINGS ###############################################

cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

############################################### PROJECT SETTINGS ###############################################

project(CGJ_Solo_Project)

include(FetchContent)

# adds our executables
add_executable(SoloProject src/main.cpp)

############################################### FETCH LIBRARIES ###############################################

# confirms that OpenGL is installed and available
find_package(OpenGL REQUIRED)

# Fetch GLM from its official repository
FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

# Fetch GLFW from its official repository
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

# Fetch GLEW from a fork that has a CMake build system
FetchContent_Declare(
    glew-cmake
    GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
    GIT_TAG glew-cmake-2.2.0
)

FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG v5.2.5
)

# Make the libraries available to the project
FetchContent_MakeAvailable(glm)
FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(glew-cmake)
FetchContent_MakeAvailable(assimp)

######################################## CREATE STATIC LIB FOR TANGRAM ########################################

# define the source files needed to create a static library "tangram_lib" for the project
set(TANGRAM_SOURCE_FILES 
    "libs/mgl/mglShader.cpp"
    "libs/mgl/mglError.cpp"
    "libs/mgl/mglApp.cpp"
    "libs/mgl/mglMesh.cpp"
    "libs/mgl/mglCamera.cpp"
    "libs/mgl/mglSceneNode.cpp"
    "libs/mgl/mglScenegraph.cpp"

    "src/app/app.cpp"
)

add_library(tangram_lib STATIC ${TANGRAM_SOURCE_FILES})

target_include_directories(tangram_lib PUBLIC libs)
target_link_libraries(tangram_lib PRIVATE glm::glm glfw libglew_static OpenGL::GL assimp)

############################################### LINK LIBRARIES ################################################

# link the libraries to the executables
target_link_libraries(SoloProject PRIVATE glm::glm glfw libglew_static OpenGL::GL tangram_lib assimp)
