
//faramotics
#include "../src/sceneRender.h"
#include "../src/groundRender.h"
#include "../src/rangeSector.h"
#include "../src/rangeImage.h"
#include "../src/rangeScan2D.h"

using namespace std;

int main(int argc, char** argv)
{
	unsigned int ii;
	CsceneRender *myRender;
	CgroundRender *myGround1;
	CrangeSector *mySector1;
	CrangeImage *myImage1;
	CrangeScan2D *myScanner; 
	Pose pose;
	vector<float> myScan;
	vector<float> myDepthI;
	
	glutInit(&argc, argv);//glut initialization
		
	myRender = new CsceneRender(500,500,70,70,0.2,100);
	myRender->loadModel(SPHERE);
	myRender->setViewPoint(-2,0,0,0,0,0);
	//myRender->fullScreen();
	myRender->render();
	//sleep(2);
/*	myRender->hide();
	sleep(2);
	myRender->show();
	sleep(2);
	for (ii = 0; ii<20; ii++)//animation
	{
		//myRender->setViewPoint(2,5,1,0+ii*0.1,0,0);
		myRender->setViewPoint(0,0,0,0+ii*0.1,0,0);
		myRender->render();
		sleep(1);
	}*/
	delete myRender;

// 	myGround1 = new CgroundRender();
// 	myGround1->loadModel("/home/andreu/iriSw/algorithms/faramotics/trunk/models/campusNordUPC.obj");
// 	pose.setPose(2,60,1,1.5,0,0);
// 	cout << "zz1 = " << myGround1->zConstraint(pose) << endl;
// 	cout << "pitch1 = " << myGround1->pConstraint(pose)*180.0/M_PI << endl;
// 	cout << "roll1 = " << myGround1->rConstraint(pose)*180.0/M_PI << endl;
// 	delete myGround1;

	mySector1 = new CrangeSector(67, 95*M_PI/180.0, 95*M_PI/(180.0*67), 0.3, 20);
	mySector1->loadModel("/home/andreu/iriSw/algorithms/faramotics/trunk/models/campusNordUPC.obj");
	pose.setPose(20,8,0.4,1.5,0,0);
	mySector1->rangeScan(pose,myScan);
	for (ii = 0 ; ii<myScan.size(); ii++) cout << myScan.at(ii) << " ";
	cout << endl;
	delete mySector1;
	
	myScanner = new CrangeScan2D(LEUZE_RS4);
	myScanner->loadModel("/home/andreu/iriSw/algorithms/faramotics/trunk/models/campusNordUPC.obj");
	pose.setPose(7.5,7.5,0,0,0,0);
	myScan.clear();	
	myScanner->computeScan(pose,myScan);
	for (ii = 0 ; ii<myScan.size(); ii++) cout << myScan.at(ii) << " ";
	delete myScanner;

	myImage1 = new CrangeImage(176,144,43.6*M_PI/180.0, 34.6*M_PI/180.0,0.25*M_PI/180.0,0.24*M_PI/180.0,0.3,5);
	//myImage1->loadModel("/home/andreu/mySvn/faRaMoTics/models/campusNordUPC.obj");
	myImage1->loadModel(SPHERE);
	pose.setPose(-3,0,0,0,0,0);
	myImage1->depthImage(pose,myDepthI);
	for (ii = 0 ; ii<myDepthI.size(); ii++) cout << myDepthI.at(ii) << " ";
	cout << endl;
	delete myImage1;
	
	//sleep(5);

	return 0;
}
