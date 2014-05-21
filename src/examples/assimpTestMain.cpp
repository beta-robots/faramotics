//assimp import test

#include <iostream>
#include <assimp/Importer.hpp> // C++ importer interface
#include <assimp/scene.h> // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/Logger.hpp> // logger
#include <assimp/DefaultLogger.hpp> // logger

int main()
{
      //importer
      Assimp::Importer importer;
      
      //logger
      Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE, aiDefaultLogStream_STDOUT);
      
      //import a given file
      const aiScene* scene = importer.ReadFile( "../models/secretarysDesk.obj", 
                                                aiProcess_CalcTangentSpace | 
                                                aiProcess_Triangulate | 
                                                aiProcess_JoinIdenticalVertices | 
                                                aiProcess_SortByPType);
      
      //kill here the logger, just to see what has happened with the importer
      Assimp::DefaultLogger::kill();
      
      //check if scene
      if (scene == NULL) 
      {
            std::cout << __LINE__ << ": Error importing scene. Check if file exists ! " << std::endl;
            return -1; 
      }
      else 
      {
//             for (int ii = 0 ; ii < scene->mRootNode->mNumChildren; ii++)
//             {
                  int meshIdx = scene->mRootNode->mChildren[0]->mMeshes[0];
                  std::cout << __LINE__ << ": mNumFaces: " << scene->mMeshes[meshIdx]->mNumFaces << std::endl;
                  std::cout << __LINE__ << ": HasFaces(): " << scene->mMeshes[meshIdx]->HasFaces() << std::endl;
                  std::cout << __LINE__ << ": ptr mFace[0]: " << &(scene->mMeshes[meshIdx]->mFaces[0]) << std::endl;            
//             }
      }
      
      //exit main
      return 0; 
}
