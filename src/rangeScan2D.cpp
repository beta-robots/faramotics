#include "rangeScan2D.h"

CrangeScan2D::CrangeScan2D(unsigned int numSec, unsigned int nP, float apert, float delta, float rmin, float rmax, unsigned int devId)
{
	deviceId = devId;
	rangeScan2Dinit(numSec, nP, apert, delta, rmin, rmax);
}

CrangeScan2D::CrangeScan2D(unsigned int deviceType, unsigned int devId)
{
    deviceId = devId;
    switch (deviceType)
    {
        case LEUZE_RS4:
            rangeScan2Dinit(LEUZE_RS4_SECTORS ,
                        LEUZE_RS4_NUM_POINTS,
                        LEUZE_RS4_APERTURE, 
                        LEUZE_RS4_APERTURE/(double)LEUZE_RS4_NUM_POINTS, 
                        LEUZE_RS4_RANGE_MIN, 
                        LEUZE_RS4_RANGE_MAX);
            break;
            
        case HOKUYO_UTM30LX:
            rangeScan2Dinit(HOKUYO_UTM30LX_SECTORS ,
                        HOKUYO_UTM30LX_NUM_POINTS,
                        HOKUYO_UTM30LX_APERTURE, 
                        HOKUYO_UTM30LX_APERTURE/(double)HOKUYO_UTM30LX_NUM_POINTS, 
                        HOKUYO_UTM30LX_RANGE_MIN, 
                        HOKUYO_UTM30LX_RANGE_MAX);
             break;
             
        case HOKUYO_UTM30LX_180DEG:
            rangeScan2Dinit(HOKUYO_UTM30LX_180DEG_SECTORS ,
                        HOKUYO_UTM30LX_180DEG_NUM_POINTS,
                        HOKUYO_UTM30LX_180DEG_APERTURE, 
                        HOKUYO_UTM30LX_180DEG_APERTURE/(double)HOKUYO_UTM30LX_180DEG_NUM_POINTS, 
                        HOKUYO_UTM30LX_RANGE_MIN, 
                        HOKUYO_UTM30LX_RANGE_MAX);
            break;
            
        case HOKUYO_UTM30LX_180DEG_SKIP2:
            rangeScan2Dinit(HOKUYO_UTM30LX_180DEG_SECTORS ,
                        HOKUYO_UTM30LX_180DEG_SKIP2_NUM_POINTS,
                        HOKUYO_UTM30LX_180DEG_APERTURE, 
                        HOKUYO_UTM30LX_180DEG_APERTURE/(double)HOKUYO_UTM30LX_180DEG_SKIP2_NUM_POINTS, 
                        HOKUYO_UTM30LX_RANGE_MIN, 
                        HOKUYO_UTM30LX_RANGE_MAX);

            break;
            
        default: 
            cout << "ERROR: CrangeScan2D::CrangeScan2D(): Unknown Device to compute rangeScan2D" << endl;
            break; 
    }
}

CrangeScan2D::~CrangeScan2D()
{
	unsigned int ii=0; 
	for(ii=0; ii<numSectors; ii++)//delete all sectors
	{
		delete sectors[ii];
	}
}

unsigned int CrangeScan2D::getDeviceId()
{
	return deviceId;
}

unsigned int CrangeScan2D::getNumPoints()
{
	return numPoints;
}

void CrangeScan2D::rangeScan2Dinit(unsigned int numSec, unsigned int nP, float apert, float delta, float rmin, float rmax)
{
	unsigned int ii=0; 
	unsigned int nPnarrow, nPwide;
	
	numSectors = numSec;
	numPoints = nP;
	nPnarrow = floor(nP/numSectors); //numSectors-1 will be narrow.  If numSectors==1, there are no narrow sectors
	nPwide = nP-nPnarrow*(numSectors-1); //the last sector is wider. If numSectors==1, there will be just one wide sector
	aperture = apert;
	apertNarrow = nPnarrow*aperture/nP;
	apertWide = nPwide*aperture/nP;
	for(ii=0; ii<numSectors-1; ii++)//create narrow sectors. 
	{
		sectors[ii] = new CrangeSector(nPnarrow,apertNarrow,delta,rmin,rmax);
		//sectors[ii]->initWindow()
	}
	sectors[ii] = new CrangeSector(nPwide,apertWide,delta,rmin,rmax); //creates the wider sector (the last one)
}

