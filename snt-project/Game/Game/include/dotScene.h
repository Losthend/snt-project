#ifndef DOT_SCENE_H
#define DOT_SCENE_H
 
#include <Ogre.h>
#include <vector>
#include "tinyxml.h"
using namespace std;
using namespace Ogre;

#include "FuncionesGenerales.h"
#include "Object.h"

Ogre::Light* LoadLight( TiXmlElement *XMLLight){
	TiXmlElement *XMLDiffuse, *XMLSpecular, *XMLAttentuation, *XMLPosition, *XMLRange;

	Light* l = gSceneMgr->createLight( XMLLight->Attribute("name") );
   if( !XMLLight->Attribute("type") || String(XMLLight->Attribute("type")) == "point" )
      l->setType( Light::LT_POINT );
   else if( String(XMLLight->Attribute("type")) == "directional")
      l->setType( Light::LT_DIRECTIONAL );
   else if( String(XMLLight->Attribute("type")) == "spot")
      l->setType( Light::LT_SPOTLIGHT );
   else if( String(XMLLight->Attribute("type")) == "radPoint")
      l->setType( Light::LT_POINT );
 
   l->setVisible( true );
   if( XMLLight->Attribute("visible") )
      if( String(XMLLight->Attribute("visible")) == "false" )
         l->setVisible( false );

  l->setCastShadows( true );
   if( XMLLight->Attribute("castShadows") )
      if( String(XMLLight->Attribute("castShadows")) == "false" )
         l->setCastShadows( false );   

  XMLDiffuse = XMLLight->FirstChildElement("colourDiffuse");
   if( XMLDiffuse ){
      ColourValue Diffuse;
      Diffuse.r = Ogre::StringConverter::parseReal( XMLDiffuse->Attribute("r") );
      Diffuse.g = Ogre::StringConverter::parseReal( XMLDiffuse->Attribute("g") );
      Diffuse.b = Ogre::StringConverter::parseReal( XMLDiffuse->Attribute("b") );
      Diffuse.a = 1;
      l->setDiffuseColour(Diffuse);
   }

   XMLSpecular = XMLLight->FirstChildElement("colourSpecular");
   if( XMLSpecular ){
      ColourValue Specular;
      Specular.r = Ogre::StringConverter::parseReal( XMLSpecular->Attribute("r") );
      Specular.g = Ogre::StringConverter::parseReal( XMLSpecular->Attribute("g") );
      Specular.b = Ogre::StringConverter::parseReal( XMLSpecular->Attribute("b") );
      Specular.a = 1;
      l->setSpecularColour(Specular);
   }
 
   XMLAttentuation = XMLLight->FirstChildElement("lightAttenuation");
   if( XMLAttentuation )
   {
      Real range, constant, linear, quadratic;
      range = l->getAttenuationRange();
      constant = l->getAttenuationConstant();
      linear = l->getAttenuationLinear();
      quadratic = l->getAttenuationQuadric();
 
      if( XMLAttentuation->Attribute("range") )
         range = StringConverter::parseReal( XMLAttentuation->Attribute("range") );
      if( XMLAttentuation->Attribute("constant") )
         constant = StringConverter::parseReal( XMLAttentuation->Attribute("constant") );
      if( XMLAttentuation->Attribute("linear") )
         linear = StringConverter::parseReal( XMLAttentuation->Attribute("linear") );
      if( XMLAttentuation->Attribute("quadratic") )
         quadratic = StringConverter::parseReal( XMLAttentuation->Attribute("quadratic") );
 
      l->setAttenuation( range, constant, linear, quadratic );      
   }
   XMLRange = XMLLight->FirstChildElement("lightRange");
   if( XMLRange ){
	 Real inner, outer, falloff;
	 if( XMLRange->Attribute("inner") )
		 inner = StringConverter::parseReal( XMLRange->Attribute("inner") );
	 if( XMLRange->Attribute("outer") )
		 outer = StringConverter::parseReal( XMLRange->Attribute("outer") );
	 if( XMLRange->Attribute("falloff") )
		 falloff = StringConverter::parseReal( XMLRange->Attribute("falloff") );
	 
	 l->setSpotlightRange(Ogre::Radian(inner), Ogre::Radian(outer),falloff);
   }

   return l;
}

