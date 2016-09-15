
# ADD_EXECUTABLE(sceneRender  sceneRenderMain.cpp)
# ADD_EXECUTABLE(visualizationTest visualizationTest.cpp)
ADD_EXECUTABLE(timeTest timeTestMain.cpp)
ADD_EXECUTABLE(assimpTest assimpTestMain.cpp)

# TARGET_LINK_LIBRARIES(sceneRender faramotics)
# TARGET_LINK_LIBRARIES(visualizationTest faramotics)
TARGET_LINK_LIBRARIES(timeTest faramotics)
TARGET_LINK_LIBRARIES(assimpTest assimp)
