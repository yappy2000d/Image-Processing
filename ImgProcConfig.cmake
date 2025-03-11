# 假設庫在 src/ 目錄下，包含頭文件的目錄是 include/
set(ImgProc_LIBRARIES ImgProc)
set(ImgProc_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/../include)

# 提供所需的變量
set(ImgProc_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/../include)

# 將庫和包含目錄暴露給使用者
include_directories(${ImgProc_INCLUDE_DIRS})

# 定義對外提供的庫
add_library(ImgProc STATIC IMPORTED)
set_target_properties(ImgProc PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/../lib/libImgProc.a
)
