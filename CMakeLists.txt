cmake_minimum_required(VERSION 3.14)
project(sjkxq_stl VERSION 0.1.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 添加头文件目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# 添加库目标
add_library(sjkxq_stl INTERFACE)
target_include_directories(sjkxq_stl INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# 启用测试
enable_testing()
add_subdirectory(tests)

# 安装规则
install(TARGETS sjkxq_stl
    EXPORT sjkxq_stlTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/ DESTINATION include)

# 导出目标
install(EXPORT sjkxq_stlTargets
    FILE sjkxq_stlTargets.cmake
    NAMESPACE sjkxq_stl::
    DESTINATION lib/cmake/sjkxq_stl
)