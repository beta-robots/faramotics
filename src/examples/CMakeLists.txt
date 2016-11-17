
ADD_EXECUTABLE(simple simple.cpp)
# ADD_EXECUTABLE(sceneRender  sceneRenderMain.cpp)
# ADD_EXECUTABLE(visualizationTest visualizationTest.cpp)
# ADD_EXECUTABLE(timeTest timeTestMain.cpp)
ADD_EXECUTABLE(assimpTest assimpTestMain.cpp)
ADD_EXECUTABLE(visualization_test_gripper visualization_test_gripper.cpp)

TARGET_LINK_LIBRARIES(simple faramotics)
# TARGET_LINK_LIBRARIES(sceneRender faramotics)
# TARGET_LINK_LIBRARIES(visualizationTest faramotics)
# TARGET_LINK_LIBRARIES(timeTest faramotics)
TARGET_LINK_LIBRARIES(assimpTest assimp)
TARGET_LINK_LIBRARIES(visualization_test_gripper faramotics)

# TODO: put the following inside an IF conditioned to finding pcl lib
# Find point cloud Library
FIND_PACKAGE(PCL 1.3 REQUIRED COMPONENTS)
INCLUDE_DIRECTORIES(${PCL_INCLUDE_DIRS})
LINK_DIRECTORIES(${PCL_LIBRARY_DIRS})
ADD_DEFINITIONS(${PCL_DEFINITIONS})
ADD_EXECUTABLE(point_cloud point_cloud.cpp)
TARGET_LINK_LIBRARIES(point_cloud faramotics ${PCL_COMMON_LIBRARIES} ${PCL_IO_LIBRARIES} ${PCL_VISUALIZATION_LIBRARIES})

