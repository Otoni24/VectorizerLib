# CMakeList.txt : CMake project for VoidLevelEditor, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("VectorizerLib")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add source to this project's executable.
add_library (VectorizerLib STATIC
    "src/Core/Vectorizer.cpp"
    "src/IO/ImageLoader.cpp"
    "src/Math/Math.cpp"
)

target_include_directories(VectorizerLib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(VectorizerLib PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/stb
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/nlohmann
)
