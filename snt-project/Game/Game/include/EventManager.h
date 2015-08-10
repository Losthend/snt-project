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
	void controlText2();
	void controlText3();
	void controlText4();
	Ogre::String selectText1(int textNumber);
	Ogre::String selectText2(int textNumber);
	Ogre::String selectText3(int textNumber);
	Ogre::String selectText4(int textNumber);
	
	//General
	bool pulseQ;
	int actualText;
	bool nextText;
	//Escenario 1
	bool text_1_7;
	bool text_8_9;
	bool text_10;
	bool text_11;
	bool text_12_13;
	//Escenario 2
	bool text_1_3;
	bool text_4_5;
	bool text_6_7;
	//Escenario 3
	int inExitCount;
	bool inExit;
	bool text_1_4;
	bool text_5;
	bool text_6_7_8;
	bool magic;
	//Escenario 4
	bool text_1_14;
};

//---------------------------------------------------------------------------

#endif // #ifndef __EventManager_h_

//---------------------------------------------------------------------------
