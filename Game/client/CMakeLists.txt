#g++ -o client client_main.cpp client_input.cpp client_WiiInput.cpp net.cpp graphic.cpp client_system.cpp client_move.cpp client_map.cpp -lm -lSDL2 -lSDL2_image -lSDL2_ttf -lGL -lGLU -lcwiimote -lbluetooth -L/usr/lib -I/usr/local/include/libcwiimote-0.4.0/libcwiimote

#buildフォルダ内でcmake ..
# 変数を宣言し、値を入れる。
set(WIIMOTE_HEADER /usr/local/include/libcwiimote-0.4.0/libcwiimote/)
set(SORYUTION_NAME client)
set(CLIENT_CODE
    client_main
    client_input
    client_KeybordInput
    client_WiiInput
    net
    graphic
    graphic_objmesh
    image
    graphic_2dui
    client_system
    client_move
    client_map
    client_time
    control_setup
    client_name
    client_gamestate
    client_smart
    client_go
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

#ヘッダファイルのパスを指定
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2ttf_INCLUDE_DIRS} ${SDL2image_INCLUDE_DIRS} ${WIIMOTE_HEADER})

# 実行ファイル名とソース指定
add_executable( ${SORYUTION_NAME} ${CLIENT_CODE})

#コンパイルオプション
target_compile_options(${SORYUTION_NAME} PUBLIC
    $<$<CONFIG:Release>:-O3>             # Release
    $<$<CONFIG:Debug>:-O0 -g>            # Debug
    $<$<CONFIG:RelWithDebgInfo>:-O3 -g>  # RelWithDebInfo
)

target_compile_definitions(${SORYUTION_NAME} PUBLIC
    $<$<CONFIG:Debug>:DEBUG>     # Debug時
)

#リンク先のライブラリを指定
target_link_libraries( ${SORYUTION_NAME} ${SDL2_LIBRARIES}  ${SDL2ttf_LIBRARIES} ${SDL2image_LIBRARIES} -lGL -lGLU -lcwiimote -lbluetooth -L/usr/local/lib -lm)
