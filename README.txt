
FaRaMoTics: Fast Ranging Models for roboTics

AUTHORS
Main Author: 
	Andreu Corominas Murtra, andreu (at) beta-robots.com
Collaborators: 
	Eduard Trulls: Campus 3D model edition
	Joan Vallvé: Bug fixing and CdynamicSceneRender::drawCorners()

VERSION
2.0

LICENSE
The current license is BSD 

OVERVIEW
FaRaMoTics is a software library dedicated to fast computation of range measurement models in 3D environments. It is specially suited to compute expected range measurements in filtering or optimization applications where 2D/3D laser scanners or depth cameras are involved. It has ready-to-use functions to get expected range data for most of popular devices, such as:
      - Hokuyo UTM30LX
      - Hokuyo URGX
      - Leuze RS4
      - Kinect
      - SwissRange SR4000

APPROACH
The implementation renders 3D models, by means of OpenGL calls, viewed from the device position in optimally-sized windows, which allows fast rendering and depth retrieval. Optimal size of these windows is based on sensor specifications such as angular accuracy, angular aperture and min/max ranges. See [1] for further details.      
      
INPUT DATA 
To run FaRaMoTics, the required inputs are:
- A 3D environment or object model, described iby one of the formats allowed by assimp lib (assimp.sourceforge.net/lib_html/index.html)
- To create the syntehtic range sensor it is required (2D scanners only require horizontal components):
      - angular apertures (horizontal and vertical) [degrees]
      - number of range points (horizontal and vertical) [#]
      - angular resolution (horizontal and vertical) [degrees]
      - Minimum and maximum range [meters]
- A pose in 3D (6DoF), provided in run-time
      
OPERATION SYSTEM
Tested and developed under Ubuntu 12.04 and Ubuntu 14.04. Not tested under other OS's.

OPTIMAL HARDWARE
Performant graphics card improves speed, since most of computations are direct calls to openGL

DEPENDENCIES
The following packages/libraries are required to be installed on the system: 
      - g++, cmake (pkg)
      - OPenGL: Mesa implementation of OpenGL2.1 specification (libgl1-mesa-dev pkg)
      - GLUT: freeglut (freeglut3-dev pkg)
      - assimp3.0 (model importer, assimp.sourceforge.net)
      - dlib (algebra, header only, dlib.net)
      - pose_state_time library (github.com/beta-robots/btr-libs/wiki/pose_state_time)
      
HOW TO INSTALL
 $ mkdir build
 $ cd build
 $ cmake ..
 $ make
 $ sudo make install
 
KNOWN ISSUES
1) Assimp version conflict:

  IMPORTANT UPDATE: ROS-Indigo Version has updated its dependency to assimp3.0! Great!! Forget this point 1) if you are using ROS-Indigo. Otherwise read it!

  Some users (specially those using ROS-Hydro) have already installed the assimp library in some 2.x version, which causes conflicts when compiling FaRaMoTics. This old version is usually installed at /usr/lib and headers at /usr/include. 
  However, when downloading and installing manually assimp3 lib version (assimp.sourceforge.net), installation of library is by default at /usr/local/lib and headers at /usr/local/include. 
   
  Current Solution: 
    To assure that FaRaMoTics is compiled and linked with version 3 of assimp library, CMakeLists.txt includes the following line:
         FIND_LIBRARY(Assimp_LIBRARIES assimp HINTS /usr/local/lib NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH) 
   
  This situation could be fixed by one of the following actions:
    - ROS updates its assimp usage to version 3.0 (this will be great!) -> DONE!
    - Assimp lib provide a Find*.cmake file to precisely locate the installed library and headers (this will be also great!)
    - FaRaMoTics finds some clever CMake configuration that fixes this situation (still thinking ...)    
    
2) Window::fullScreen() does not work properly. This is a minor issue, just required for full screen visualization.

RELEASE NOTE
This release version has been completely updated, specially in two features: 
      - Use of pose_state_time library for pose3d operations.
      - Use of assimp library for 3d model import.
Moreover, some minor bugs have been fixed and the whole code has been reorganized.

HISTORY
The first version (not publicited) was developed at IRI (www.iri.upc.edu) by the same author, as a research library for real time map-based particle filter localization. This project [2] used "faramotics inside", where hundreds of thousdands rays per second where computed in the correction step of the particle filter in charge of computing 3D localization estimate. 

REFERENCES
[1]
@INPROCEEDINGS{corominas_simpar10,
  AUTHOR = {{Corominas Murtra}, A. and Trulls, E. and {Mirats Tur}, J.M. and Sanfeliu, A.},
  TITLE = {{Efficient Use of 3D Environment Models for Mobile Robot Simulation and Localization}},
  BOOKTITLE = Proceedings of the Simulation, Modelling and Programming for Autonomous Robots (SIMPAR).,
  ADDRESS = Lecture Notes on Artificial Intelligence, Num. 6472. Springer ed. ISBN 978-3-642-17318-9. Darmstadt, Germany. November, 2010.,
  PAGES={461-472},
}
[2]
@article {trulls11,
author = {Trulls, E. and Corominas Murtra, A. and P\'{e}rez-Ibarz, J. and Ferrer, G. and Vasquez, D. and Mirats-Tur, Josep M. and Sanfeliu, A.},
title = {Autonomous navigation for mobile service robots in urban pedestrian environments},
journal = {Journal of Field Robotics},
volume = {28},
number = {3},
publisher = {Wiley Subscription Services, Inc., A Wiley Company},
issn = {1556-4967},
url = {http://dx.doi.org/10.1002/rob.20386},
doi = {10.1002/rob.20386},
pages = {329-354},
year = {2011},
}

