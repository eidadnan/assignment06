cmake_minimum_required(VERSION 3.17)
project(assignment06)



link_libraries(cs4722  glfw3 opengl32 glu32)

set(CMAKE_CXX_STANDARD 20)

include_directories(C:/cs4722lib/include cs4722lib/include)
link_directories(C:/cs4722lib cs4722lib)

#add_executable(v01 v01/mixed-colors.cpp artifact_rt2.cpp)
add_executable(assign06 temple.cpp)
configure_file(vertex_shader.glsl .)
configure_file(fragment_shader.glsl .)