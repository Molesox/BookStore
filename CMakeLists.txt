cmake_minimum_required(VERSION 3.13)
project(BookStore)

set(CMAKE_CXX_STANDARD 14)

add_executable(BookStore main.cpp Book.cpp Book.h Library.cpp Library.h Customer.cpp Customer.h John.cpp John.h Shelf.cpp Shelf.h)