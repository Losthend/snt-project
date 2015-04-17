#ifndef __KeyboardMouse_h_
#define __KeyboardMouse_h_

class KeyboardMouse: public OIS::KeyListener, public OIS::MouseListener
{
public:
	KeyboardMouse(void);
	virtual ~KeyboardMouse(void);

	//Controladores de teclado y raton mediante listeners
	virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);
    virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
};

//---------------------------------------------------------------------------

#endif // #ifndef __KeyboardMouse_h_

//---------------------------------------------------------------------------