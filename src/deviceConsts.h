#ifndef deviceConsts_H
#define deviceConsts_H

// Known devices id's for laser scanners
enum knownScannerDevices {
        LEUZE_RS4 = 1, 
        HOKUYO_UTM30LX, 
        HOKUYO_UTM30LX_180DEG, 
        HOKUYO_UTM30LX_180DEG_SKIP2};
        
// Known devices id's for depth images
enum knownDepthCameraDevices {
        SR4000 = 1, 
        KINECT, 
        TOF_X};
        
// Maximum allowed sectors
static const unsigned int MAX_SECTORS=6;
        
//LEUZE_RS4 params        
const unsigned int LEUZE_RS4_SECTORS = 2;
const unsigned int LEUZE_RS4_NUM_POINTS = 133;
const double LEUZE_RS4_APERTURE = 190.*M_PI/180.;
const double LEUZE_RS4_RANGE_MIN = 0.3;
const double LEUZE_RS4_RANGE_MAX = 20;

//HOKUYO_UTM30LX params
const unsigned int HOKUYO_UTM30LX_SECTORS = 3;
const unsigned int HOKUYO_UTM30LX_NUM_POINTS = 1081;
const double HOKUYO_UTM30LX_APERTURE = 270.*M_PI/180.;
const double HOKUYO_UTM30LX_RANGE_MIN = 0.3;
const double HOKUYO_UTM30LX_RANGE_MAX = 20;

//HOKUYO_UTM30LX_180DEG params
const unsigned int HOKUYO_UTM30LX_180DEG_SECTORS = 2;
const unsigned int HOKUYO_UTM30LX_180DEG_NUM_POINTS = 720;
const double HOKUYO_UTM30LX_180DEG_APERTURE = 180.*M_PI/180.;

//HOKUYO_UTM30LX_180DEG_SKIP2 params
const unsigned int HOKUYO_UTM30LX_180DEG_SKIP2_NUM_POINTS = 240;

//SR4000 params
const unsigned int SR4000_NUM_POINTS_HORIZONTAL = 176;
const unsigned int SR4000_NUM_POINTS_VERTICAL = 144;
const double SR4000_APERTURE_HORIZONTAL = 43.6*M_PI/180.;
const double SR4000_APERTURE_VERTICAL = 34.6*M_PI/180.;
const double SR4000_ANGULAR_ACCURACY_HORIZONTAL = 0.25*M_PI/180.;
const double SR4000_ANGULAR_ACCURACY_VERTICAL = 0.24*M_PI/180.;
const double SR4000_MIN_RANGE = 0.3; 
const double SR4000_MAX_RANGE = 5.; 

//KINECT params
const unsigned int KINECT_NUM_POINTS_HORIZONTAL = 640;
const unsigned int KINECT_NUM_POINTS_VERTICAL = 480;
const double KINECT_APERTURE_HORIZONTAL = 57.*M_PI/180.;
const double KINECT_APERTURE_VERTICAL = 43.*M_PI/180.;
const double KINECT_ANGULAR_ACCURACY_HORIZONTAL = (57./640.)*(M_PI/180.);
const double KINECT_ANGULAR_ACCURACY_VERTICAL = (43./640.)*(M_PI/180.);
const double KINECT_MIN_RANGE = 0.7; 
const double KINECT_MAX_RANGE = 5.; 


#endif