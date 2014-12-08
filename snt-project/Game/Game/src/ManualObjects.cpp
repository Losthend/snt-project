#include "stdafx.h"
#include "ManualObjects.h"

//---------------------------------------------------------------------
//--Genera un ManualObject denominado "ManualObjectCube" que puede
//--ser utilizado como "mesh" al crear una nueva entidad.
//---------------------------------------------------------------------
void simpleCube(Ogre::SceneManager* mSceneMgr)
{
	//Se declara el objeto
	Ogre::ManualObject* lManualObject = mSceneMgr->createManualObject("ManualObjectCube");	
	//Se le asigna el material/textura que se desea utilizar, declarado en:
	//C:\Ogre\OgreSDK_vc9_v1-9-0\media\materials\scripts\MaterialDePrueba.material
	lManualObject->begin("MyMaterial1", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	//La creación del objeto, dando vertices, coordenadas para la textura, etc.
	float lSize = 10.0f;
	float cp = 1.0f * lSize ;
	float cm = -1.0f * lSize;

	lManualObject->position(cm, cp, cm);
	//lManualObject->colour(Ogre::ColourValue(0.0f,1.0f,0.0f,1.0f));
	lManualObject->textureCoord(0.0, 1.0);

	lManualObject->position(cp, cp, cm);
	//lManualObject->colour(Ogre::ColourValue(1.0f,1.0f,0.0f,1.0f));
	lManualObject->textureCoord(1.0, 1.0);

	lManualObject->position(cp, cm, cm);
	//lManualObject->colour(Ogre::ColourValue(1.0f,0.0f,0.0f,1.0f));
	lManualObject->textureCoord(1.0, 0.0);

	lManualObject->position(cm, cm, cm);
	//lManualObject->colour(Ogre::ColourValue(0.0f,0.0f,0.0f,1.0f));
	lManualObject->textureCoord(0.0, 0.0);

	//-----------------------------

	lManualObject->position(cm, cp, cp);
	//lManualObject->colour(Ogre::ColourValue(0.0f,1.0f,1.0f,1.0f));
	lManualObject->textureCoord(0.0, 1.0);

	lManualObject->position(cp, cp, cp);
	//lManualObject->colour(Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f));
	lManualObject->textureCoord(1.0, 1.0);

	lManualObject->position(cp, cm, cp);
	//lManualObject->colour(Ogre::ColourValue(1.0f,0.0f,1.0f,1.0f));
	lManualObject->textureCoord(1.0, 0.0);

	lManualObject->position(cm, cm, cp);
	//lManualObject->colour(Ogre::ColourValue(0.0f,0.0f,1.0f,1.0f));
	lManualObject->textureCoord(0.0, 0.0);

	//-----------------------------

	// face behind / front
	lManualObject->triangle(0,1,2);
	lManualObject->triangle(2,3,0);
	lManualObject->triangle(4,6,5);
	lManualObject->triangle(6,4,7);

	// face top / down
	lManualObject->triangle(0,4,5);
	lManualObject->triangle(5,1,0);
	lManualObject->triangle(2,6,7);
	lManualObject->triangle(7,3,2);

	// face left / right
	lManualObject->triangle(0,7,4);
	lManualObject->triangle(7,0,3);
	lManualObject->triangle(1,5,6);
	lManualObject->triangle(6,2,1);		

	//-----------------------------

	lManualObject->end();
	lManualObject->convertToMesh("ManualObjectCube");
}