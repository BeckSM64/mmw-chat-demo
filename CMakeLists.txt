cmake_minimum_required(VERSION 3.16)
project(ChatDemo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  mmw
  GIT_REPOSITORY https://github.com/BeckSM64/Minimal-Middleware.git
  GIT_TAG v1.0.1
)
FetchContent_MakeAvailable(mmw)

find_package(wxWidgets REQUIRED COMPONENTS core base)

include(${wxWidgets_USE_FILE})

add_executable(chatdemo MmwChat.cpp)
target_link_libraries(chatdemo PRIVATE ${wxWidgets_LIBRARIES} mmw::mmw)

