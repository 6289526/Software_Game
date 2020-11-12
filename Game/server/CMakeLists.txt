#buildフォルダ内でcmake ..
# 変数を宣言し、値を入れる。
set(SORYUTION_NAME server)
set(SERVER_CODE
    server_main
    net
    sys
    server_map
)
# CMakeの最低バージョンを記述
cmake_minimum_required(VERSION 2.8)

# ソリューション名を指定
project( ${SORYUTION_NAME} )

# SDL2のパッケージを探す
find_package(SDL2 REQUIRED)
find_package(PkgConfig)
pkg_check_modules(SDL2ttf REQUIRED SDL2_ttf)
find_package(PkgConfig)
pkg_check_modules(SDL2image REQUIRED SDL2_image)
set(CMAKE_CXX_FLAGS "-O0 -g")
#ヘッダファイルのパスを指定
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2ttf_INCLUDE_DIRS} ${SDL2image_INCLUDE_DIRS})

# 実行ファイル名とソース指定
add_executable( ${SORYUTION_NAME} ${SERVER_CODE})

#リンク先のライブラリを指定
target_link_libraries( ${SORYUTION_NAME} ${SDL2_LIBRARIES}  ${SDL2ttf_LIBRARIES} ${SDL2image_LIBRARIES})
