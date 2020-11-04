#buildフォルダ内でcmake ..
# 変数を宣言し、値を入れる。
set(MAIN_CODE client_main)
set(INPUT_CODE client_input)
set(KEYBORD_CODE client_KeybordInput)
set(NET_CODE net)
#set(WINDOW_CODE window)
set(GRAPHIC_CODE graphic)
set(SYSTEM_CODE client_system)
set(MOVE_CODE client_move)
set(MAP_CODE client_map)
set(SORYUTION_NAME client)

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

#ヘッダファイルのパスを指定
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2ttf_INCLUDE_DIRS} ${SDL2image_INCLUDE_DIRS})

# 実行ファイル名とソース指定
add_executable( ${SORYUTION_NAME} ${MAIN_CODE} ${INPUT_CODE} ${KEYBORD_CODE} ${NET_CODE} ${GRAPHIC_CODE} ${SYSTEM_CODE} ${MOVE_CODE} ${MAP_CODE})

#リンク先のライブラリを指定
target_link_libraries( ${SORYUTION_NAME} ${SDL2_LIBRARIES}  ${SDL2ttf_LIBRARIES} ${SDL2image_LIBRARIES} -lGL -lGLU)
