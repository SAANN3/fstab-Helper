cmake_minimum_required (VERSION 3.14)
project(fstabWrapper LANGUAGES CXX VERSION 1.0.0)
find_package(Threads REQUIRED)
include(FetchContent)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v5.0.0
)

FetchContent_Declare(ftxui-grid-container
  GIT_REPOSITORY https://github.com/mingsheng13/grid-container-ftxui
  GIT_TAG master
)

FetchContent_MakeAvailable(ftxui)
FetchContent_MakeAvailable(ftxui-grid-container)

add_executable(fstabWrapper 
 src/main.cpp 
 src/DeviceClass.cpp
 src/DeviceClass.h
 src/Controller.h
 src/Controller.cpp	
 src/Ui.h
 src/Ui.cpp
)
target_link_libraries(fstabWrapper
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
  PRIVATE blkid
  PRIVATE ftxui-grid-container
)
