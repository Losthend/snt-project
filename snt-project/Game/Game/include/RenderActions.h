#ifndef __Main_h_
#define __Main_h_

#include "stdafx.h"

//Se añade herencia para que puedan utilizarse posteriormente ciertos metodos
class RenderActions : public Ogre::WindowEventListener, public Ogre::FrameListener
{
//Aqui se establecen las declaraciones de metodos y variables
public:
	//Constructor y destructor 
    RenderActions(void);
    virtual ~RenderActions(void);
	
private:
	//METODOS 
	//Es llamado antes de renderizar un frame para realizar acciones
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
};

//---------------------------------------------------------------------------

#endif // #ifndef __Main_h_

//---------------------------------------------------------------------------