#ifndef __EventManager_h_
#define __EventManager_h_

class EventManager
{
public:
	EventManager(void);
	virtual ~EventManager(void);

	void handleEvent(void);
	void doScene3(void);

	void controlText1();
	Ogre::String selectText1(int textNumber);

	Ogre::String selectText2(int textNumber);

	//Escenario 1
	int actualText;
	bool nextText;
	bool text_1_7;
	bool text_8_9;
	bool text_10;
	bool text_11;
	bool text_12_13;
	//Escenario 3
	int inExitCount;
	bool inExit;
};

//---------------------------------------------------------------------------

#endif // #ifndef __EventManager_h_

//---------------------------------------------------------------------------
