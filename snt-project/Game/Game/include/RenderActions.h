#ifndef __Main_h_
#define __Main_h_

class EventManager;

class RenderActions : public Ogre::FrameListener, public Ogre::WindowEventListener
{
public:
    RenderActions(void);
    virtual ~RenderActions(void);
	
private:
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
};

//---------------------------------------------------------------------------

#endif // #ifndef __Main_h_

//---------------------------------------------------------------------------