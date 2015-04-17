#include "../include/stdafx.h"

#include "../include/KeyboardMouse.h"
#include "../include/Global.h"
#include "../include/Player.h"
#include "../include/SceneObject.h"

KeyboardMouse::KeyboardMouse(void)
{
	//Creacion del frame listener para teclado/raton
    gMouse->setEventCallback(this);
    gKeyboard->setEventCallback(this);
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
	switch (arg.key)
	{
		case OIS::KC_ESCAPE:
			gShutDown = true;
			break;
		case OIS::KC_A:
			gPlayer->m_direction.x = -1;
			break;
		case OIS::KC_D:
			gPlayer->m_direction.x = 1;
			break;
		case OIS::KC_S:
			if (gPlayer->m_run == false)
			{
				gPlayer->m_crouchDown = true;
				gPlayer->m_Speed = gPlayer->m_Speed / 2;
			}
			break;
		case OIS::KC_LSHIFT:
			if (gPlayer->m_crouchDown == false)
			{
				gPlayer->m_run = true;
				gPlayer->m_Speed = gPlayer->m_Speed * 2;
			}
			break;
		case OIS::KC_SPACE:
			if (gPlayer->m_jumpCount < gPlayer->m_maxNumJump)
			{
				gPlayer->m_jump = true;
				gPlayer->m_direction.y = 1;
			}
			break;
	}

    return true;
}
//---------------------------------------------------------------------------
bool KeyboardMouse::keyReleased(const OIS::KeyEvent &arg)
{
	switch (arg.key)
	{
		case OIS::KC_A:
			gPlayer->m_direction.x = 0;
			break;
		case OIS::KC_D:
			gPlayer->m_direction.x = 0;
			break;
		case OIS::KC_S:
			if (gPlayer->m_crouchDown == true)
			{
				gPlayer->m_crouchDown = false;
				gPlayer->m_Speed = gPlayer->m_Speed * 2;
			}
		case OIS::KC_LSHIFT:
			if (gPlayer->m_run == true)
			{
				gPlayer->m_run = false;
				gPlayer->m_Speed = gPlayer->m_Speed / 2;
			}
			break;
	}

    return true;
}
//---------------------------------------------------------------------------
bool KeyboardMouse::mouseMoved(const OIS::MouseEvent &arg)
{
    return true;
}
//---------------------------------------------------------------------------
bool KeyboardMouse::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    return true;
}
//---------------------------------------------------------------------------
bool KeyboardMouse::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	return true;
}