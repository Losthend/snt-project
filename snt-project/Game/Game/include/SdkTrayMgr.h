#ifndef __SdkTrayMgr_h_
#define __SdkTrayMgr_h_

class SdkTrayMgr: OgreBites::SdkTrayListener
{
public:
	SdkTrayMgr(void);
	virtual ~SdkTrayMgr(void);
	void update(const Ogre::FrameEvent& evt);

	//OgreBites
	OgreBites::SdkTrayManager*	mTrayMgr;
    OgreBites::InputContext     mInputContext;
    OgreBites::ParamsPanel*     mDetailsPanel;   	// Panel de ejemplo (futura barra de vida, estadisticas, etc?)
    bool                        mCursorWasVisible;	// Si el cursor es visible o no
};

//---------------------------------------------------------------------------

#endif // #ifndef __SdkTrayMgr_h_

//---------------------------------------------------------------------------