cmake_minimum_required(VERSION 3.27)
project(gimpsep_clm_project) # You need to change the Project Name

set(CMAKE_CXX_STANDARD 20)
set(ENV{OPENCV_DIR} "C:\\tools\\opencv\\build")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRST})

add_executable(gimpsep_clm_project # You need to change the Project Name
        # main
        src/main/main.cpp
        # lib
        src/lib/cvui.h
        src/lib/EnhancedWindow.h
        src/lib/tinyfiledialogs.cpp
        src/lib/tinyfiledialogs.h
        # our class
        src/main/Image.cpp
        src/main/Image.h
        src/main/ImageApp.cpp
        src/main/ImageApp.h
        src/main/ImageHandler.cpp
        src/main/ImageHandler.h
        src/main/PanoramaCreator.cpp
        src/main/PanoramaCreator.h
)


set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)
target_link_libraries(gimpsep_clm_project ${OpenCV_LIBS}) # You need to change the Project Name

