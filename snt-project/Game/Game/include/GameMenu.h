/***********************************************************************
created:    31/7/2012
author:     Lukas E Meindl
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
*
*   Permission is hereby granted, free of charge, to any person obtaining
*   a copy of this software and associated documentation files (the
*   "Software"), to deal in the Software without restriction, including
*   without limitation the rights to use, copy, modify, merge, publish,
*   distribute, sublicense, and/or sell copies of the Software, and to
*   permit persons to whom the Software is furnished to do so, subject to
*   the following conditions:
*
*   The above copyright notice and this permission notice shall be
*   included in all copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
*   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
*   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
*   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
*   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
*   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
*   OTHER DEALINGS IN THE SOFTWARE.
***************************************************************************/
#ifndef _GameMenu_
#define _GameMenu_

// Sample class
class GameMenu
{
public:
	GameMenu(void);
	virtual ~GameMenu(void);

    // method to initialse the samples windows and events.
    virtual bool initialise();

    // method to perform any required cleanup operations.
    virtual void deinitialise();

    virtual void onEnteringSample();

//protected:
    void setupWindows();

    void setupInnerButtonsSubOptionsLabels();
    void setupButtonClickHandlers();
    void setupNaviIconAnimationEventHandlers();
    void setupAnimations();

    void setupSelectionIconAnimations();
    void setupPopupLinesAnimations();
    void resetAnimations();
    void startEntranceAnimations();

    bool handleInnerPartStartClickAreaClick(const CEGUI::EventArgs& args);
    bool handleCheckIfNaviIconAnimationNeedsChange(const CEGUI::EventArgs& args);
    bool handleNaviSelectionIconAnimStart(const CEGUI::EventArgs& args);

    bool handleStartPopupLinesLoadDisplay(const CEGUI::EventArgs& args);
    bool handleStartPopupLinesOptionsDisplay(const CEGUI::EventArgs& args);
    bool handleStartPopupLinesQuitDisplay(const CEGUI::EventArgs& args);

    bool handleInnerButtonsLabelEntered(const CEGUI::EventArgs& args);

    CEGUI::Window* getIconWindowFromLabel(CEGUI::Window* window);
    bool handleInnerButtonsLabelLeft(const CEGUI::EventArgs& args);

    void makeAllSelectionIconsInvisible();
    void stopStartPopupLinesAnimations();

    CEGUI::Window* d_root;

    bool d_mouseIsHoveringNavi;
    bool d_startButtonClicked;

    CEGUI::AnimationInstance* d_topBarAnimInst;
    CEGUI::AnimationInstance* d_botBarAnimInst;
    CEGUI::AnimationInstance* d_insideBlendInAnimInst;
    CEGUI::AnimationInstance* d_insideImage3RotateInInst;
    CEGUI::AnimationInstance* d_insideImage4RotateInInst;
    CEGUI::AnimationInstance* d_insideImageRingsContainerSizeInInst;

    CEGUI::AnimationInstance* d_buttonFadeInAnimInst1;
    CEGUI::AnimationInstance* d_buttonFadeInAnimInst2;
    CEGUI::AnimationInstance* d_buttonFadeInAnimInst3;

    CEGUI::AnimationInstance* d_naviBotMoveInInst;
    CEGUI::AnimationInstance* d_startButtonBlendInAnimInst;
    CEGUI::AnimationInstance* d_navigationLabelBlendInAnimInst;
    CEGUI::AnimationInstance* d_navigationLabelPartialBlendOutInst;

    CEGUI::AnimationInstance* d_naviPartialBlendOutInst;
    CEGUI::AnimationInstance* d_naviBlendInInst;
    CEGUI::AnimationInstance* d_centerButtonsPartialBlendOutInst;
    CEGUI::AnimationInstance* d_centerButtonsBlendInInst;

    CEGUI::AnimationInstance* d_popupLinesLoadAnimInst;
    CEGUI::AnimationInstance* d_popupLinesOptionsAnimInst;
    CEGUI::AnimationInstance* d_popupLinesQuitAnimInst;

    CEGUI::Window* d_botNaviCenter;
    CEGUI::Window* d_startButtonClickArea;
    CEGUI::Window* d_navigationTravelIcon;
    CEGUI::Window* d_navigationSelectionIcon;
    CEGUI::Window* d_botNaviContainer;
};

#endif