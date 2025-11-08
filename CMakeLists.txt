cmake_minimum_required(VERSION 3.16)
project(ChatDemo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Force static libraries for all subprojects
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries" FORCE)

# Make wxWidgets visible
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# Minimal-Middleware
include(FetchContent)
FetchContent_Declare(
  mmw
  GIT_REPOSITORY https://github.com/BeckSM64/Minimal-Middleware.git
  GIT_TAG v1.0.1
)
FetchContent_MakeAvailable(mmw)

# wxWidgets from source
set(wxBUILD_SHARED OFF CACHE BOOL "")
set(wxBUILD_SAMPLES OFF CACHE BOOL "")
set(wxBUILD_TESTS OFF CACHE BOOL "")
set(wxBUILD_EXAMPLES OFF CACHE BOOL "")
set(wxBUILD_PRECOMP OFF CACHE BOOL "")

add_executable(chatdemo MmwChat.cpp)

# Only do this on Windows, build takes way too long
if (WIN32)
  add_subdirectory(third_party/wxWidgets EXCLUDE_FROM_ALL)

  # Link against wxWidgets and MMW
  # Using CMake target provided by wxWidgets
  target_link_libraries(chatdemo PRIVATE
      wx::base
      wx::core
      mmw::mmw
  )
else (WIN32)
  target_link_libraries(chatdemo PRIVATE mmw::mmw ${wxWidgets_LIBRARIES})
endif(WIN32)
