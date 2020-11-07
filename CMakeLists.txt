cmake_minimum_required(VERSION 3.1.0)
project(quantumController)
add_compile_options(-std=c++11)
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  genmsg
  qt_build
  roscpp
  roslib
  image_transport
  cv_bridge
)
find_package(Qt4 REQUIRED COMPONENTS
  QtCore
  QtGui
)
include(${QT_USE_FILE})

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${QT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}

)
file(GLOB QT_FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ui/*.ui)
set(QT_MOC  include/quantumController/mainwindow.h include/customplot/qcustomplot.h)
SET(QT_SOURCES	src/mainwindow.cpp src/qcustomplot.cpp)

QT4_WRAP_UI(QT_FORMS_HPP ${QT_FORMS})
QT4_WRAP_CPP(QT_MOC_HPP ${QT_MOC} )

catkin_package(
CATKIN_DEPENDS message_runtime
)



add_library(qt_lib ${QT_SOURCES} ${QT_FORMS_HPP} ${QT_MOC_HPP})
target_link_libraries(qt_lib ${QT_LIBRARIES} ${catkin_LIBRARIES})

add_executable(gui src/main.cpp)
target_link_libraries(gui qt_lib ${catkin_LIBRARIES} )
