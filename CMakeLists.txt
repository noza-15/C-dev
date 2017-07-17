cmake_minimum_required(VERSION 2.8)
project(Flappy-Cube)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c game.c hoge.c menu.c obstacle.c ranking.c selector.c huga.c missile.c speed.c background.c player.c raw_data.c score.c)

add_executable(Flappy-Cube ${SOURCE_FILES})
target_link_libraries(Flappy-Cube GL GLU glut m)