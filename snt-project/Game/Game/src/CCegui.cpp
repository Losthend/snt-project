#include "../include/stdafx.h"

#include "../include/CCegui.h"
#include "../include/Global.h"
#include "../include/Player.h"
#include "../include/GameMenu.h"

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

	//Carga inicial de los menus
	initLoads();	
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

//-----------------------------------------------------------------
//Carga inicial de los recursos necesarios para que funcionen los menus (loadWindows, ventanas)
//-----------------------------------------------------------------
void CCegui::initLoads()
{
	//Menu principal
    CEGUI::SchemeManager::getSingleton().createFromFile("GameMenu.scheme");
    CEGUI::SchemeManager::getSingleton().createFromFile("Generic.scheme");
	//CEGUI::FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("GameMenuImages/MouseCursor");
    CEGUI::Font& defaultFont = CEGUI::FontManager::getSingleton().createFromFile("Jura-13.font");
    CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont(&defaultFont);

	//Menu 1
	CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");

	//Cargar ventanas
	loadWindows();
}

//-----------------------------------------------------------------
//Se crean las ventanas, ocultas, para que esten disponibles posteriormente desde el root
//-----------------------------------------------------------------
void CCegui::loadWindows()
{
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	//Creación de "root window"
	CEGUI::Window *wRoot = wmgr.createWindow("DefaultWindow", "MainWindow");
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(wRoot);
	
	//Menu principal
	gameMenu = new GameMenu();

	//Menu 1
	menu1 = wmgr.createWindow("TaharezLook/Button", "MainWindow/Menu1");
	menu1->setText("Quit");
	menu1->setSize(CEGUI::USize(CEGUI::UDim(float(0.15), 0), CEGUI::UDim(float(0.05), 0)));
	wRoot->addChild(menu1);
	menu1->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CCegui::quit, this));

	//Por defecto, todas las ventanas ocultas
	size_t numChild = wRoot->getChildCount();
	for (unsigned i = 0; i < numChild; i++)
	{
		wRoot->getChildAtIdx(i)->hide();
	}
}

//CEGUI::OgreRenderer::bootstrapSystem().setDisplaySize(CEGUI::Sizef(width, height));