#ifndef __CCegui_h_
#define __CCegui_h_

class GameMenu;

class CCegui
{
public:
	CCegui(void);
	virtual ~CCegui(void);
	CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID);

	void initLoads();
	void loadWindows();
	void gameDialogBox();
	void gameDeadBox();

	bool handleExit(const CEGUI::EventArgs &e);
	bool handleReload(const CEGUI::EventArgs &e);

	CEGUI::OgreRenderer* mRenderer;

	GameMenu* gameMenu;
	CEGUI::Window* dialogBox;
	CEGUI::Window* alertBox;
	CEGUI::Window* deadBox;
};

//---------------------------------------------------------------------------

#endif // #ifndef __CCegui_h_

//---------------------------------------------------------------------------
