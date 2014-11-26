#ifndef __GameApplication_h_
#define __GameApplication_h_

#include "stdafx.h"
#include "BaseApplication.h"

//---------------------------------------------------------------------------

class GameApplication : public BaseApplication
{
	BaseApplication app;
public:
	//Constructor/destructor
    GameApplication(void);
    virtual ~GameApplication(void);
	//Metodos publicos
    void createScene(void);
	BaseApplication getBaseApplication(void);
};

//---------------------------------------------------------------------------

#endif // #ifndef __GameApplication_h_

//---------------------------------------------------------------------------
