cmake_minimum_required(VERSION 3.10)

project(ImgProcLib VERSION 1.0.1 DESCRIPTION "A lightweight C++ library you need for image processing")

# 設定 package 的標頭檔目錄
set(PACKAGE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 設定 package 的來源檔案
set(PACKAGE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ImgProc.cpp
)

# 建立靜態連結庫
add_library(Image-Processing STATIC ${PACKAGE_SOURCES})

# 指定標頭檔目錄
target_include_directories(Image-Processing PUBLIC ${PACKAGE_INCLUDE_DIR})

# 安裝靜態庫
install(TARGETS Image-Processing
        ARCHIVE DESTINATION lib)

# 安裝標頭檔
install(FILES ${PACKAGE_INCLUDE_DIR}/ImgProc.h DESTINATION include)
