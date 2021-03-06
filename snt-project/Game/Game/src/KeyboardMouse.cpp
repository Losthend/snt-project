#include "../include/stdafx.h"

#include "../include/KeyboardMouse.h"
#include "../include/Global.h"
#include "../include/Player.h"
#include "../include/Object.h"
#include "../include/SceneObject.h"
#include "../include/PhysicsManager.h"
#include "../include/CCegui.h"
#include "../include/GameMenu.h"
#include "../include/AnimationManager.h"
#include "../include/EventManager.h"
#include "../include/GameApplication.h"

KeyboardMouse::KeyboardMouse(void)
{
	//Creacion del frame listener para teclado/raton
    gMouse->setEventCallback(this);
    gKeyboard->setEventCallback(this);

	//Creaci�n de CEGUI
	gCCegui = new CCegui();
}

KeyboardMouse::~KeyboardMouse(void)
{
}

//--------------------------------------------------------------------------
//Controladores de teclado y raton
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
bool KeyboardMouse::keyPressed( const OIS::KeyEvent &arg )
{
	//Injectar eventos de teclado en CEGUI
	CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	context.injectKeyDown((CEGUI::Key::Scan)arg.key);
	context.injectChar((CEGUI::Key::Scan)arg.text);

	//Basicos
	switch (arg.key)
	{
	case OIS::KC_ESCAPE:
		gShutDown = true;
		break;
	case OIS::KC_RETURN:
		if (gCCegui->dialogBox->isActive())
			gEventMgr->nextText = true;
		break;
	case OIS::KC_Q:
		if (gCCegui->alertBox->isActive())
			gEventMgr->pulseQ = true;
		break;
	}

	//Jugador
	if(gCanUpdate && gPlayer != 0 && gCanUpdateKeyboard)
	{
		switch (arg.key)
		{
			case OIS::KC_A:
				if (gPlayer->m_direction.x == 0)
					gPlayer->m_direction.x = -1;
				break;
			case OIS::KC_D:
				if (gPlayer->m_direction.x == 0)
					gPlayer->m_direction.x = 1;
				break;
			case OIS::KC_E:
				gPlayer->m_SwordsDrawn = true;
				break;
			case OIS::KC_LSHIFT:
					gPlayer->m_run = true;
					gPlayer->m_moveX = gPlayer->m_moveX * 2;
				break;
			case OIS::KC_SPACE:
				if (!(gPlayer->m_jump) && !(gPlayer->m_fall))
					gPlayer->m_jump = true;
				break;
			case OIS::KC_C:
				if (gGameApp->activeScene == 3){
					if(!gEventMgr->magic){
						gSceneMgr->getSceneNode("templeBlood01")->setVisible(false);
						gSceneMgr->getSceneNode("templeBlood02")->setVisible(true);
					}
					OIS::MouseState ms = gMouse->getMouseState();
					Ogre::Ray ray = gCamera->getCameraToViewportRay(ms.X.abs/(float)gWindow->getWidth(),ms.Y.abs/(float)gWindow->getHeight()); 
					Ogre::Vector3 mouseCoord = ray.getPoint(gCamera->getPosition().z);
					gPhysics->magicGenerator(mouseCoord);
					gEventMgr->magic = true;
				}
				break;
		}
	}

    return true;
}
//---------------------------------------------------------------------------
bool KeyboardMouse::keyReleased(const OIS::KeyEvent &arg)
{
	//Injectar eventos de teclado en CEGUI
	CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)arg.key);

	if(gPlayer != 0)
	{
		switch (arg.key)
		{
			case OIS::KC_A:
				if (gPlayer->m_direction.x == -1)
					gPlayer->m_direction.x = 0;
				break;
			case OIS::KC_D:
				if (gPlayer->m_direction.x == 1)
					gPlayer->m_direction.x = 0;
				break;
			case OIS::KC_LSHIFT:
				if (gPlayer->m_run == true)
				{
					gPlayer->m_run = false;
					gPlayer->m_moveX = gPlayer->m_moveX / 2;
				}
				break;
		}
	}

    return true;
}
//---------------------------------------------------------------------------
bool KeyboardMouse::mouseMoved(const OIS::MouseEvent &arg)
{
	//Configuraciones del raton (uni�n CEGUI/OIS/Sistema)

	static int iOSCursorVisible = 0;
	static bool bCEGUICursorVisible = true;
	
	if ((unsigned int)arg.state.X.abs == gWindow->getWidth() || 
		(unsigned int)arg.state.Y.abs == gWindow->getHeight() ||
		arg.state.X.abs == 0 || 
		arg.state.Y.abs == 0)
	{
		//Cursor fuera de la aplicaci�n: ocultar cursor de CEGUI y mostrar cursor de sistema
		while (iOSCursorVisible < 0)
		{
			iOSCursorVisible = ShowCursor(true);
		}
		CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseLeaves();
		if (bCEGUICursorVisible)
		{
		CEGUI::System::getSingleton().setMutedState(true);
		CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
		bCEGUICursorVisible = false;
		}
	}
	else
	{
		//Cursor dentro de la aplicaci�n: mostrar cursor de CEGUI y ocultar cursor de sistema
		while (iOSCursorVisible > -1)
		{
			iOSCursorVisible = ShowCursor(false);
		}
		if (!bCEGUICursorVisible)
		{
			CEGUI::System::getSingleton().setMutedState(false);
			CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
			bCEGUICursorVisible = true;
		}
	}

	//Inyectar el movimiento del raton a CEGUI segun OIS
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(float(arg.state.X.abs), float(arg.state.Y.abs));
	
	//Rueda de scroll
	//if (arg.state.Z.rel)
	//	sys.getDefaultGUIContext().injectMouseWheelChange(arg.state.Z.rel / 120.0f);

	if (arg.state.Z.rel != 0)
	{
		Ogre::Vector3 pos = gCamera->getPosition();
		if(pos.z >= 200 && pos.z <= 450)
			pos.z = Ogre::Real(pos.z - arg.state.Z.rel*0.05);
		else if(pos.z < 200)
			pos.z = 200;
		else if(pos.z > 450)
			pos.z = 450;
		gCamera->setPosition(pos);
	}

	return true;
}

