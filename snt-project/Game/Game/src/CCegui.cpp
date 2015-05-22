#include "../include/stdafx.h"

#include "../include/CCegui.h"
#include "../include/Global.h"
#include "../include/Player.h"

CCegui::CCegui(void)
{
	//Se inicializa CEGUI
	mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
	//Se actualizan los resourceGroup (definidos en resources.cfg)
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes"); //Se encarga de la apariencia de los menus
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

	//Por ahora, utilizamos una "skin" (apariencia) por defecto tanto para el menu como para el raton
	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
	CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

	menu1();
}

CCegui::~CCegui(void)
{
}

CEGUI::MouseButton CCegui::convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;
 
    case OIS::MB_Right:
        return CEGUI::RightButton;
 
    case OIS::MB_Middle:
        return CEGUI::MiddleButton;

    default:
        return CEGUI::LeftButton;
    }
}

bool CCegui::quit(const CEGUI::EventArgs &e)
{
	gShutDown = true;
    return true;
}

void CCegui::menu1()
{
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
	CEGUI::Window *quit = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
	quit->setText("Quit");
	quit->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
	sheet->addChild(quit);
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
	quit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CCegui::quit, this));
}


//CEGUI::OgreRenderer::bootstrapSystem().setDisplaySize(CEGUI::Sizef(width, height));