#include "sceneRender.h"

CsceneRender::CsceneRender(bool visible) : 
    Window(visible), 
    view_point_()
{
    
}

CsceneRender::CsceneRender(unsigned int ww, unsigned int hh, float hAp, float vAp, float nearZ, float farZ, const std::string & label, bool visible) :
    Window(visible), 
    view_point_()  
{
    //init window and GL state
    setRenderParameters(ww, hh, hAp, vAp, nearZ, farZ);
    initWindow(label);
    this->initGL();
    
    //init model render lists
    modelList = glGenLists(1);
}

CsceneRender::~CsceneRender()
{
    //glutDestroyWindow(winId);
}

float CsceneRender::getZNear() const
{
    return zNear; 
}

float CsceneRender::getZFar() const
{
    return zFar; 
}

void CsceneRender::initGL()
{
    //check openGL version
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    
    //initialize GL
    glClearColor(0.0, 0.5, 0.8, 0.5); //Specifies clear color value (sets background color to blue)
    glClearDepth(1.0); //Specifies clear depth value
    glEnable(GL_DEPTH_TEST); //enables hidden-surface removal and depth buffer updates
    glEnable(GL_COLOR_MATERIAL); //enables color rendering. Useful for debugging but not necessary when computing depth data. 
    glShadeModel(GL_FLAT);//GL_SMOOTH
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //renderizes both faces of polygons	
    glColor3f( .8, .8, .8);

    //sets projection and viewport
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(vAperture*180.0/M_PI, widthM/heightM, zNear, zFar);
    glViewport(0, 0, widthP, heightP);
    glFinish();
}

void CsceneRender::setRenderParameters(unsigned int ww, unsigned int hh, float hAp, float vAp, float nearZ, float farZ)
{
    widthP = ww;
    heightP = hh;
    hAperture = hAp;
    vAperture = vAp;
    zNear = nearZ;
    zFar = farZ;
    widthM = 2.0*zNear*tan(hAperture/2.0); //metric width
    heightM = 2.0*zNear*tan(vAperture/2.0); //metric height	
}

void CsceneRender::printRenderParameters()
{
    cout << "CsceneRender::printRenderParameters:" << endl;
    cout << "  w [pixels] = " << widthP << endl;
    cout << "  h [pixels] = " << heightP << endl;
    cout << "  apertureH [deg] = " << hAperture*180.0/M_PI << endl;
    cout << "  apertureV [deg] = " << vAperture*180.0/M_PI << endl;
    cout << "  w [meters] = " << widthM << endl;
    cout << "  h [meters] = " << heightM << endl;
    cout << "  zNear [meters] = " << zNear << endl;
    cout << "  zFar [meters] = " << zFar << endl;
}

void CsceneRender::setViewPoint(const Eigen::Affine3d & _vp)
{
    view_point_ = _vp;
    //std::cout << "view_point_:" << std::endl << view_point_.matrix() << std::endl; 
}


void CsceneRender::render()
{    
    //sets target window      
    glutSetWindow(winId);
    
    //check to show or not the window
    if (!isVisible) this->hide();
    
    //Clear Screen And Depth Buffer with previous values
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //Sets GL_MODELVIEW, necessary to perform viewing and model transformations
    glMatrixMode(GL_MODELVIEW);
    
    //resets the model/view matrix
    glLoadIdentity();
    
//     TODO set openGL matrix directly from Eigen Transform data pointer. See http://www.songho.ca/opengl/gl_transform.html
//     Eigen::Transform<double,3,Eigen::Affine> vp(view_point_.inverse());
//     glLoadMatrixd(vp.data()); //directly from Eigen matrix pointer. See http://eigen.tuxfamily.org/dox/group__TutorialGeometry.html
    
    //sets openGL matrix through gluLookAt (eye, at, up). eye=translation, at=Kx+eye, up=z
    gluLookAt(  view_point_(0,3), view_point_(1,3), view_point_(2,3),
                view_point_(0,0)*10000. + view_point_(0,3), view_point_(1,0)*10000. + view_point_(1,3) , view_point_(2,0)*10000. + view_point_(2,3),
                view_point_(0,2), view_point_(1,2), view_point_(2,2) );
    
    //calls lists to render
    glCallList(modelList);
    
    //that's all
    glFinish();
}

