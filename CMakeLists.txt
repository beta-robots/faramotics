# minimum cmake version
cmake_minimum_required(VERSION 2.8)

# The project name and the type of project
PROJECT(faramotics)

#Set bin folder to place executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

#Set lib folder to place library
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)

# Set INSTALL folder
SET(CMAKE_INSTALL_PREFIX /usr/local)

# Build type
IF (NOT CMAKE_BUILD_TYPE)
 SET(CMAKE_BUILD_TYPE "RELEASE") 
ENDIF (NOT CMAKE_BUILD_TYPE)

# compliler flags
SET(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -D_REENTRANT")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -D_REENTRANT")

#Set compiler according C++11 support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has C++11 support.")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has C++0x support.")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

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
#     pose.cpp
    src/window.cpp
    src/sceneRender.cpp
#     dynamicSceneRender.cpp
    src/rangeSector.cpp
    src/rangeScan2D.cpp      
    src/rangeImage.cpp
)

#faramotics headers
SET(HEADERS 
#     pose.h
    src/window.h
    src/sceneRender.h
#     dynamicSceneRender.h
    src/rangeSector.h
    src/rangeScan2D.h      
    src/rangeImage.h
    src/deviceConsts.h
)

# create the shared library
ADD_LIBRARY(${PROJECT_NAME} SHARED ${SRCS}) 
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${Assimp_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_glut_LIBRARY})

#install library
INSTALL(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib/faramotics
        ARCHIVE DESTINATION lib/faramotics)

# install headers
INSTALL(FILES ${HEADERS} DESTINATION include/faramotics)

#install Findfaramotics.cmake
INSTALL(FILES Findfaramotics.cmake DESTINATION ${CMAKE_ROOT}/Modules/)

# carry on building the examples  
ADD_SUBDIRECTORY(src/examples)