void parseDotScene( const String &SceneName, const String& groupName, SceneManager *mSceneMgr ){
	TiXmlDocument   *XMLDoc;
	TiXmlElement   *XMLRoot, *XMLNodes;

	try{
		DataStreamPtr pStream = ResourceGroupManager::getSingleton().openResource( SceneName, groupName ); 
      	String data = pStream->getAsString();
      	// Open the .scene File
      	XMLDoc = new TiXmlDocument();
      	XMLDoc->Parse( data.c_str() );
      	pStream->close();
      	pStream.setNull(); 
      	if( XMLDoc->Error()){
        	LogManager::getSingleton().logMessage("[dotSceneLoader] The TiXmlDocument reported an error");
         	delete XMLDoc;
         	return;
      }
	}
	catch(...){
     	LogManager::getSingleton().logMessage("[dotSceneLoader] Error creating TiXmlDocument");
     	delete XMLDoc;
     	return;
   }

   // Validate the File
   XMLRoot = XMLDoc->RootElement();
   if( String( XMLRoot->Value()) != "scene"  ){
    	LogManager::getSingleton().logMessage( "[dotSceneLoader]Error: Invalid .scene File. Missing <scene>" );
    	delete XMLDoc;      
    	return;
   } 

   XMLNodes = XMLRoot->FirstChildElement( "nodes" );
   if( XMLNodes ){
   		TiXmlElement *XMLNode, *XMLNodeSon, *XMLPosition, *XMLRotation, *XMLScale,  *XMLEntity, *XMLLight, *XMLUserData, *XMLObjectType/*, *XMLBillboardSet*/; 
    	
    	XMLNode = XMLNodes->FirstChildElement( "node" );

    	while( XMLNode ){ 
			Vector3 TempVecPos, TempVecScal;
    		Quaternion TempQuat;		
			String TempValue, EntityName, EntityMeshFilename;
			bool castShadows = true;
			int typeInt = 0;

        	String NodeName = XMLNode->Attribute("name");

        	XMLPosition = XMLNode->FirstChildElement("position");
        	if( XMLPosition ){
            	TempValue = XMLPosition->Attribute("x");
            	TempVecPos.x = StringConverter::parseReal(TempValue);
            	TempValue = XMLPosition->Attribute("y");
            	TempVecPos.y = StringConverter::parseReal(TempValue);
            	TempValue = XMLPosition->Attribute("z");
            	TempVecPos.z = StringConverter::parseReal(TempValue);
         	}

         	XMLRotation = XMLNode->FirstChildElement("rotation");
         	if( XMLRotation ){            
            	TempValue = XMLRotation->Attribute("qx");
            	TempQuat.x = StringConverter::parseReal(TempValue);
            	TempValue = XMLRotation->Attribute("qy");
            	TempQuat.y = StringConverter::parseReal(TempValue);
            	TempValue = XMLRotation->Attribute("qz");
            	TempQuat.z = StringConverter::parseReal(TempValue);
            	TempValue = XMLRotation->Attribute("qw");
            	TempQuat.w = StringConverter::parseReal(TempValue);
         	}

         	XMLScale = XMLNode->FirstChildElement("scale");
         	if( XMLScale ){
            	TempValue = XMLScale->Attribute("x");
            	TempVecScal.x = StringConverter::parseReal(TempValue);
            	TempValue = XMLScale->Attribute("y");
            	TempVecScal.y = StringConverter::parseReal(TempValue);
            	TempValue = XMLScale->Attribute("z");
            	TempVecScal.z = StringConverter::parseReal(TempValue);
         	}

         	XMLNodeSon = XMLNode->FirstChildElement("node");
         	if( XMLNodeSon ){
         		XMLLight = XMLNodeSon->FirstChildElement( "light" );
         		if( XMLLight ){	
					Ogre::Light* l = LoadLight(XMLLight);
					l->setPosition(TempVecPos);
					//obtener estos valores de algun lugar...?
					l->setDirection(-1, -1, 0);
		 		}
         	}
         	else{
	         	XMLEntity = XMLNode->FirstChildElement("entity");
	         	if( XMLEntity){
	         		EntityName = XMLEntity->Attribute( "name" );
	            	EntityMeshFilename = XMLEntity->Attribute( "meshFile" );

	            	if( XMLEntity->Attribute("castShadows") )
						if( String(XMLEntity->Attribute("castShadows")) == "false" )
							castShadows = false; 
						else
							castShadows = true;
	         	}

	         	XMLUserData = XMLNode->FirstChildElement( "userDataReference" );
	         	if ( XMLUserData )
	        	{   
					TempValue = XMLUserData->Attribute("type");
					typeInt = StringConverter::parseInt(TempValue);
	         	}
				Object* obj = new Object(NodeName, typeInt, EntityMeshFilename);	
				if( typeInt == 3 ){
					 imprimir("establecer ruta");
				}
				obj->m_node->setPosition(TempVecPos);
				obj->m_node->scale(TempVecScal);
				obj->m_node->rotate(TempQuat);			 
				vObjects.push_back(obj);
				obj->m_node->showBoundingBox(true);	
			}
			XMLNode = XMLNode->NextSiblingElement( "node" );
      	}
    }
}



#endif
