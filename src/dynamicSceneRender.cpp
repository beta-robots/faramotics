#include "dynamicSceneRender.h"

CdynamicSceneRender::CdynamicSceneRender(unsigned int ww, unsigned int hh, float hAp, float vAp, float nearZ, float farZ, const std::string & label) :
            CsceneRender(ww,hh,hAp,vAp,nearZ,farZ, label)
{
      //init GL lists
      frameList = glGenLists(1);
      scanHitsList = glGenLists(1);
      depthPointsList = glGenLists(1);
}

CdynamicSceneRender::~CdynamicSceneRender()
{
    //nothing to do
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
      glCallList(depthPointsList);
      glCallList(frameList);
      glCallList(frameVectorList);
      glCallList(cornersList);
      glCallList(landmarkList);
      
      //that's all
	glFinish();
}

void CdynamicSceneRender::drawPoseAxis(Cpose3d & axis)
{
	//GLuint dynamicObjectsList;
	double vv[6];

    //gets pose as XYZ, fwd and left vectors
    axis.getFwdLft(vv);      
      
    //set current window and reset matrix view
	glutSetWindow(winId);	
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
        
    //set list
	glNewList(frameList, GL_COMPILE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //move axis origin
    glTranslatef(axis.pt(0),axis.pt(1),axis.pt(2));// moves model origin
    //glPushMatrix();
    
	//x axis
	glColor3f(100.,0.,0.);
	glRotated(90.0+axis.rt.pitch(inDEGREES),vv[3],vv[4],vv[5]);
    //glPushMatrix();
	gluCylinder(gluNewQuadric(),0.02,0.02,0.5,10,10);
	glRotated(-90.0-axis.rt.pitch(inDEGREES),vv[3],vv[4],vv[5]);
    //glPopMatrix();
	
	//y axis
	glColor3f(0.,100.,0.);
	glRotated(-90.0,vv[0],vv[1],vv[2]);
    //glPushMatrix();
	gluCylinder(gluNewQuadric(),0.02,0.02,0.5,10,10);	
	glRotated(90.0,vv[0],vv[1],vv[2]);
    //glPopMatrix();
	
	//z axis
	glColor3f(0.,0.,100.);
	gluCylinder(gluNewQuadric(),0.02,0.02,0.5,10,10);	

	glTranslatef(-axis.pt(0),-axis.pt(1),-axis.pt(2));// moves model origin

      //Ends list
	glEndList();
	glFinish();	//finish all openGL work
}

void CdynamicSceneRender::drawPoseAxisVector(const vector<Cpose3d> & axis_vector)
{
	//set current window and reset matrix view
	glutSetWindow(winId);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//set list
	glNewList(frameVectorList, GL_COMPILE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (uint i= 0; i < axis_vector.size(); i++)
	{
		Cpose3d axis = axis_vector[i];

		//GLuint dynamicObjectsList;
		double vv[6];

		//gets pose as XYZ, fwd and left vectors
		axis.getFwdLft(vv);

		//move axis origin
		glTranslatef(axis.pt(0),axis.pt(1),axis.pt(2));// moves model origin
		//glPushMatrix();

		//x axis
		glColor3f(50.,0.,0.);
		glRotated(90.0+axis.rt.pitch(inDEGREES),vv[3],vv[4],vv[5]);
		//glPushMatrix();
		gluCylinder(gluNewQuadric(),0.02,0.02,0.3,10,10);
		glRotated(-90.0-axis.rt.pitch(inDEGREES),vv[3],vv[4],vv[5]);
		//glPopMatrix();

		//y axis
		glColor3f(0.,50.,0.);
		glRotated(-90.0,vv[0],vv[1],vv[2]);
		//glPushMatrix();
		gluCylinder(gluNewQuadric(),0.02,0.02,0.3,10,10);
		glRotated(90.0,vv[0],vv[1],vv[2]);
		//glPopMatrix();

		//z axis
		glColor3f(0.,0.,50.);
		gluCylinder(gluNewQuadric(),0.02,0.02,0.3,10,10);

		glTranslatef(-axis.pt(0),-axis.pt(1),-axis.pt(2));// moves model origin
	}
	//Ends list
	glEndList();
	glFinish();	//finish all openGL work
}

void CdynamicSceneRender::drawScan(Cpose3d & devicePose, const vector<float> & scan, const double aperture, const double firstAngle)
{
      unsigned int ii;
      Cpose3d scanPoint;
      double delta;
      
      //precompute angle increment
      delta = aperture/(double)scan.size();
            
      //set window and list
      glutSetWindow(winId);   

      glNewList(scanHitsList, GL_COMPILE);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glColor3f(0.,100.,0.); //set light green color

      for (ii=0; ii<scan.size(); ii++)
      {
            //compute the point wrt model
            scanPoint.setPose(devicePose);
            scanPoint.rt.turnHeading(firstAngle-(double)ii*delta);//head the ray
            scanPoint.moveForward(scan.at(ii));//move following the ray
            
            //draws the scan point
            glTranslatef(scanPoint.pt(0),scanPoint.pt(1),scanPoint.pt(2));// moves model origin
            gluSphere(gluNewQuadric(),0.05, 5,5);
            glTranslatef(-scanPoint.pt(0),-scanPoint.pt(1),-scanPoint.pt(2));// moves model origin
      }
      glEndList();
      glFinish(); //finish all openGL work
}

void CdynamicSceneRender::drawScan(Cpose3d & devicePose, const vector<double> & scan, const double aperture, const double firstAngle)
{
      unsigned int ii;
      Cpose3d scanPoint;
      double delta;
      
      //precompute angle increment
      delta = aperture/(double)scan.size();
            
      //set window and list
      glutSetWindow(winId);   

      glNewList(scanHitsList, GL_COMPILE);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glColor3f(0.,100.,0.); //set light green color

      for (ii=0; ii<scan.size(); ii++)
      {
            //compute the point wrt model
            scanPoint.setPose(devicePose);
            scanPoint.rt.turnHeading(firstAngle-(double)ii*delta);//head the ray
            scanPoint.moveForward(scan.at(ii));//move following the ray
            
            //draws the scan point
            glTranslatef(scanPoint.pt(0),scanPoint.pt(1),scanPoint.pt(2));// moves model origin
            gluSphere(gluNewQuadric(),0.05, 5,5);
            glTranslatef(-scanPoint.pt(0),-scanPoint.pt(1),-scanPoint.pt(2));// moves model origin
      }
      glEndList();
      glFinish(); //finish all openGL work
}

void CdynamicSceneRender::drawCorners(Cpose3d & devicePose, const vector<double> & corners, double height, double radius)
{
      Cpose3d cornerPoint;

      //set window and list
      glutSetWindow(winId);

      glNewList(cornersList, GL_COMPILE);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glColor3f(0.,0.,100.); //set light green color

      for (unsigned int ii=0; ii<corners.size(); ii+=2)
      {
            //compute the point wrt model
    	  	cornerPoint.setPose(devicePose);
    	  	cornerPoint.moveForward(corners[ii]);
    	  	cornerPoint.rt.turnHeading(M_PI/2);
    	  	cornerPoint.moveForward(corners[ii+1]);

            //draws the scan point
            glTranslatef(cornerPoint.pt(0),cornerPoint.pt(1),cornerPoint.pt(2)-height/2);// moves model origin
            gluCylinder(gluNewQuadric(),radius,radius,height,5,5);
            glTranslatef(-cornerPoint.pt(0),-cornerPoint.pt(1),-cornerPoint.pt(2)+height/2);// moves model origin
      }

      glEndList();
      glFinish(); //finish all openGL work
}

void CdynamicSceneRender::drawLandmarks(const vector<double> & landmarks, double height, double radius)
{
      //set window and list
      glutSetWindow(winId);

      glNewList(landmarkList, GL_COMPILE);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glColor3f(100.,0.,0.); //set light red color

      for (unsigned int ii=0; ii<landmarks.size(); ii+=3)
      {
            //draws the scan point
            glTranslatef(landmarks[ii],landmarks[ii+1],landmarks[ii+2]-height/2);// moves model origin
            gluCylinder(gluNewQuadric(),radius,radius,height,5,5);
            glTranslatef(-landmarks[ii],-landmarks[ii+1],-landmarks[ii+2]+height/2);// moves model origin
      }

      glEndList();
      glFinish(); //finish all openGL work
}

void CdynamicSceneRender::drawDepthPoints(Cpose3d & devicePose, const vector<float> & depths, const unsigned int nPointsH , const unsigned int nPointsV, const double apertureH, const double apertureV, const double firstAngleH, const double firstAngleV)
{
    unsigned int ii, jj;
    Cpose3d depthPoint;
    double deltaH, deltaV, dd;

    //precompute angle increments
    deltaH = apertureH/(double)nPointsH;
    deltaV = apertureV/(double)nPointsV;
        
    //set window and list
    glutSetWindow(winId);   

    glNewList(depthPointsList, GL_COMPILE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glColor3f(100.,0.,0.); //set light orange color ?

    //init point 
//     depthPoint.setPose(devicePose);
//     depthPoint.rt.turnPitch(-firstAngleV);
    
    for (ii=0; ii<nPointsV; ii=ii+5)
    {
//         depthPoint.rt.turnPitch((double)ii*deltaV);//head the ray in pitch
//         depthPoint.rt.turnHeading(firstAngleH);
        for (jj=0; jj<nPointsH; jj=jj+5)
        {
            if (depths.at(ii*nPointsH + jj) < 5.)
            {
                //compute the point wrt model
                //depthPoint.rt.turnHeading(-(double)jj*deltaH);//head the ray in heading            
                depthPoint.setPose(devicePose);
                depthPoint.rt.turnHeading(firstAngleH-(double)jj*deltaH);//head the ray in heading
                depthPoint.rt.turnPitch(firstAngleV-(double)ii*deltaV);//head the ray in pitch
                dd = depths.at(ii*nPointsH + jj) / (cos(firstAngleH-(double)jj*deltaH)*cos(firstAngleH-(double)jj*deltaH));
                //dd = depths.at(ii*nPointsH + jj);
                depthPoint.moveForward(dd);//move following the ray
            
                //draws the scan point
                glColor4f(100.,0., 0., dd*100./5.); //set light orange color ?
                glTranslatef(depthPoint.pt(0),depthPoint.pt(1),depthPoint.pt(2));// moves model origin
                gluSphere(gluNewQuadric(),0.01, 2,2);
                glTranslatef(-depthPoint.pt(0),-depthPoint.pt(1),-depthPoint.pt(2));// moves model origin
                
                //reset the forward movement
                //depthPoint.moveForward(-depths.at(ii*nPointsH + jj));
            }
        }
    }

    glEndList();
    glFinish(); //finish all openGL work    
}
