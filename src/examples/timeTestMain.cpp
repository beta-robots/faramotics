
//std
#include <time.h>
#include <sys/time.h>

//faramotics
#include "../src/rangeScan2D.h"
#include "../src/rangeImage.h"

using namespace std;

int main(int argc, char** argv)
{
	unsigned int ii;
	CrangeScan2D *myLaserScanner; 
	CrangeImage *myDepthCamera;
	Pose pose;
	vector<float> myScan;
	vector<double> myImage;
	timeval t1,t2;
	double dTscan = 0;
	double dTimage = 0;
	unsigned int nTrials = 100;
	
	//glut initialization
	glutInit(&argc, argv);
	
    //set devices
    //myLaserScanner = new CrangeScan2D(LEUZE_RS4);
    myLaserScanner = new CrangeScan2D(HOKUYO_UTM30LX);
	//myDepthCamera = new CrangeImage(SR4000);
	myDepthCamera = new CrangeImage(KINECT);
	
	//load 3D models
	//myLaserScanner->loadHardModel("../models/campusNordUPC.obj");
    myLaserScanner->loadHardModel(SPHERE);
	//myDepthCamera->loadHardModel("../models/campusNordUPC.obj");
	myDepthCamera->loadHardModel(SPHERE);
      
	//main loop 
	for (int jj=0; jj<5; jj++)
	{
		dTscan = 0;
		dTimage = 0;
		for (ii = 0 ; ii<nTrials; ii++)
		{
            pose.setPose(1.0+ii*1e-2, 1.0-ii*1e-2, 1.0, 30*M_PI/180., 0.0, 0.0);//just to modify a little bit the view point
            myScan.clear(); //clear vector results
            myImage.clear(); //clear vector results

            //laser scan
            gettimeofday(&t1, NULL); 
            myLaserScanner->computeScan(pose,myScan);
            gettimeofday(&t2, NULL); 
            dTscan += (double) ( (t2.tv_sec + t2.tv_usec/1e6) - (t1.tv_sec + t1.tv_usec/1e6) );

            //depth image
            gettimeofday(&t1, NULL); 
            myDepthCamera->depthImage(pose,myImage);
            gettimeofday(&t2, NULL); 
            dTimage += (double) ( (t2.tv_sec + t2.tv_usec/1e6) - (t1.tv_sec + t1.tv_usec/1e6) );				
		}
		
		cout << "dTscan = " << (dTscan/nTrials)*1000 << " ms" << endl;
		cout << "dTimage = " << (dTimage/nTrials)*1000 << " ms" << endl;
		cout << endl;
	}
	
	//delete objects
	delete myLaserScanner;
	delete myDepthCamera;
	
	return 0;
}
