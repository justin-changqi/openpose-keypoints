cmake_minimum_required(VERSION 2.8.7)

add_definitions(-std=c++11)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

find_package(GFlags REQUIRED)
find_package(Glog REQUIRED)
find_package(OpenCV REQUIRED)
find_package(OpenPose REQUIRED)

include_directories( include ${OpenPose_INCLUDE_DIRS} ${GFLAGS_INCLUDE_DIR} ${GLOG_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS})

add_library(common src/common.cc)
target_link_libraries(common ${OpenCV_LIBS})

add_library(kp_extractor src/kp_extractor.cc)
target_link_libraries(kp_extractor common 
                                   ${OpenPose_LIBS} 
                                   ${GFLAGS_LIBRARY} 
                                   ${GLOG_LIBRARY} 
                                   ${OpenCV_LIBS})

add_executable(test_cvopenpose app/test_cvopenpose.cc)
target_link_libraries(test_cvopenpose kp_extractor )

add_executable(2_whole_body_from_image test/2_whole_body_from_image.cpp)
target_link_libraries(2_whole_body_from_image ${OpenPose_LIBS} ${GFLAGS_LIBRARY} ${GLOG_LIBRARY} ${OpenCV_LIBS})