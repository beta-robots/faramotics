# minimum cmake version
cmake_minimum_required(VERSION 2.8)

#required libs
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLUT REQUIRED)
FIND_LIBRARY(Assimp_LIBRARIES assimp HINTS /usr/local/lib NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH) 

#required directories to find includes
INCLUDE_DIRECTORIES(.)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Assimp_SOURCE_DIR}/include ${Assimp_SOURCE_DIR}/code)

#faramotics srcs
SET(SRCS
    pose.cpp
    window.cpp
    sceneRender.cpp
    dynamicSceneRender.cpp
    rangeSector.cpp
    rangeScan2D.cpp      
    rangeImage.cpp
)

#faramotics headers
SET(HEADERS 
    pose.h
    window.h
    sceneRender.h
    dynamicSceneRender.h
    rangeSector.h
    rangeScan2D.h      
    rangeImage.h
    deviceConsts.h
)

# create the shared library
ADD_LIBRARY(${PROJECT_NAME} SHARED ${SRCS}) 
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${Assimp_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_glut_LIBRARY})

#install 
INSTALL(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib/faramotics
        ARCHIVE DESTINATION lib/faramotics)

INSTALL(FILES ${HEADERS} DESTINATION include/faramotics)

INSTALL(FILES ../Findfaramotics.cmake DESTINATION ${CMAKE_ROOT}/Modules/)

ADD_SUBDIRECTORY(examples)
