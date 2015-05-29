#ifndef __GameApplication_h_
#define __GameApplication_h_

//---------------------------------------------------------------------------
class PhysicsManager;

class GameApplication
{
public:
	//Constructor/destructor
    GameApplication(void);
    virtual ~GameApplication(void);

	//Escenarios disponibles
    void createScene(void);
};

//---------------------------------------------------------------------------

#endif // #ifndef __GameApplication_h_

//---------------------------------------------------------------------------
