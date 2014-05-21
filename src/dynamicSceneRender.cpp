#include "dynamicSceneRender.h"

CdynamicSceneRender::CdynamicSceneRender(unsigned int ww, unsigned int hh, float hAp, float vAp, float nearZ, float farZ) : CsceneRender(ww,hh,hAp,vAp,nearZ,farZ)
{
      //init sensor frame and scan hits lists
      frameList = glGenLists(1);
      scanHitsList = glGenLists(1);
}

CdynamicSceneRender::~CdynamicSceneRender()
{

}

void CdynamicSceneRender::render()
{
   	lookAtValues lav;
	
      //sets target window
	glutSetWindow(winId);
      
      //Clear Screen And Depth Buffer with previous values
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
      
      //Sets GL_MODELVIEW, necessary to perform viewing and model transformations
	glMatrixMode(GL_MODELVIEW); 
      
      //resets the model/view matrix
	glLoadIdentity(); 
      
      //gets "look at" values from a 3D position
	viewPoint.getLookAt(lav); 
      
      //sets matrix viewpoint though gluLookAt()
	gluLookAt(lav.ex,lav.ey,lav.ez,lav.ax,lav.ay,lav.az,lav.ux,lav.uy,lav.uz);
      
      //calls all lists to render: model, scan and sensor frame
      	glCallList(modelList);
      	glCallList(scanHitsList);
	glCallList(frameList);
      
      //that's all
	glFinish();
}

void CdynamicSceneRender::drawScan(Cpose3d devicePose, vector<float> scan, double aperture, double firstAngle)
{
	//GLuint dynamicObjectsList;
	unsigned int ii;
	Cpose3d scanPoint;
	double delta;
	
	delta = aperture/(double)scan.size();
            
      //set window and list
	glutSetWindow(winId);	
	glNewList(scanHitsList, GL_COMPILE);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glColor3f(0.,1.,0.); //set green color
      
	for (ii=0; ii<scan.size(); ii++)
	{
		//compute the point wrt model
		scanPoint.setPose(devicePose);
		scanPoint.rt.turnHeading(firstAngle-(double)ii*delta);//head the ray
		scanPoint.moveForward(scan.at(ii));//move following the ray
		
		//draws the scan point
		glTranslatef(scanPoint.pt(0),scanPoint.pt(1),scanPoint.pt(2));// moves model origin
		gluSphere(gluNewQuadric(),0.1, 5,5);
		glTranslatef(-scanPoint.pt(0),-scanPoint.pt(1),-scanPoint.pt(2));// moves model origin
	}

	glEndList();
	glFinish();	//finish all openGL work
}

void CdynamicSceneRender::drawPoseAxis(Cpose3d & axis)
{
	//GLuint dynamicObjectsList;
	double vv[6];

      //gets pose as XYZ, fwd and left vectors
      axis.getFwdLft(vv);      
      
      //set current window
	glutSetWindow(winId);	
      
      //set list
	glNewList(frameList, GL_COMPILE);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	//x axis
	glColor3f(1.,0.,0.);
	glTranslatef(axis.pt(0),axis.pt(1),axis.pt(2));// moves model origin
	glRotated(90.0,vv[3],vv[4],vv[5]);
	gluCylinder(gluNewQuadric(),0.02,0.02,0.5,10,10);
	glRotated(90.0,vv[3],vv[4],vv[5]);
	
	//y axis
	glColor3f(0.,1.,0.);
	glRotated(-90.0,vv[0],vv[1],vv[2]);
	gluCylinder(gluNewQuadric(),0.02,0.02,0.5,10,10);	
	glRotated(-90.0,vv[0],vv[1],vv[2]);
	
	//z axis
	glColor3f(0.,0.,1.);
	gluCylinder(gluNewQuadric(),0.02,0.02,0.5,10,10);	

      //Ends list
	glEndList();
	glFinish();	//finish all openGL work
}
