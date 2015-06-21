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

	//DialogBox
	CEGUI::SchemeManager::getSingleton().createFromFile("VanillaCommonDialogs.scheme");
	CEGUI::SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");
	
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

	//Dialog box
	gameDialogBox();

	//Menu de aviso de interacción
	alertBox = wmgr.createWindow("Vanilla/StaticText", "MainWindow/AlertBox");
	alertBox->setSize(CEGUI::USize(CEGUI::UDim(0.03f, 0), CEGUI::UDim(0.05f, 0)));
	alertBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5f, 0),CEGUI::UDim(0.45f, 0)));
	alertBox->setDisabled(true);
	alertBox->setText("E");
	wRoot->addChild(alertBox);

	//Por defecto, todas las ventanas ocultas
	size_t numChild = wRoot->getChildCount();
	for (unsigned i = 0; i < numChild; i++)
	{
		wRoot->getChildAtIdx(i)->hide();
	}
}

void CCegui::gameDialogBox()
{
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();

	dialogBox = wmgr.createWindow("Vanilla/FrameWindow", "MainWindow/DialogBox");
	dialogBox->setSize(CEGUI::USize(CEGUI::UDim(0.75f, 0), CEGUI::UDim(0.18f, 0)));
	dialogBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15f, 0),CEGUI::UDim(0.75f, 0)));
	dialogBox->setDisabled(true);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(dialogBox);
	
	CEGUI::Window* titlebar = wmgr.createWindow("Vanilla/Titlebar");
    titlebar->setSize(CEGUI::USize(CEGUI::UDim(1, 0.0), CEGUI::UDim(0.1f, 0)));
	titlebar->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0),CEGUI::UDim(0.10f, 0)));
	titlebar->setText("SUJETO 123");

	CEGUI::Window* textLabel = wmgr.createWindow("Vanilla/StaticText");
    textLabel->setSize(CEGUI::USize(CEGUI::UDim(1, 0.0), CEGUI::UDim(0.75f, 0)));
	textLabel->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0),CEGUI::UDim(0, 0)));
	textLabel->setText("");

	CEGUI::Window* button1 = wmgr.createWindow("Vanilla/Button");
    button1->setSize(CEGUI::USize(CEGUI::UDim(0.3f, 0.0), CEGUI::UDim(0.2f, 0)));
	button1->setPosition(CEGUI::UVector2(CEGUI::UDim(0.70f, 0),CEGUI::UDim(0.80f, 0)));
	button1->setText("Siguiente (ENTER)");
	//button1->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&CCegui::handleNext,this));
	
	//El orden en el que se añaden importa! De variarlo, realizar las modificaciones correspondientes en EventManager.cpp
	//Posicion 0 (dialog box), posicion 1 (otro), titlebar, textlabel, button1 
	dialogBox->addChild(titlebar);
	dialogBox->addChild(textLabel);
	dialogBox->addChild(button1);
}

bool CCegui::handleNext(const CEGUI::EventArgs &e)
{
	gShutDown = true;
    return true;
}

//CEGUI::OgreRenderer::bootstrapSystem().setDisplaySize(CEGUI::Sizef(width, height));