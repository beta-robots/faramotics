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
                //std::cout << kH.back() << std::endl; 
	}	
	for (ii=0;ii<numPointsV;ii++) //precomputes vectors (vertical)
	{
		ai=vAperture*(0.5-(float)ii/(float)numPointsV);//angle associated to ray i
		wi=zNear*tan(ai);//distance from the hit point i to the x sensor axis
		kV.push_back( (int) ((0.5 - wi/heightM)*heightP) );//pixel index associated to ray i
                //std::cout << kV.back() << std::endl; 
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

void CrangeImage::depthImage(const Eigen::Affine3d & _ss, vector<float> & depthImg)
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

void CrangeImage::pointCloud(const Eigen::Affine3d & _ss, 
                            vector<float> & _x_values,
                            vector<float> & _y_values,
                            vector<float> & _z_values )
{
    //local variables
    float dd, zbuf[widthP*heightP];
    float ai,aj; 
    unsigned valid_points = 0; 
    
    //allocates memory
    _x_values.reserve(numPointsH*numPointsV);
    _y_values.reserve(numPointsH*numPointsV);
    _z_values.reserve(numPointsH*numPointsV);    

    //set view point
    setViewPoint(_ss);
        
    //render model
    render();
    
    //get depth buffer values
    glReadPixels(1,1,widthP,heightP,GL_DEPTH_COMPONENT,GL_FLOAT, zbuf );//read the depth buffer
    for (unsigned int ii=0; ii<numPointsV; ii++)
    {
        //vertical angle (elevation) //TODO precompute them at constructor
        ai = vAperture*(-0.5+(float)ii/(float)numPointsV);
        
        for (unsigned int jj=0; jj<numPointsH; jj++)
        {
            //depth value
            dd = (zNear*zFar)/(zFar-zbuf[kV[ii]*widthP+kH[jj]]*(zFar-zNear));//undoes z buffer normalization
            
            //if ( ( dd < (zFar-10) ) && ( dd > (zNear+10) ) )//avoid returning points close ot the limits of the frustum
            if ( dd < (zFar-10) )//avoid returning points close ot the limits of the frustum                    
            {
                //depth. According LookAt rendering, where "at" vector is aligned with X axis
                _x_values.push_back(dd); 
                
                //horizontal angle (azimuth) //TODO precompute them at constructor
                aj = hAperture*(0.5-(float)jj/(float)numPointsH);
                
                //camera plane coordinates
                _y_values.push_back(dd*tan(aj)); //horizontal coordinate
                _z_values.push_back(dd*tan(ai)); //vertical coordinate
                
                //increment valid counter
                valid_points ++; 
            }
        }
    }
    
    //crop allocated memory
    _x_values.resize(valid_points);
    _y_values.resize(valid_points);
    _z_values.resize(valid_points);
    
    //debugging
    //_x_values.at(valid_points-1) = 1.;
    
}



