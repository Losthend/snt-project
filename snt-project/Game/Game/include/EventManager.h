#ifndef __EventManager_h_
#define __EventManager_h_

class EventManager
{
public:
	EventManager(void);
	virtual ~EventManager(void);

	void handleEvent(void);

	bool platform;
};

//---------------------------------------------------------------------------

#endif // #ifndef __EventManager_h_

//---------------------------------------------------------------------------
