cmake_minimum_required(VERSION 3.23)
project(project)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(project
        document.cpp
        document.h
        main.cpp
        paginator.h
        read_input_functions.cpp
        read_input_functions.h
        request_queue.cpp
        request_queue.h
        search_server.cpp
        search_server.h
        string_processing.cpp
        string_processing.h remove_duplicates.cpp remove_duplicates.h test_example_functions.cpp test_example_functions.h process_queries.cpp process_queries.h)