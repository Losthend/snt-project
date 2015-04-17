#include "../include/stdafx.h"

#include "../include/SdkTrayMgr.h"
#include "../include/Global.h"

SdkTrayMgr::SdkTrayMgr(void)
{
	mInputContext.mKeyboard = gKeyboard;
    mInputContext.mMouse = gMouse;

    mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", gWindow, mInputContext, this);
    mTrayMgr->showFrameStats(OgreBites::TL_TOPLEFT);
    mTrayMgr->hideCursor();

    // Create a params panel for displaying sample details
    Ogre::StringVector items;
    items.push_back("cam.pX");
    items.push_back("cam.pY");
    items.push_back("cam.pZ");
    items.push_back("");
    items.push_back("cam.oW");
    items.push_back("cam.oX");
    items.push_back("cam.oY");
    items.push_back("cam.oZ");
    items.push_back("");
    items.push_back("Filtering");
    items.push_back("Poly Mode");

    mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
    mDetailsPanel->setParamValue(9, "Bilinear");
    mDetailsPanel->setParamValue(10, "Solid");
    mDetailsPanel->hide();
}

SdkTrayMgr::~SdkTrayMgr(void)
{
	if (mTrayMgr) delete mTrayMgr;
}


void SdkTrayMgr::update(const Ogre::FrameEvent& evt)
{
	//Visualizacion de datos en el menu
	mTrayMgr->frameRenderingQueued(evt);
    if (!mTrayMgr->isDialogVisible())
    {
        if (mDetailsPanel->isVisible())          // If details panel is visible, then update its contents
        {
            mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(gCamera->getDerivedPosition().x));
            mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(gCamera->getDerivedPosition().y));
            mDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(gCamera->getDerivedPosition().z));
            mDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(gCamera->getDerivedOrientation().w));
            mDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(gCamera->getDerivedOrientation().x));
            mDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(gCamera->getDerivedOrientation().y));
            mDetailsPanel->setParamValue(7, Ogre::StringConverter::toString(gCamera->getDerivedOrientation().z));
        }
    }
}
