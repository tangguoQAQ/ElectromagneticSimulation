cmake_minimum_required (VERSION 3.8)

project(ElectromagneticSimulation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")

add_executable(${PROJECT_NAME} WIN32)

file(GLOB sources CONFIGURE_DEPENDS "./src/*.cpp" "./src/*.h")
target_sources(${PROJECT_NAME} PUBLIC ${sources})

target_include_directories(${PROJECT_NAME} PRIVATE 
    "N:/Visual Studio/VC/Tools/MSVC/14.39.33519/include"
    "N:/Windows Kits/10/Include/10.0.22000.0")
target_link_libraries(${PROJECT_NAME} d2d1.lib dwrite.lib)

# Set the character set to Unicode
target_compile_definitions(${PROJECT_NAME} PUBLIC UNICODE _UNICODE)
# Set the source character set to UTF-8
add_compile_options("/source-charset:utf-8")