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
			rangeScan2Dinit(2,133,190*M_PI/180.0, (190.0/133)*(M_PI/180.0),0.3,20);
			break;
		case HOKUYO_UTM30LX:
			rangeScan2Dinit(3,1081,270*M_PI/180.0, (270.0/1081.0)*(M_PI/180.0),0.3,20);
			break;
		case HOKUYO_UTM30LX_180DEG:
			rangeScan2Dinit(2,720,M_PI,(180.0/720.0)*(M_PI/180.0),0.3,20);
			break;
		case HOKUYO_UTM30LX_180DEG_SKIP1:
			rangeScan2Dinit(2,360,M_PI,(180.0/360.0)*(M_PI/180.0),0.3,20);
			break;
		case HOKUYO_UTM30LX_180DEG_SKIP2:
			rangeScan2Dinit(2,240,M_PI,(180.0/240.0)*(M_PI/180.0),0.3,20);
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

void CrangeScan2D::loadModel(const int modelID)
{
	unsigned int ii=0; 
	for(ii=0; ii<numSectors; ii++)//delete all sectors
	{
		sectors[ii]->loadModel(modelID);
	}
}

// void CrangeScan2D::loadModel(const string & modelFile)
// {
//    unsigned int ii=0; 
//    for(ii=0; ii<numSectors; ii++)//delete all sectors
//    {
//          sectors[ii]->loadModel(modelFile);
//    }
// }

void CrangeScan2D::computeScan(Cpose3d & ss, vector<float> & scan)
{
	unsigned int ii=0;
	Cpose3d ssp; //pose of the sector center
	
	//sets ssp to the center 
	ssp.setPose(ss);
	
	//Preallocate memory for scan array
	scan.reserve(numPoints);

      //initialize ssp to a first position. In case of one sector, ssp is not modified.
	if (numSectors>1)
	{
		//ssp.turnZaxis(aperture/2.0 - apertNarrow/2.0 );
		ssp.rt.turnHeading(aperture/2.0 - apertNarrow/2.0 );
	}
	
	//compute scans for the narrow sectors
	for (ii=0;ii<numSectors-1;ii++)
	{
		sectors[ii]->rangeScan(ssp,scan);
		//ssp.turnZaxis(-apertNarrow);//moves position for the next sector
            ssp.rt.turnHeading(-apertNarrow);//moves position for the next sector
	}
	
	//compute scan for the last sector (the wide one)
	sectors[ii]->rangeScan(ssp,scan);
}
