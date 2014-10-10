# minimum cmake version
cmake_minimum_required(VERSION 2.6)

#required libs
FIND_PACKAGE(pose_state_time REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLUT REQUIRED)
FIND_LIBRARY(Assimp_LIBRARIES assimp HINTS /usr/local/lib NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH) 

#required directories to find includes
INCLUDE_DIRECTORIES(.)
INCLUDE_DIRECTORIES(${pose_state_time_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR})
# INCLUDE_DIRECTORIES(${Assimp_SOURCE_DIR}/include ${Assimp_SOURCE_DIR}/code)

#faramotics srcs
SET(SRCS
    window.cpp
    sceneRender.cpp
    dynamicSceneRender.cpp
    rangeSector.cpp
    rangeScan2D.cpp      
    rangeImage.cpp
)

#faramotics headers
SET(HEADERS 
    window.h
    sceneRender.h
    dynamicSceneRender.h
    rangeSector.h
    rangeScan2D.h      
    rangeImage.h
)

# create the shared library
ADD_LIBRARY(faramotics SHARED ${SRCS}) 
TARGET_LINK_LIBRARIES(faramotics ${Assimp_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_glut_LIBRARY} ${pose_state_time_LIBRARIES})

#install 
INSTALL(TARGETS faramotics
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib/faramotics
        ARCHIVE DESTINATION lib/faramotics)

INSTALL(FILES ${HEADERS} DESTINATION include/faramotics)

INSTALL(FILES ../Findfaramotics.cmake DESTINATION ${CMAKE_ROOT}/Modules/)

ADD_SUBDIRECTORY(examples)