//---------------------------------------------------------------------------
bool KeyboardMouse::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	//Injectar eventos de raton en CEGUI
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(gCCegui->convertButton(id));

	if (gPlayer != 0 && id == OIS::MB_Right)
	{
		Object* obj = 0;
	
		//Raycasting
		OIS::MouseState ms = gMouse->getMouseState();
		Ogre::Ray ray = gCamera->getCameraToViewportRay(ms.X.abs/(float)gWindow->getWidth(),ms.Y.abs/(float)gWindow->getHeight()); 
		Ogre::RaySceneQuery* mRaySceneQuery = gSceneMgr->createRayQuery(ray); 
		mRaySceneQuery->setRay(ray);
		mRaySceneQuery->setSortByDistance(true);
		Ogre::RaySceneQueryResult& result = mRaySceneQuery->execute();
		Ogre::RaySceneQueryResult::iterator itr = result.begin();

		//Busqueda del objeto
		while(itr != result.end() && obj == 0)
		{
			for(unsigned x = 0; x < gObjects.size(); x++)
				if(itr->movable->getParentNode()->getName() == gObjects[x]->m_sceneObject->mNode->getName())
					obj = gObjects[x];
			itr++;
		}

		//Comprobacion del objeto / agarre
		if(obj!=0 && obj->m_objType == 2)
		{
			gPlayer->m_catchObj = obj;
			gPlayer->m_catchObj->m_sceneObject->mRigidBody->setGravity(btVector3(0,0,0));
		}
	}

	//Ataques: lanzar objeto agarrado o golpes de espada
	if (gPlayer != 0 && id == OIS::MB_Left)
	{
		if (gPlayer->m_catchObj != 0)
			gPlayer->catchAttack();
		else
			gPlayer->m_attack = true;
	}

	return true;
}
//---------------------------------------------------------------------------
bool KeyboardMouse::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	//Injectar eventos de raton en CEGUI
	CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(gCCegui->convertButton(id));

	if (gPlayer != 0 && id == OIS::MB_Right && gPlayer->m_catchObj != 0)
	{
		//Devuelve el efecto de la gravedad al objeto
		gPlayer->m_catchObj->m_sceneObject->mRigidBody->setGravity(gPhysics->mWorld->getGravity());
		//Suelta el objeto
		gPlayer->m_catchObj = 0;
	}

	if (gPlayer != 0 && id == OIS::MB_Left)
			gPlayer->m_attack = false;

	return true;
}