int CsceneRender::loadAssimpModel(const string & modelFile, const bool wireFrame)
{      
    // Import model file
    const aiScene* scene = importer.ReadFile( modelFile,
        aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
      
    if ( !scene ) // If the import failed, report it
    {
        std::cout << importer.GetErrorString() << std::endl;
        return -1;
    }
    else //if scene != NULL, import suceed, so associates the scene to the model List
    {
        //Sets modelList
        glutSetWindow(winId);
        glNewList(modelList, GL_COMPILE);      
        
        //Draw the model, only polygon faces, filled with orange
        glColor3f(5.,3.,0.);            
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        this->recursiveAssimpRender(scene, scene->mRootNode);
        
        //if wireFrame, redraw the model but only the polygon line borders in black
        if (wireFrame) 
        {
            glColor4f(0.,0.,0.,0.1);            
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            this->recursiveAssimpRender(scene, scene->mRootNode);
        }

        
        glEndList();
        glFinish(); //finish all openGL work      
        
        // We're done. Everything will be cleaned up by the importer destructor
        std::cout << "Model loaded successfully by Assimp::Importer";
        std::cout << " (Version: " << aiGetVersionMajor() << "." << aiGetVersionMinor() << ")" << std::endl;
        return 1;
    }
}

//Assimp Recursve Render: http://www.lighthouse3d.com/cg-topics/code-samples/importing-3d-models-with-assimp/
void CsceneRender::recursiveAssimpRender (const struct aiScene *sc, const struct aiNode* nd)
{
    unsigned int i;
    unsigned int n = 0, t;
    aiMatrix4x4 m = nd->mTransformation;

    // update transform
    aiTransposeMatrix4(&m);
    glPushMatrix();
    glMultMatrixf((float*)&m);

    for (; n < nd->mNumMeshes; ++n)
    {
        const struct aiMesh* mesh = sc->mMeshes[nd->mMeshes[n]];
        
        if(mesh->mNormals == NULL) 
        {
            glDisable(GL_LIGHTING);
        } 
        else 
        {
            glEnable(GL_LIGHTING);
        }

        if ( mesh->HasFaces() )
        {
            for (t = 0; t < mesh->mNumFaces; ++t) 
            {
                const aiFace* face = &mesh->mFaces[t];
                GLenum face_mode;

                switch(face->mNumIndices) 
                {
                    case 1: face_mode = GL_POINTS;// std::cout << "GL_POINTS" << std::endl; break;
                    case 2: face_mode = GL_LINES; //std::cout << "GL_LINES" << std::endl; break;
                    case 3: face_mode = GL_TRIANGLES; //std::cout << "GL_TRIANGLES" << std::endl; break;
                    default: face_mode = GL_POLYGON; //std::cout << "GL_POLYGON" << std::endl; break;
                }
                
                glBegin(face_mode);
                for(i = 0; i < face->mNumIndices; i++) 
                {
                    int index = face->mIndices[i];
                    if(mesh->mColors[0] != NULL) glColor4fv((GLfloat*)&mesh->mColors[0][index]);
                    if(mesh->mNormals != NULL) glNormal3fv(&mesh->mNormals[index].x);
                    glVertex3fv(&mesh->mVertices[index].x);
                }
                glEnd();
            }
        }
    }

    // draw all children
    for (n = 0; n < nd->mNumChildren; ++n) 
    {
        this->recursiveAssimpRender(sc, nd->mChildren[n]);
    }

    // pop gl matrix
    glPopMatrix();
}

void CsceneRender::loadHardModel(const int modelID)
{
    glutSetWindow(winId);
    glNewList(modelList, GL_COMPILE);

    switch(modelID)
    {
        case DEBUG_SCENE:
            //simple rectangular floor
            glColor3f(0.0,1.0,0.7);
            glBegin(GL_QUADS);//Draws the floor
                glNormal3f( 0.0,0.0, 1.0);
                glVertex3f(-1.0,23.0, 0.0);
                glVertex3f(-1.0,-1.0, 0.0);
                glVertex3f( 23.0,-1.0, 0.0);
                glVertex3f( 23.0, 23.0, 0.0);
            glEnd();

            //draws a red wall
            glColor3f( 1.0, 0.0, 0.0);
            glBegin(GL_QUADS);
                glNormal3f( -1.0, 0.0, 0.0);
                glVertex3f(15.0, 15.0, 0.0);
                glVertex3f(15.0, 7.0, 0.0);
                glVertex3f(15.0, 7.0, 3.0);
                glVertex3f(15.0, 15.0, 3.0);
            glEnd();

            //Draws a magenta wall
            glColor3f( 1., 0., 1.0);
            glBegin(GL_QUADS);
                glNormal3f( -1.0, 0.0, 0.0);
                glVertex3f(14.0, 17.0, 0.0);
                glVertex3f(14.0, 12.0, 0.0);
                glVertex3f(14.0, 12.0, 2.0);
                glVertex3f(14.0, 17.0, 2.0);
            glEnd();

            //draws an orange cone
            glColor3f(0.9,0.6,0.0);
            glTranslatef(12.0,15.5,0.0);// moves model origin
            gluCylinder(gluNewQuadric(),1.2, 0.0, 3.5, 25,25);
            glTranslatef(-12.0,-15.5,0.0);// moves model origin

            //draws a yellow sphere
            glColor3f(1,0.9,0.1);
            glTranslatef(11.0,10.0,0.0);// moves model origin
            gluSphere(gluNewQuadric(),2.0, 40,40);
            glTranslatef(-11.0,-10.0,0.0);// moves model origin
            break;
            
        case SPHERE:
            //draws a yellow sphere
            glColor3f(1,0.9,0.1);
            glTranslatef(3.0,0.0,0.0);// moves model origin
            gluSphere(gluNewQuadric(),2.0, 140,140);
            //glTranslatef(-3.0,0.0,0.0);// comes back
            break;
                 
        default:
            cout << "CsceneRender::loadModel(): Unknown modelID" << endl; 
            break;
            
    }//end switch
      
    //glLoadIdentity();
    glEndList();
    glFinish(); //finish all openGL work
}
