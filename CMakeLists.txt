cmake_minimum_required(VERSION 3.16)
project(ChatDemo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries" FORCE)

# Minimal-Middleware
include(FetchContent)
FetchContent_Declare(
    mmw
    GIT_REPOSITORY https://github.com/BeckSM64/Minimal-Middleware.git
    GIT_TAG v1.0.1
)
FetchContent_MakeAvailable(mmw)

# Add executable
add_executable(chatdemo MmwChat.cpp)

# Linux
if(UNIX AND NOT APPLE)
    find_package(wxWidgets REQUIRED COMPONENTS core base)
    include(${wxWidgets_USE_FILE})
    target_link_libraries(chatdemo PRIVATE mmw::mmw ${wxWidgets_LIBRARIES})
endif()

# Windows
if(WIN32)
    add_subdirectory(third_party/wxWidgets EXCLUDE_FROM_ALL)
    target_link_libraries(chatdemo PRIVATE wx::core wx::base mmw::mmw)
endif()
