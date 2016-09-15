#include "window.h"

//---------------- faramotics namespace --------------------------------------------------

namespace faramotics
{
    void initGLUT(int argc, char** argv)
    {
        glutInit(&argc, argv);
    }
}

//---------------- Window class --------------------------------------------------

unsigned int Window::yOffset = 100; //init offset

Window::Window(bool visible)
{
    isVisible = visible;
}

Window::Window(unsigned int ww, unsigned int hh, bool visible, const std::string & label)
{
    //window visibility
    isVisible = visible; 
    
    //size
    widthP = ww;
    heightP = hh;

    //init window state
    initWindow(label);
    initGL();
}

Window::~Window()
{
    glutDestroyWindow(winId);
}

unsigned int Window::getWidthPixels() const
{
    return widthP;
}

unsigned int Window::getHeightPixels() const
{
    return heightP;
}

void Window::initWindow(const std::string & label)
{
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (widthP, heightP);
    glutInitWindowPosition (100, yOffset);
    yOffset += 50;
    glutCreateWindow(label.c_str());
    winId = glutGetWindow();
    if(!isVisible) glutHideWindow();
}

void Window::initGL()
{
    //check openGL version
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    
    //initialize GL
    glClearColor(0., 0., 0., 1.); //Specifies clear color value to black
    glClearDepth(1.0); //Specifies clear depth value
//       glEnable(GL_DEPTH_TEST); //enables hidden-surface removal and depth buffer updates
    glEnable(GL_COLOR_MATERIAL); //enables color rendering. Useful for debugging but not necessary when computing depth data. 
//       glShadeModel(GL_FLAT);//GL_SMOOTH
//       glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //renderizes both faces of polygons      
//       glColor3f( .8, .8, .8);

    //sets projection and viewport
//       glMatrixMode(GL_PROJECTION);
//       glLoadIdentity();
//       gluPerspective(vAperture*180.0/M_PI, widthM/heightM, zNear, zFar);
    glViewport(0, 0, widthP, heightP);
    glFinish();
}

void Window::show()
{
    isVisible = true;
    glutSetWindow(winId);
    glutShowWindow(); 
    //render();
}

void Window::hide()
{
    isVisible = false;
    glutSetWindow(winId);
    glutHideWindow();
}

void Window::fullScreen()
{
    //to do: code below doesn't work properly. why??
    glutSetWindow(winId);
    
    widthP = glutGet(GLUT_SCREEN_WIDTH);
    heightP = glutGet(GLUT_SCREEN_HEIGHT);
    std::cout << "sW = " << widthP << "; sH = " << heightP << std::endl; 
//     glMatrixMode(GL_PROJECTION);
//     glLoadIdentity();
//     gluPerspective(vAperture*180.0/M_PI, widthM/heightM, zNear, zFar);
    glViewport(0, 0, widthP, heightP);
    
    glFinish();
    glutFullScreen();
}

void Window::draw(float* values)
{
    glutSetWindow(winId);
    glDrawPixels(widthP, heightP, GL_GREEN, GL_FLOAT, values);
    glFinish();
}

void Window::drawDepths(float* values, float minV, float maxV)
{
    for (unsigned int ii = 0; ii<widthP*heightP; ii++)
    {
        values[ii] = (values[ii]-minV)/(maxV-minV);
    }
    draw(values);
}