cmake_minimum_required(VERSION 3.5 FATAL_ERROR)


project(pcd_read)


find_package(PCL 1.2 REQUIRED COMPONENTS common io visualization filters)
find_package(VTK REQUIRED)

add_executable (pcd_read read_pcd.cpp)

target_link_libraries(pcd_read ${PCL_LIBRARIES} ${VTK_LIBRARIES})
