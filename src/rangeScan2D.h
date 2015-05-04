
#ifndef rangeScan2D_H
#define rangeScan2D_H

//include faramotics
#include "rangeSector.h"
#include "deviceConsts.h"

/**
* \brief CrangeScan2D class
* 
* This class implements a 2D ranging devices with an aperture that can be greater than PI. 
* From an initial set of device parameters, the class determines the number of sectors
* and the parameters of each one. The full scan is computed as the concatenation of each partial scan from each sector.
* The function loadModel() allows to load the 3D model used to compute range scans.
* The function computeScan() implements the full computation of the entire range scan of the device being simulated.
* So, the required input parameters that describe a 2D range device are:
*	- numPoints: number of range points [#]
*	- aperture: horizontal aperture [rad]
*	- angularAccuracy: angular accuracy of the rays [rad]
*	- rangeMin: minimum range [m]
*	- rangeMax: maximum range [m]
* It is also required to specify a 3D model, either as a .obj file or as an ID. 
*/
class CrangeScan2D
{
	protected:
		/**
		* \brief Device Id
		* 
		* device Id, [#]. Useful when managing several rangeScan2D objects.
		*
		*/
		unsigned int deviceId;
	
		/**
		* \brief Number of range points
		* 
		* Number of range points, [#]
		*
		*/
		unsigned int numPoints;

		/**
		* \brief Angular aperture of the 2D range scanner
		* 
		* Angular aperture of the scan device, [rad]
		*
		*/
		float aperture; 
				
		/**
		* \brief Angular accuracy of the 2D range scanner
		* 
		* Angular accuracy the scan device, [rad]
		*
		*/
		float angularAccuracy;
		
		/**
		* \brief Minimum range
		* 
		* Minimum range of the 2D range scanner, [m]
		*
		*/
		float rangeMin;
		
		/**
		* \brief Maximum range
		* 
		* Maximum range of the 2D range scanner, [m]
		*
		*/
		float rangeMax;
		
		/**
		* \brief Number of sectors
		* 
		* Number of sectors to divide the device aperture. 
		* The full scan is computed as the concatenation of each partial scan from each sector.
		*
		*/
		unsigned int numSectors;
		
		/**
		* \brief Vector of sectors
		* 
		* Vector of sectors
		*
		*/		
		CrangeSector *sectors[MAX_SECTORS];
		
		/**
		* \brief Horizontal angular aperture for the narrow sectors
		* 
		* Horizontal angular aperture for the narrow sectors, [rad]
		* If only one sector exists, there will not be narrow sectors.
		*
		*/		
		float apertNarrow;
		
		/**
		* \brief Horizontal angular aperture for the wide sector
		* 
		* Horizontal angular aperture for the wide sector, [rad]
		* The last sector is always wide. 
		* If only one sector exists, it will be wide.
		*
		*/		
		float apertWide;
		
		/**
		* \brief Initialization
		* 
		* Initialization. Called only by constructors.
		* From the device parameters creates each sector and sets their parameters
		* 
		*/		
		void rangeScan2Dinit(unsigned int numSec, unsigned int nP, float apert, float delta, float rmin, float rmax);		
		
	public:
		/**
		* \brief Constructor with parameters
		* 
		* Constructor with device parameters:
		* 	- numSec: number of sectors in which the scan is divided [#]
		*	- nP: number of range points [#]
		*	- apert: horizontal aperture [rad]
		*	- delta: angular accuracy of the rays [rad]
		*	- rmin: minimum range [m]
		*	- rmax: maximum range [m]		
		*	- devId: device Id. Useful when managing some rangeScan2D objects. By default = 0.
		*
		*/		
		CrangeScan2D(unsigned int numSec, unsigned int nP, float apert, float delta, float rmin, float rmax, unsigned int devId = 0);

		/**
		* \brief Constructor with a known device type
		* 
		* Constructor with a known device type
		*
		*/		
		CrangeScan2D(unsigned int deviceType, unsigned int devId = 0);

		/**
		* \brief Destructor
		* 
		* Destructor
		*
		*/		
		~CrangeScan2D();

		/** \brief Returns deviceId
		* 
		* Returns deviceId
		*
		*/		
		unsigned int getDeviceId();
		
		/** \brief Returns numPoints
		* 
		* Returns numPoints
		*
		*/		
		unsigned int getNumPoints();
	
            /** \brief Loads 3D model by file
            * 
            * Loads the given 3D model to each sector
            *
            */          
            void loadAssimpModel(const string & modelFile);
            
		/** \brief Loads 3D model by ID
		* 
		* Loads the indicated 3D model to each sector
		*
		*/		
		void loadHardModel(const int modelID);
            
		/** \brief Computes a full 2D range scan.
		* 
		* Computes a full 2D range scan from position ss. Range results are output to scan vector.
		*
		*/		
		void computeScan(Cpose3d & ss, vector<float> & scan); /**<sets the scan vector with expected ranges computed from ss against the 3d model*/
        
        /** \brief Computes a full 2D range scan. Cast to double
        * 
        * Computes a full 2D range scan from position ss. Range results are output to scan vector.
        * Cast to double
        * TODO: Update of all faramotics to work with doubles
        *
        */              
        void computeScan(Cpose3d & ss, vector<double> & scan); /**<sets the scan vector with expected ranges computed from ss against the 3d model*/
};
#endif
