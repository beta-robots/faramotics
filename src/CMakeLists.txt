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

# GLM lib srcs
# SET(GLM_SRCS
# 	glm_src/glm.c
# 	glm_src/glmimg.c
# 	glm_src/glmimg_devil.c
# 	glm_src/glmimg_jpg.c
# 	glm_src/glmimg_sdl.c
# 	glm_src/glmimg_sim.c
# 	glm_src/glm_util.c
# )

# SET(GLM_HEADERS
# 	glm_src/glm.h
# 	glm_src/glmint.h
# )

#faramotics srcs
SET(SRCS
	sceneRender.cpp
	dynamicSceneRender.cpp
      rangeSector.cpp
      rangeScan2D.cpp      
	rangeImage.cpp
)

SET(HEADERS 
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
        LIBRARY DESTINATION lib/btr-libs
        ARCHIVE DESTINATION lib/btr-libs)

INSTALL(FILES ${HEADERS} DESTINATION include/btr-headers)
# INSTALL(FILES ${GLM_HEADERS} DESTINATION include/btr-headers/glm_src)

INSTALL(FILES ../Findfaramotics.cmake DESTINATION ${CMAKE_ROOT}/Modules/)

ADD_SUBDIRECTORY(examples)
