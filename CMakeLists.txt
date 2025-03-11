cmake_minimum_required(VERSION 3.10)

project(ImgProcLib VERSION 1.0.1 DESCRIPTION "A lightweight C++ library you need for image processing")

set(CMAKE_BUILD_TYPE Release)

# 安裝庫和 CMake 配置文件
install(TARGETS ImgProc EXPORT ImgProcTargets
        ARCHIVE DESTINATION lib
        INCLUDES DESTINATION include)

install(EXPORT ImgProcTargets
        FILE ImgProcConfig.cmake
        DESTINATION lib/cmake/ImgProc)

install(FILES include/ImgProc.h
        DESTINATION include)

# 包含目錄
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(ImgProc STATIC src/ImgProc.cpp)
