#include "rangeSector.h"

CrangeSector::CrangeSector(unsigned int numP, float apert, float angularAcc, float rmin, float rmax) : 
            CsceneRender(true)
{
	unsigned int ii;
	float ai, wi;
	unsigned int wPixels, hPixels;
	
	//sets sector parameters 
	numPointsH = numP; 
	numPointsV = SECTOR_VERT_POINTS;
	angularAccuracyH = angularAcc;
	angularAccuracyV = SECTOR_VERT_APERTURE / SECTOR_VERT_POINTS;
	wPixels = (int)(2.0*tan(apert/2.0)/tan(angularAccuracyH));
	hPixels = (int)(2.0*tan(SECTOR_VERT_APERTURE/2.0)/tan(angularAccuracyV));
	
	//sets render parameters
	setRenderParameters(wPixels, hPixels, apert, SECTOR_VERT_APERTURE, rmin, rmax);
	//printRenderParameters();
	
	//inits window and GL state
	initWindow("Range Sector 2D");
	initGL();
    //this->hide();//To search: Some graphics cards avoid depth computations when window is hidden
      
    //avoid color computation , not necessary for depth 
    glDisable(GL_COLOR_MATERIAL);
    
    //init model render lists
    modelList = glGenLists(1);

	//precomputes vectors
	for (ii=0;ii<numPointsH;ii++) //precomputes vectors (horizontal)
	{
		ai=hAperture*(0.5-(float)ii/(float)numPointsH);//angle associated to ray i
		wi=zNear*tan(ai);//distance from the hit point i to the x sensor axis
		kH.push_back( (int) ((0.5 - wi/widthM)*widthP) );//pixel index associated to ray i
		cosHi.push_back( cos(ai) ); //cosinus of the ray angle
	}
}

CrangeSector::~CrangeSector()
{
	kH.clear();
	cosHi.clear();
}

// void CrangeSector::rangeScan(const Pose & _ss, vector<float> & _scan)
// {
// 	float dd, zbuf[widthP];
// 	unsigned int ii;
// 
// 	_scan.reserve(_scan.size()+numPointsH);
// 	
// 	setViewPoint(_ss);
// 	render();
// 	
// 	//cout << __LINE__ << endl;
// 	glReadPixels(1,(int)(floor(heightP/2)+1),widthP,1,GL_DEPTH_COMPONENT,GL_FLOAT,(GLvoid*)(&zbuf));//read the depth buffer
// 	for (ii=0;ii<numPointsH;ii++)
// 	{
// 		dd = (zNear*zFar)/(zFar-zbuf[kH[ii]]*(zFar-zNear));//undoes z buffer normalization 
// 		_scan.push_back(dd/cosHi[ii]);
// 		//scan.at(ii+1) = dd/cosHi[ii];cout << ii << " " ;
// 	}
// }

void CrangeSector::rangeScan(const Eigen::Transform<double,3,Eigen::Affine> & _ps, vector<double> & _scan)
{
    float dd, zbuf[widthP];
    unsigned int ii;

    //allocate space for the output vector
    _scan.reserve(_scan.size()+numPointsH);
    
    //set view point and render the model from it
    setViewPoint(_ps);
    render();
    
    //read depth buffer, and compute the true depths
    glReadPixels(1,(int)(floor(heightP/2)+1),widthP,1,GL_DEPTH_COMPONENT,GL_FLOAT,(GLvoid*)(&zbuf));//read the depth buffer
    for (ii=0;ii<numPointsH;ii++)
    {
        dd = (zNear*zFar)/(zFar-zbuf[kH[ii]]*(zFar-zNear));//undoes z buffer normalization 
        _scan.push_back( (double)(dd/cosHi[ii]) );
        //scan.at(ii+1) = dd/cosHi[ii];cout << ii << " " ;
    }
}
