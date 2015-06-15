#ifndef __EventManager_h_
#define __EventManager_h_

class EventManager
{
public:
	EventManager(void);
	virtual ~EventManager(void);

	void handleEvent(void);
	void doScene3(void);

	int inExitCount;
	bool inExit;
};

//---------------------------------------------------------------------------

#endif // #ifndef __EventManager_h_

//---------------------------------------------------------------------------