void CrangeScan2D::loadAssimpModel(const string & modelFile)
{
      unsigned int ii=0; 
      for(ii=0; ii<numSectors; ii++)//delete all sectors
      {
            sectors[ii]->loadAssimpModel(modelFile);
      }
}

void CrangeScan2D::loadHardModel(const int modelID)
{
	unsigned int ii=0; 
	for(ii=0; ii<numSectors; ii++)//delete all sectors
	{
		sectors[ii]->loadHardModel(modelID);
	}
}

// void CrangeScan2D::computeScan(const Pose & _ss, vector<float> & scan)
// {
// 	unsigned int ii=0;
// 	Pose ssp; //pose of the sector center
// 	
// 	//sets ssp to the center 
// 	ssp.setPose(_ss);
// 	
// 	//Preallocate memory for scan array
// 	scan.reserve(numPoints);
// 
//       //initialize ssp to a first position. In case of one sector, ssp is not modified.
// 	if (numSectors>1)
// 	{
// 		//ssp.turnZaxis(aperture/2.0 - apertNarrow/2.0 );
// 		//ssp.rt.turnHeading(aperture/2.0 - apertNarrow/2.0 );
//         ssp.rotateAboutZ(aperture/2.0 - apertNarrow/2.0);
// 	}
// 	
// 	//compute scans for the narrow sectors
// 	for (ii=0;ii<numSectors-1;ii++)
// 	{
// 		sectors[ii]->rangeScan(ssp,scan);
// 		//ssp.turnZaxis(-apertNarrow);//moves position for the next sector
//         //ssp.rt.turnHeading(-apertNarrow);//moves position for the next sector
//         ssp.rotateAboutZ(-apertNarrow);
// 	}
// 	
// 	//compute scan for the last sector (the wide one)
// 	sectors[ii]->rangeScan(ssp,scan);
// }

void CrangeScan2D::computeScan(const Eigen::Transform<double,3,Eigen::Affine> & _ps, vector<double> & _scan)
{
    unsigned int ii=0;
    double angle; 
    Eigen::Quaterniond qt; 
    
    //pose of the central sector
    Eigen::Transform<double,3,Eigen::Affine> ssp(_ps); 
    
    //Preallocate memory for scan array
    _scan.reserve(numPoints);

    //in case of more than 1 sector, initialize ssp to a first position
    if (numSectors>1)
    {
        //rotate angle about Z        
            //ssp.turnZaxis(aperture/2.0 - apertNarrow/2.0 );
            //ssp.rt.turnHeading(aperture/2.0 - apertNarrow/2.0 );
            //ssp.rotateAboutZ(aperture/2.0 - apertNarrow/2.0); 
        angle = aperture/2.0 - apertNarrow/2.0;
        qt.w() = cos(angle/2.); 
        qt.x() = 0.;
        qt.y() = 0.;
        qt.z() = sin(angle/2.);
        ssp = ssp*qt;         
    }
    
    //compute scans for the narrow sectors
    for (ii=0;ii<numSectors-1;ii++)
    {
        //compute the scan for sector ii
        sectors[ii]->rangeScan(ssp,_scan);
        
        //rotate angle about Z        
            //ssp.turnZaxis(-apertNarrow);//moves position for the next sector
            //ssp.rt.turnHeading(-apertNarrow);//moves position for the next sector
            //ssp.rotateAboutZ(-apertNarrow);
        angle = -apertNarrow;
        qt.w() = cos(angle/2.); 
        qt.x() = 0.;
        qt.y() = 0.;
        qt.z() = sin(angle/2.);
        ssp = ssp*qt;         
    }
    
    //compute scan for the last sector (the wide one)
    sectors[ii]->rangeScan(ssp,_scan);
}
