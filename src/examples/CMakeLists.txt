
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
