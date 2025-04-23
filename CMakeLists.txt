cmake_minimum_required(VERSION 3.28.3)
project(Cours_Project_SetiGame C)

set(CMAKE_C_STANDARD 11)

add_executable(Cours_Project_SetiGame server.c
        client.c)
