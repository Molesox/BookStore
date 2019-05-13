cmake_minimum_required(VERSION 3.13)
project(BookStore)

set(CMAKE_CXX_STANDARD 14)

add_executable(BookStore main.cpp Utilities/Book.cpp Utilities/Book.h Utilities/Library.cpp Utilities/Library.h Customer.cpp Customer.h John.cpp John.h Utilities/Shelf.cpp Utilities/Shelf.h Utilities/Semaphore.h)