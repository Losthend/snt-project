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

	//Otros metodos
    void createScene(void);

	//Objeto de la clase de fisicas
	PhysicsManager* mPhysics;

};

//---------------------------------------------------------------------------

#endif // #ifndef __GameApplication_h_

//---------------------------------------------------------------------------
