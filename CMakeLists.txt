cmake_minimum_required(VERSION 3.5)

project(Examples_14)

find_package(Threads)



add_executable(tlc tlc.cpp)
set_target_properties(tlc PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)
target_link_libraries(tlc ${CMAKE_THREAD_LIBS_INIT})

add_executable(stack stack.cpp)
set_target_properties(stack PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)
target_link_libraries(stack ${CMAKE_THREAD_LIBS_INIT})

add_executable(dead_lock dead_lock.cpp)
set_target_properties(dead_lock PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)
target_link_libraries(dead_lock ${CMAKE_THREAD_LIBS_INIT})

add_executable(pass_out pass_out.cpp)
set_target_properties(pass_out PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)
target_link_libraries(pass_out ${CMAKE_THREAD_LIBS_INIT})

add_executable(exception exception.cpp)
set_target_properties(exception PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)
target_link_libraries(exception ${CMAKE_THREAD_LIBS_INIT})

add_executable(async_1 async_1.cpp)
set_target_properties(async_1 PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)
target_link_libraries(async_1 ${CMAKE_THREAD_LIBS_INIT})

add_executable(promise promise.cpp)
set_target_properties(promise PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)
target_link_libraries(promise ${CMAKE_THREAD_LIBS_INIT})

add_executable(async_2 async_2.cpp)
set_target_properties(async_2 PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
target_link_libraries(async_2 ${CMAKE_THREAD_LIBS_INIT})

add_executable(conditional_1 conditional_1.cpp)
set_target_properties(conditional_1 PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)
target_link_libraries(conditional_1 ${CMAKE_THREAD_LIBS_INIT})

add_executable(multi_producer_consumer multi_producer_consumer.cpp)
set_target_properties(multi_producer_consumer PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)
target_link_libraries(multi_producer_consumer ${CMAKE_THREAD_LIBS_INIT})

add_executable(asynchron_1 asynchron_1.cpp)
set_target_properties(asynchron_1 PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)
target_link_libraries(asynchron_1 ${CMAKE_THREAD_LIBS_INIT})

add_executable(asynchron_2 asynchron_2.cpp)
set_target_properties(asynchron_2 PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)
target_link_libraries(asynchron_2 ${CMAKE_THREAD_LIBS_INIT})