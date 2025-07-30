cmake_minimum_required(VERSION 3.16.3)
project(Project_3)

set(CMAKE_CXX_STANDARD 14)

if(APPLE)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libraries/bridges/include)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libraries/bridges/lib)

    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libraries/curl/include)

    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libraries/SFML-2.6.0-macOS-clang-arm64/include)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libraries/SFML-2.6.0-macOS-clang-arm64/lib)

    add_executable(Project_3 main.cpp
            gui.cpp
            gui.h
            routePlanner.cpp
            routePlanner.h)
    target_link_libraries(Project_3 PRIVATE curl sfml-system sfml-window sfml-graphics)
endif()




if(WIN32)
    include_directories({CMAKE_CURRENT_SOURCE_DIR}/../libraries/bridges/include)
    link_directories({CMAKE_CURRENT_SOURCE_DIR}/../libraries/bridges/lib)

    include_directories({CMAKE_CURRENT_SOURCE_DIR}/../libraries/curl/include)
    link_directories({CMAKE_CURRENT_SOURCE_DIR}/../libraries/curl/lib)

    set(SFML_DIR "C:/SFML-2.5.1/lib/cmake/SFML")
    find_package(SFML 2.5.1 COMPONENTS system window graphics audio REQUIRED)

    add_executable(Project_3 main.cpp
            gui.cpp
            gui.h
            routePlanner.cpp
            routePlanner.h)
    target_link_libraries(Project_3 libcurl.lib sfml-system sfml-window sfml-graphics sfml-audio)
endif()