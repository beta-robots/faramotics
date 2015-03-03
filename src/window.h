#ifndef WINDOW_H
#define WINDOW_H

//std
#include <iostream>
#include <string>

//GLUT
#include <GL/glut.h>

//namespaces
using namespace std;

//just encapsulate glutInit() call
namespace faramotics
{
    static void initGLUT(int argc, char** argv)
    {
        glutInit(&argc, argv);
    }
}

/** \brief Just a GLUT window to draw pixels
 * 
 * Just a GLUT window to draw pixels
 * 
 **/
class Window
{
    protected:
        /** \brief Window identifier
        * 
        * Window identifier for multiple window application cases
        * 
        */
        int winId;

        /** \brief Visibility boolean
        * 
        * Set to true indicates that the window should be visible, false indicates a hidden window
        * 
        */          
        bool isVisible;

        /** \brief Window width [pixels]
        * 
        * Window width [pixels]
        * 
        */          
        unsigned int widthP;

        /** \brief Window height [pixels]
        * 
        * Window height [pixels]
        * 
        */                      
        unsigned int heightP;
        
        /** \brief screen Y offset
        * 
        * screen Y offset
        * 
        **/
        static unsigned int yOffset; 
                
    protected:
        /** \brief Initializes GLUT window
        *
        * Glut window required initializations
        *
        */                                  
        void initWindow(const std::string & label = "faramotics");

        /** \brief Initializes GL
        *
        * GL required initializations just for pixel drawing
        *
        */                                              
        void initGL();
        
    public:
        /** \brief Constructor without sizes
        *
        * Constructor without sizes
        *
        */                                  
        Window(bool visible=true);

        /** \brief Constructor with size
        *
        * Constructor. Requires width and height of the window in pixels
        *
        */                                          
        Window(unsigned int ww, unsigned int hh, bool visible=true, const std::string & label = "faramotics");
        
        /** \brief Destructor
        *
        * Destructor.
        *
        */                                  
        ~Window();
        
        /** \brief Returns width in pixels
         * 
         * Returns width in pixels
         * 
         **/
        unsigned int getWidthPixels() const; 
        
        /** \brief Returns height in pixels
         * 
         * Returns height in pixels
         * 
         **/
        unsigned int getHeightPixels() const; 
        
        /** \brief Shows this window & renders
        *
        * Shows this window and renders the model from the viewPoint 
        *
        */                                                          
        void show();
            
        /** \brief Hides this window
        *
        * Hides this window
        *
        */                                                          
        void hide();

        /** \brief Sets window to full screen. (Doesn't work properly !!!)
        *
        * Sets size of the rendering window to full screen 
        *
        */                                                          
        void fullScreen();
        
        /** \brief Draw values to a single color channel
         * 
         * Draw values to a single color channel.
         * Array should have widthP x heightP dimension
         * 
         **/
        void draw(float* values);

        /** \brief Draw values to a single color channel, mapped between max and min
         * 
         * Draw values to a single color channel, mapped between max and min
         * Array should have widthP x heightP dimension
         * 
         **/
        void drawDepths(float* values, float minV, float maxV);
};
#endif 
