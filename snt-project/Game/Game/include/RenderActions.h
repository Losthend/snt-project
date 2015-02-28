#ifndef __Main_h_
#define __Main_h_

//Se añade herencia para que puedan utilizarse posteriormente ciertos metodos
class RenderActions : public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener, OgreBites::SdkTrayListener
{
//Aqui se establecen las declaraciones de metodos y variables
public:
	//Constructor y destructor 
    RenderActions(void);
    virtual ~RenderActions(void);

	//Controladores de teclado y raton mediante listeners
	virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);
    virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	
private:
	//METODOS 
	//Es llamado antes de renderizar un frame para realizar acciones
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    //OgreBites
	OgreBites::SdkTrayManager*	mTrayMgr;
    OgreBites::InputContext     mInputContext;
    OgreBites::ParamsPanel*     mDetailsPanel;   	// Panel de ejemplo (futura barra de vida, estadisticas, etc?)
    bool                        mCursorWasVisible;	// Si el cursor es visible o no
};

//---------------------------------------------------------------------------

#endif // #ifndef __Main_h_

//---------------------------------------------------------------------------