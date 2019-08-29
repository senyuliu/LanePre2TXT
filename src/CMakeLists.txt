cmake_minimum_required(VERSION 2.8.12)
#project (lanepre2txt)

####################opencv##############
find_package(OpenCV REQUIRED)

####################GDAL################
include_directories(/usr/include/gdal)

###################PCL##################
find_package(PCL REQUIRED)  

include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARIES_DIRS})
add_definitions(${PCL_DEFINITIONS})
list(REMOVE_ITEM PCL_LIBRARIES "vtkproj4")

###################COMPILE#############
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MODULE_CORE ../)
set(MODULE_CORE_INCLUDE ${MODULE_CORE}/include/)
set(MODULE_CORE_SRC ${MODULE_CORE}/src/)
set(MODULE_CORE_LIB ${MODULE_CORE}/lib/)
include_directories(${MODULE_CORE_INCLUDE})
MESSAGE("PROJECT_ROOT include dirs: ${MODULE_CORE_SRC}")
set(LANE_CORE "Lane2txt.bin")
set(EXECUTABLE_OUTPUT_PATH "../${MODULE_CORE}/bin")

add_executable(${LANE_CORE} ${MODULE_CORE_SRC}/laneprocess.cpp ${MODULE_CORE_SRC}/main.cpp)
target_link_libraries(${LANE_CORE} ${OpenCV_LIBS} gdal glog ${PCL_LIBRARIES})







