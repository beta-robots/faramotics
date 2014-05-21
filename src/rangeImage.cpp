#include "rangeImage.h"

CrangeImage::CrangeImage(unsigned int numPH, unsigned int numPV, float apertH, float apertV, float angularAccH, float angularAccV, float rmin, float rmax) : CsceneRender()
{
	rangeImageInit(numPH, numPV, apertH, apertV, angularAccH, angularAccV, rmin, rmax);
}

CrangeImage::CrangeImage(unsigned int deviceId)
{
	switch (deviceId)
	{
		case SR4000:
			rangeImageInit(176,144,43.6*M_PI/180.0, 34.6*M_PI/180.0,0.25*M_PI/180.0,0.24*M_PI/180.0,0.3,5);
			break;
		case KINECT:
			rangeImageInit(640,480,57*M_PI/180.0, 43*M_PI/180.0,(57.0/640)*(M_PI/180.0),(43.0/640)*(M_PI/180.0),0.7,6);
			break;
		default: 
			cout << "ERROR: CrangeImage::CrangeImage(): Unknown Device to compute rangeImages" << endl;
			break; 
	}
}

CrangeImage::~CrangeImage()
{
	kH.clear();
	kV.clear();
}

void CrangeImage::rangeImageInit(unsigned int numPH, unsigned int numPV, float apertH, float apertV, float angularAccH, float angularAccV, float rmin, float rmax)
{
	unsigned int ii;
	float ai, wi;
	unsigned int wPixels, hPixels;
	
	//sets sector parameters 
	numPointsH = numPH; 
	numPointsV = numPV;
	angularAccuracyH = angularAccH;
	angularAccuracyV = angularAccV;
	wPixels = (int)(2.0*tan(apertH/2.0)/tan(angularAccuracyH));
	hPixels = (int)(2.0*tan(apertV/2.0)/tan(angularAccuracyV));
	
	//sets render parameters
	setRenderParameters(wPixels, hPixels, apertH, apertV, rmin, rmax);
	//printRenderParameters();
	
	//inits window and GL state
	initWindow();
	initGL();
	this->hide();
	
	//precomputes vectors
	for (ii=0;ii<numPointsH;ii++) //precomputes vectors (horizontal)
	{
		ai=hAperture*(0.5-(float)ii/(float)numPointsH);//angle associated to ray i
		wi=zNear*tan(ai);//distance from the hit point i to the x sensor axis
		kH.push_back( (int) ((0.5 - wi/widthM)*widthP) );//pixel index associated to ray i
	}	
	for (ii=0;ii<numPointsV;ii++) //precomputes vectors (vertical)
	{
		ai=vAperture*(0.5-(float)ii/(float)numPointsV);//angle associated to ray i
		wi=zNear*tan(ai);//distance from the hit point i to the x sensor axis
		kV.push_back( (int) ((0.5 - wi/heightM)*heightP) );//pixel index associated to ray i
	}
}

void CrangeImage::depthImage(Cpose3d & ss, vector<float> & depthImg)
{
	float dd, zbuf[widthP*heightP];
	unsigned int ii,jj;

	setViewPoint(ss);
	depthImg.reserve(numPointsH*numPointsV);
	render();
	glReadPixels(1,1,widthP,heightP,GL_DEPTH_COMPONENT,GL_FLOAT,(GLvoid*)(&zbuf));//read the depth buffer
	for (jj=0;jj<numPointsV;jj++)
	{
		for (ii=0;ii<numPointsH;ii++)
		{
			dd = (zNear*zFar)/(zFar-zbuf[kH[ii]+kV[jj]*widthP]*(zFar-zNear));//undoes z buffer normalization
			depthImg.push_back(dd);
		}
	}
}
