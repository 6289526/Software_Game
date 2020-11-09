#g++ -o client client_main.cpp client_input.cpp client_WiiInput.cpp net.cpp graphic.cpp client_system.cpp client_move.cpp client_map.cpp -lm -lSDL2 -lSDL2_image -lSDL2_ttf -lGL -lGLU -lcwiimote -lbluetooth -L/usr/lib -I/usr/local/include/libcwiimote-0.4.0/libcwiimote

#buildフォルダ内でcmake ..
# 変数を宣言し、値を入れる。
set(SORYUTION_NAME client)
set(WIIMOTE_HEADER /usr/local/include/libcwiimote-0.4.0/libcwiimote/)
# set(WIIMOTEAPI_HEADER /usr/local/include/libcwiimote-0.4.0/libcwiimote/wiimote_api.h)
set(CLIENT_SRC
    client_main
    client_input
    client_WiiInput
    net
    graphic
    client_system
    client_move
    client_map
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
add_executable( ${SORYUTION_NAME} ${CLIENT_SRC} )

#リンク先のライブラリを指定
target_link_libraries( ${SORYUTION_NAME} ${SDL2_LIBRARIES}  ${SDL2ttf_LIBRARIES} ${SDL2image_LIBRARIES} -lGL -lGLU -lcwiimote -lbluetooth -L/usr/local/lib -lm)
