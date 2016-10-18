#include "rangeImage.h"

CrangeImage::CrangeImage(unsigned int numPH, unsigned int numPV, float apertH, float apertV, float angularAccH, float angularAccV, float rmin, float rmax) : 
            CsceneRender(true)
{
	rangeImageInit(numPH, numPV, apertH, apertV, angularAccH, angularAccV, rmin, rmax);
}

CrangeImage::CrangeImage(unsigned int deviceId) :
            CsceneRender(true)
{
    switch (deviceId)
    {
        case SR4000:
            rangeImageInit( SR4000_NUM_POINTS_HORIZONTAL, SR4000_NUM_POINTS_VERTICAL, 
                            SR4000_APERTURE_HORIZONTAL, SR4000_APERTURE_VERTICAL, 
                            SR4000_ANGULAR_ACCURACY_HORIZONTAL, SR4000_ANGULAR_ACCURACY_VERTICAL, 
                            SR4000_MIN_RANGE, SR4000_MAX_RANGE);                    
            cout << "CrangeImage::CrangeImage(): SR4000 device created" << endl;
            break;
            
        case KINECT:
            rangeImageInit( KINECT_NUM_POINTS_HORIZONTAL, KINECT_NUM_POINTS_VERTICAL, 
                            KINECT_APERTURE_HORIZONTAL, KINECT_APERTURE_VERTICAL, 
                            KINECT_ANGULAR_ACCURACY_HORIZONTAL, KINECT_ANGULAR_ACCURACY_VERTICAL, 
                            KINECT_MIN_RANGE, KINECT_MAX_RANGE);                    
            cout << "CrangeImage::CrangeImage(): KINECT device created" << endl;
            break;

        case ENSENSO_N35:
            rangeImageInit( ENSENSO_N35_NUM_POINTS_HORIZONTAL, ENSENSO_N35_NUM_POINTS_VERTICAL, 
                            ENSENSO_N35_APERTURE_HORIZONTAL, ENSENSO_N35_APERTURE_VERTICAL, 
                            ENSENSO_N35_ANGULAR_ACCURACY_HORIZONTAL, ENSENSO_N35_ANGULAR_ACCURACY_VERTICAL, 
                            ENSENSO_N35_MIN_RANGE, ENSENSO_N35_MAX_RANGE);                    
            cout << "CrangeImage::CrangeImage(): ENSENSO_N35 device created" << endl;
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
	
	//inits window and GL state
	initWindow("Range Image");
	initGL();
	//this->hide();
	
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

unsigned int CrangeImage::getNumHorizontalPoints() const
{
    return numPointsH;
}

unsigned int CrangeImage::getNumVerticalPoints() const
{
    return numPointsV;
}

void CrangeImage::depthImage(const Eigen::Transform<double,3,Eigen::Affine> & _ss, vector<float> & depthImg)
{
	float dd, zbuf[widthP*heightP];
	unsigned int ii,jj;

    //sets view point
	setViewPoint(_ss);
    
    //allocates memory
	depthImg.reserve(numPointsH*numPointsV);
    //depthImg.resize(numPointsH*numPointsV);
    
    //render model
	render();
    
    //get depth buffer values
	//glReadPixels(1,1,widthP,heightP,GL_DEPTH_COMPONENT,GL_FLOAT,(GLvoid*)(&zbuf));//read the depth buffer
    glReadPixels(1,1,widthP,heightP,GL_DEPTH_COMPONENT,GL_FLOAT, zbuf );//read the depth buffer
	for (jj=0;jj<numPointsV;jj++)
	{
		for (ii=0;ii<numPointsH;ii++)
		{
			dd = (zNear*zFar)/(zFar-zbuf[kH[ii]+kV[jj]*widthP]*(zFar-zNear));//undoes z buffer normalization
			depthImg.push_back(dd);
            //depthImg[jj*numPointsH + ii] = dd;
		}
	}
}
