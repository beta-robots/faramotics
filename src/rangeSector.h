
#ifndef rangeSector_H
#define rangeSector_H

//faramotics
#include "sceneRender.h"

//std
#include <vector>

/**
* \brief Vertical aperture for 2D sectors
* 
* Vertical aperture for 2D sectors
*
*/
static const float SECTOR_VERT_APERTURE = 0.2*M_PI/180.0;

/**
* \brief Vertical number of points for 2D sectors
* 
* Vertical number of points for 2D sectors
*
*/
static const float SECTOR_VERT_POINTS = 3; /**<number of vertical points */

/**
 * \brief CrangeSector class
 * 
 * This class implements a generic 2D rangefinder device with an aperture lower than PI.
 * The class inherits sceneRender and incorporates the rangeScan() function that computes a range scan given a viewPoint and a set of sector parameters.
 *
*/
class CrangeSector : public CsceneRender
{
	protected:
		/**
		* \brief Horizontal number of points
		* 
		* Horizontal number of points
		*
		*/
		unsigned int numPointsH;
		
		/**
		* \brief Vertical number of points
		* 
		* Vertical number of points
		*
		*/
		unsigned int numPointsV;
		
		/**
		* \brief Horizontal angular accuracy
		* 
		* Horizontal angular accuracy, [rad].
		*
		*/
		float angularAccuracyH;
		
		/**
		* \brief Vertical angular accuracy
		* 
		* Vertical angular accuracy, [rad].
		*
		*/
		float angularAccuracyV; 		
		
		/**
		* \brief Precomputed indexes
		* 
		* Precomputed indexes for the renderized pixels of interest.
		*
		*/
		vector<int> kH;
		
		/**
		* \brief Precomputed values
		* 
		* precomputed values for the cos(alpha_i)
		*
		*/
		vector<float> cosHi; 
		
	public:
		/**
		* \brief Default constructor
		* 
		* Default constructor with the following parameters:
		*	- numP: number of range points, [#]
		*	- apert: horizontal aperture, [rad]
		*	- angularAcc: angular accuracy of the rays, [rad]
		*	- rmin: minimum range, [m]
		*	- rmax: maximum range, [m]
		* 
		* This constructor computes the window rendering paramers (size and projection) according the device parameters provided.
		* 
		*/
		CrangeSector(unsigned int numP, float apert, float angularAcc, float rmin, float rmax);
		
		/**
		* \brief Destructor
		* 
		* Destructor
		*
		*/		
		~CrangeSector();

		/**
		* \brief Computes a range scan
		* 
		* Computes the vector of ranges from the position ss, over the entire aperture of this sector
		*
		*/
		void rangeScan(const Pose & _ss, vector<float> & _scan); /**<sets the scan vector with expected ranges computed from ss against the 3d model*/
        
        /**
        * \brief Computes a range scan. Cast to double
        * 
        * Computes the vector of ranges from the position ss, over the entire aperture of this sector
        * Cast to double
        * TODO: Update of all faramotics to work with doubles
        */
        void rangeScan(Cpose3d & ss, vector<double> & scan); /**<sets the scan vector with expected ranges computed from ss against the 3d model*/

};
#endif
