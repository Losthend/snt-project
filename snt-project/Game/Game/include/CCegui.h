#ifndef __CCegui_h_
#define __CCegui_h_

class GameMenu;

class CCegui
{
public:
	CCegui(void);
	virtual ~CCegui(void);
	CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID);

	bool quit(const CEGUI::EventArgs &e);
	void initLoads();
	void loadWindows();

	CEGUI::OgreRenderer* mRenderer;

	GameMenu* gameMenu;
};

//---------------------------------------------------------------------------

#endif // #ifndef __CCegui_h_

//---------------------------------------------------------------------------
