/***********************************************************************
created:    31/7/2012
author:     Lukas E Meindl
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2012 Paul D Turner & Thce CEGUI Development Team
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
#include "../include/stdafx.h"

#include "../include/GameMenu.h"
#include "../include/Global.h"
#include "../include/GameApplication.h"
#include "../include/CCegui.h"

#define M_PI 3.14159265358979323846
#define _USE_MATH_DEFINES

GameMenu::GameMenu(void)
{
	initialise();
}

GameMenu::~GameMenu(void)
{
}

bool GameMenu::initialise()
{
    CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

    d_root = winMgr.loadLayoutFromFile("GameMenu.layout");
	d_root->setName("MainWindow/GameMenu");
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(d_root);

    setupWindows();

    setupAnimations();

    d_mouseIsHoveringNavi = false;

	shouldBeDisplayed = true;

    return true;
}


void GameMenu::deinitialise()
{
}

//-----------------------------------------------------------------
//Acciones a realizar al entrar en el menu
//-----------------------------------------------------------------
void GameMenu::onEnteringSample()
{
    d_navigationTravelIcon->setEnabled(false);

    d_botNaviContainer->setAlpha(1.0f);

    d_centerButtonsBlendInInst->setPosition(d_centerButtonsBlendInInst->getDefinition()->getDuration());
    d_centerButtonsBlendInInst->apply();

    d_root->getChild("InnerButtonsContainer/PopupLinesQuit")->setVisible(false);

    resetAnimations();

    //d_startButtonClickArea->setVisible(false);

    //Start the animations
    startEntranceAnimations();

    makeAllSelectionIconsInvisible();
}


//-----------------------------------------------------------------
//Acciones a realizar al pulsar en el boton "start"
//-----------------------------------------------------------------
bool GameMenu::handleInnerPartStartClickAreaClick(const CEGUI::EventArgs& args)
{
    d_startButtonBlendInAnimInst->pause();
    d_startButtonClickArea->setVisible(false);

	//INICIO DEL JUEGO
	//Se crea el escenario inicial
	gGameApp->createScene1();
	//Ocultamos el menu principal y no se muestra mas (en principio)
	d_root->hide();
	shouldBeDisplayed = false;

    return false;
}


bool GameMenu::handleCheckIfNaviIconAnimationNeedsChange(const CEGUI::EventArgs& args)
{

    bool mouseIsHovering = false;
    CEGUI::Window* window;

    window = d_root->getChild("BotNavigationContainer/NaviCenterContainer/NavigationLabel");
    mouseIsHovering |= window->isMouseContainedInArea();
    window = d_root->getChild("BotNavigationContainer/NaviCenterContainer/NavigationIcon");
    mouseIsHovering |= window->isMouseContainedInArea();
    window = d_root->getChild("BotNavigationContainer/NaviCenterContainer/NaviBotSelectionIcon");
    mouseIsHovering |= window->isMouseContainedInArea();

    //We fire an event to trigger the animation depending on if the mouse hovers a critical
    //window or not. Additionally we perform a check to not fire an event for an animation that is already running
    if(d_mouseIsHoveringNavi != mouseIsHovering)
    {
        d_mouseIsHoveringNavi = mouseIsHovering;
        CEGUI::EventArgs args;

        if(mouseIsHovering)
        {
            d_navigationSelectionIcon->fireEvent("StartRotate", args);
            d_navigationTravelIcon->fireEvent("StartRotate", args);
        }
        else
        {
            d_navigationSelectionIcon->fireEvent("StopRotate", args);
            d_navigationTravelIcon->fireEvent("StopRotate", args);
        }
    }

    return false;
}


float getAngle(const CEGUI::Quaternion& quat)
{
    return 2.0f * std::acos(quat.d_w);
}


bool GameMenu::handleNaviSelectionIconAnimStart(const CEGUI::EventArgs& args)
{
    const CEGUI::AnimationEventArgs& animArgs = static_cast<const CEGUI::AnimationEventArgs&>(args);

    CEGUI::AnimationInstance* animInst = animArgs.instance;

    if(animInst->getDefinition() == CEGUI::AnimationManager::getSingleton().getAnimation("LoopRotateRight"))
    {
        CEGUI::Quaternion curRotation = animInst->getTarget()->getProperty<CEGUI::Quaternion>("Rotation");

        float curAngle = getAngle(curRotation);
        float progress = curAngle / (2.0f * static_cast<float>(M_PI));
        animInst->setPosition(progress * animInst->getDefinition()->getDuration());
    }

    return false;
}

bool GameMenu::handleStartPopupLinesQuitDisplay(const CEGUI::EventArgs& args)
{
    makeAllSelectionIconsInvisible();

    stopStartPopupLinesAnimations();
    d_popupLinesQuitAnimInst->start();

    d_root->getChild("InnerButtonsContainer/YesSelectionIcon")->setVisible(true);
    d_root->getChild("InnerButtonsContainer/NoSelectionIcon")->setVisible(true);

    return false;
}


bool GameMenu::handleInnerButtonsLabelEntered(const CEGUI::EventArgs& args)
{
    const CEGUI::MouseEventArgs& mouseArgs = static_cast<const CEGUI::MouseEventArgs&>(args);

    CEGUI::Window* iconWindow = getIconWindowFromLabel(mouseArgs.window);

    CEGUI::EventArgs fireArgs;
    iconWindow->fireEvent("StartRotate", fireArgs);

    return false;
}

bool GameMenu::handleInnerButtonsLabelLeft(const CEGUI::EventArgs& args)
{
    const CEGUI::MouseEventArgs& mouseArgs = static_cast<const CEGUI::MouseEventArgs&>(args);

    CEGUI::Window* iconWindow = getIconWindowFromLabel(mouseArgs.window);
 
    CEGUI::EventArgs fireArgs;
    iconWindow->fireEvent("StopRotate", fireArgs);

    return false;
}


void GameMenu::makeAllSelectionIconsInvisible()
{
    CEGUI::EventArgs fireArgs;

    d_root->getChild("InnerButtonsContainer/NoSelectionIcon")->setVisible(false);
    d_root->getChild("InnerButtonsContainer/YesSelectionIcon")->setVisible(false);
}

void GameMenu::stopStartPopupLinesAnimations()
{
    d_popupLinesQuitAnimInst->setPosition(d_popupLinesQuitAnimInst->getDefinition()->getDuration());
    d_popupLinesQuitAnimInst->apply();
    d_popupLinesQuitAnimInst->pause();

    d_root->getChild("InnerButtonsContainer/PopupLinesQuit")->setVisible(false);

}

//-----------------------------------------------------------------
//Configuracion de las animaciones
//-----------------------------------------------------------------
void GameMenu::setupAnimations()
{
    CEGUI::AnimationManager& animMgr = CEGUI::AnimationManager::getSingleton();
    animMgr.loadAnimationsFromXML("GameMenu.anims");

    CEGUI::Animation* startButtAnim = animMgr.getAnimation("StartButtonPulsating");
    CEGUI::AnimationInstance* startButtAnimInstance = animMgr.instantiateAnimation(startButtAnim);
    CEGUI::Window* startButtWindow = d_root->getChild("InnerPartContainer/InsideStartClickArea/StartButtonImage");
    startButtAnimInstance->setTargetWindow(startButtWindow);
    startButtAnimInstance->start();

    CEGUI::Animation* insideImg1Anim = animMgr.getAnimation("InsideImage1Pulsating");
    CEGUI::AnimationInstance* insideImg1AnimInst = animMgr.instantiateAnimation(insideImg1Anim);
    CEGUI::Window* insideImg1 = d_root->getChild("InnerPartContainer/InsideImage1");
    insideImg1AnimInst->setTargetWindow(insideImg1);
    insideImg1AnimInst->start();

    CEGUI::Animation* topBarAnim = animMgr.getAnimation("TopBarMoveInAnimation");
    d_topBarAnimInst = animMgr.instantiateAnimation(topBarAnim);
    CEGUI::Window* topBarWindow = d_root->getChild("TopBar");
    d_topBarAnimInst->setTargetWindow(topBarWindow);

    CEGUI::Animation* botBarAnim = animMgr.getAnimation("BotBarMoveInAnimation");
    d_botBarAnimInst = animMgr.instantiateAnimation(botBarAnim);
    CEGUI::Window* botBarWindow = d_root->getChild("BotBar");
    d_botBarAnimInst->setTargetWindow(botBarWindow);

    CEGUI::Animation* insideBlendInAnim = animMgr.getAnimation("InsideBlendIn");
    d_insideBlendInAnimInst = animMgr.instantiateAnimation(insideBlendInAnim);
    CEGUI::Window* innerPartContainer = d_root->getChild("InnerPartContainer");
    d_insideBlendInAnimInst->setTargetWindow(innerPartContainer);

    CEGUI::Animation* insideImage3RotateIn = animMgr.getAnimation("InsideImage3RotateIn");
    d_insideImage3RotateInInst = animMgr.instantiateAnimation(insideImage3RotateIn);
    CEGUI::Window* insideImage3 = d_root->getChild("InnerPartContainer/OuterRingsContainer/InsideImage3");
    d_insideImage3RotateInInst->setTargetWindow(insideImage3);

    CEGUI::Animation* insideImage4RotateIn = animMgr.getAnimation("InsideImage4RotateIn");
    d_insideImage4RotateInInst = animMgr.instantiateAnimation(insideImage4RotateIn);
    CEGUI::Window* insideImage4 = d_root->getChild("InnerPartContainer/OuterRingsContainer/InsideImage4");
    d_insideImage4RotateInInst->setTargetWindow(insideImage4);

    CEGUI::Animation* insideImageRingsContainerSizeIn = animMgr.getAnimation("RingsContainerSizeIn");
    d_insideImageRingsContainerSizeInInst = animMgr.instantiateAnimation(insideImageRingsContainerSizeIn);
    CEGUI::Window* insideImageContainer = d_root->getChild("InnerPartContainer/OuterRingsContainer");
    d_insideImageRingsContainerSizeInInst->setTargetWindow(insideImageContainer);

    CEGUI::Animation* buttonFadeInAnim = animMgr.getAnimation("ButtonFadeIn");
    d_buttonFadeInAnimInst1 = animMgr.instantiateAnimation(buttonFadeInAnim);
    
	CEGUI::Window* window = d_root->getChild("InnerButtonsContainer/ButtonQuit");
    d_buttonFadeInAnimInst1->setTargetWindow(window);

    CEGUI::Animation* naviBotMoveInAnim = animMgr.getAnimation("NaviBotCenterMoveIn");
    d_naviBotMoveInInst = animMgr.instantiateAnimation(naviBotMoveInAnim);
    window = d_root->getChild("BotNavigationContainer/NaviCenterContainer");
    d_naviBotMoveInInst->setTargetWindow(window);

    CEGUI::Animation* blendInAnim = animMgr.getAnimation("BlendIn");
    d_startButtonBlendInAnimInst = animMgr.instantiateAnimation(blendInAnim);
    d_startButtonBlendInAnimInst->setTargetWindow(d_startButtonClickArea);

    CEGUI::Animation* partialBlendOutAnim = animMgr.getAnimation("PartialBlendOut");

    window = d_root->getChild("BotNavigationContainer");
    d_naviPartialBlendOutInst = animMgr.instantiateAnimation(partialBlendOutAnim);
    d_naviPartialBlendOutInst->setTargetWindow(window);
    d_naviBlendInInst = animMgr.instantiateAnimation(blendInAnim);
    d_naviBlendInInst->setTargetWindow(window);
    window = d_root->getChild("InnerButtonsContainer");
    d_centerButtonsPartialBlendOutInst = animMgr.instantiateAnimation(partialBlendOutAnim);
    d_centerButtonsPartialBlendOutInst->setTargetWindow(window);
    d_centerButtonsBlendInInst = animMgr.instantiateAnimation(blendInAnim);
    d_centerButtonsBlendInInst->setTargetWindow(window);

    window = d_root->getChild("BotNavigationContainer/NaviCenterContainer/NavigationLabel");
    d_navigationLabelPartialBlendOutInst = animMgr.instantiateAnimation(partialBlendOutAnim);
    d_navigationLabelPartialBlendOutInst->setTargetWindow(window);
    d_navigationLabelBlendInAnimInst = animMgr.instantiateAnimation(blendInAnim);
    d_navigationLabelBlendInAnimInst->setTargetWindow(window);

    CEGUI::Animation* travelRotateInAnim = animMgr.getAnimation("TravelRotateIn");
    CEGUI::AnimationInstance* travelRotateInInst = animMgr.instantiateAnimation(travelRotateInAnim);
    travelRotateInInst->setTargetWindow(d_navigationTravelIcon);
    CEGUI::Animation* travelRotateOutAnim = animMgr.getAnimation("TravelRotateOut");
    CEGUI::AnimationInstance* travelRotateOutInst = animMgr.instantiateAnimation(travelRotateOutAnim);
    travelRotateOutInst->setTargetWindow(d_navigationTravelIcon);

    CEGUI::Animation* loopRotateRightAnim = animMgr.getAnimation("LoopRotateRight");
    CEGUI::Animation* stopRotateAnim = animMgr.getAnimation("StopRotate");
    CEGUI::AnimationInstance* loopRotateRightAnimInst = animMgr.instantiateAnimation(loopRotateRightAnim);
    loopRotateRightAnimInst->setTargetWindow(d_navigationSelectionIcon);
    CEGUI::AnimationInstance* loopRotateLeftAnimInst = animMgr.instantiateAnimation(stopRotateAnim);
    loopRotateLeftAnimInst->setTargetWindow(d_navigationSelectionIcon);

    setupPopupLinesAnimations();

    setupSelectionIconAnimations();
}

void GameMenu::setupWindows()
{
    setupButtonClickHandlers();

    d_botNaviContainer = d_root->getChild("BotNavigationContainer");
    d_botNaviCenter = d_root->getChild("BotNavigationContainer/NaviCenterContainer");

    d_startButtonClickArea = d_root->getChild("InnerPartContainer/InsideStartClickArea");   
    d_startButtonClickArea->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&GameMenu::handleInnerPartStartClickAreaClick, this));

    d_navigationTravelIcon = d_root->getChild("BotNavigationContainer/NaviCenterContainer/NavigationIcon");
    d_navigationSelectionIcon = d_root->getChild("BotNavigationContainer/NaviCenterContainer/NaviBotSelectionIcon");

    d_navigationSelectionIcon->subscribeEvent(CEGUI::AnimationInstance::EventAnimationStarted, CEGUI::Event::Subscriber(&GameMenu::handleNaviSelectionIconAnimStart, this));

    setupNaviIconAnimationEventHandlers();

    setupInnerButtonsSubOptionsLabels();

}

void GameMenu::setupNaviIconAnimationEventHandlers()
{
    CEGUI::Window* window;
    window = d_root->getChild("BotNavigationContainer/NaviCenterContainer/NavigationLabel");
    window->subscribeEvent(CEGUI::Window::EventMouseEntersArea, CEGUI::Event::Subscriber(&GameMenu::handleCheckIfNaviIconAnimationNeedsChange, this));
    window->subscribeEvent(CEGUI::Window::EventMouseLeavesArea, CEGUI::Event::Subscriber(&GameMenu::handleCheckIfNaviIconAnimationNeedsChange, this));
    window = d_root->getChild("BotNavigationContainer/NaviCenterContainer/NavigationIcon");
    window->subscribeEvent(CEGUI::Window::EventMouseEntersArea, CEGUI::Event::Subscriber(&GameMenu::handleCheckIfNaviIconAnimationNeedsChange, this));
    window->subscribeEvent(CEGUI::Window::EventMouseLeavesArea, CEGUI::Event::Subscriber(&GameMenu::handleCheckIfNaviIconAnimationNeedsChange, this));
    window = d_root->getChild("BotNavigationContainer/NaviCenterContainer/NaviBotSelectionIcon");
    window->subscribeEvent(CEGUI::Window::EventMouseEntersArea, CEGUI::Event::Subscriber(&GameMenu::handleCheckIfNaviIconAnimationNeedsChange, this));
    window->subscribeEvent(CEGUI::Window::EventMouseLeavesArea, CEGUI::Event::Subscriber(&GameMenu::handleCheckIfNaviIconAnimationNeedsChange, this));
}

void GameMenu::startEntranceAnimations()
{
    d_topBarAnimInst->start();
    d_botBarAnimInst->start();
    d_insideBlendInAnimInst->start();
    d_insideImage3RotateInInst->start();
    d_insideImage4RotateInInst->start();
    d_insideImageRingsContainerSizeInInst->start();
    d_buttonFadeInAnimInst1->start();
    d_naviBotMoveInInst->start();
}

void GameMenu::resetAnimations()
{
    //Set animations to end and apply their changes to the windows
    //For every animation instance that uses source properties
    //so the values will be taken correctly after starting them again
    d_buttonFadeInAnimInst1->setPosition(d_buttonFadeInAnimInst1->getDefinition()->getDuration());
    d_buttonFadeInAnimInst1->apply();
    d_naviBotMoveInInst->setPosition(d_naviBotMoveInInst->getDefinition()->getDuration());
    d_naviBotMoveInInst->apply();
}

void GameMenu::setupButtonClickHandlers()
{
    CEGUI::Window* buttonQuit = d_root->getChild("InnerButtonsContainer/ButtonQuit");
    buttonQuit->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&GameMenu::handleStartPopupLinesQuitDisplay, this));
}

void GameMenu::setupInnerButtonsSubOptionsLabels()
{
    CEGUI::Window* label;
    label = d_root->getChild("InnerButtonsContainer/PopupLinesQuit/LabelYes");
    label->subscribeEvent(CEGUI::Window::EventMouseEntersArea, CEGUI::Event::Subscriber(&GameMenu::handleInnerButtonsLabelEntered, this));
    label->subscribeEvent(CEGUI::Window::EventMouseLeavesArea, CEGUI::Event::Subscriber(&GameMenu::handleInnerButtonsLabelLeft, this));
    label = d_root->getChild("InnerButtonsContainer/PopupLinesQuit/LabelNo");
    label->subscribeEvent(CEGUI::Window::EventMouseEntersArea, CEGUI::Event::Subscriber(&GameMenu::handleInnerButtonsLabelEntered, this));
    label->subscribeEvent(CEGUI::Window::EventMouseLeavesArea, CEGUI::Event::Subscriber(&GameMenu::handleInnerButtonsLabelLeft, this));
}

CEGUI::Window* GameMenu::getIconWindowFromLabel(CEGUI::Window* window)
{
    const CEGUI::String windowPrefix = "Label";
    const CEGUI::String iconPostfix = "SelectionIcon";

    CEGUI::String windowName = window->getName();

    windowName = windowName.substr(windowPrefix.length());
    return window->getParent()->getParent()->getChild(windowName + iconPostfix);
}

void GameMenu::setupPopupLinesAnimations()
{
    CEGUI::Window* window;
    CEGUI::AnimationManager& animMgr = CEGUI::AnimationManager::getSingleton();

    CEGUI::Animation* sizeGrowth = animMgr.getAnimation("SizeGrowth");

    window = d_root->getChild("InnerButtonsContainer/PopupLinesQuit");
    d_popupLinesQuitAnimInst = animMgr.instantiateAnimation(sizeGrowth);
    d_popupLinesQuitAnimInst->setTarget(window);
}

void GameMenu::setupSelectionIconAnimations()
{
    CEGUI::Window* window;
    CEGUI::AnimationManager& animMgr = CEGUI::AnimationManager::getSingleton();

    CEGUI::Animation* iconAnimationLoop = animMgr.getAnimation("LoopRotateRight");
    CEGUI::Animation* iconAnimationStop = animMgr.getAnimation("StopRotate");
    CEGUI::AnimationInstance* iconAnimInst;

    window = d_root->getChild("InnerButtonsContainer/NoSelectionIcon");
    iconAnimInst = animMgr.instantiateAnimation(iconAnimationLoop);
    iconAnimInst->setTargetWindow(window);
    iconAnimInst = animMgr.instantiateAnimation(iconAnimationStop);
    iconAnimInst->setTargetWindow(window);
    window = d_root->getChild("InnerButtonsContainer/YesSelectionIcon");
    iconAnimInst = animMgr.instantiateAnimation(iconAnimationLoop);
    iconAnimInst->setTargetWindow(window);
    iconAnimInst = animMgr.instantiateAnimation(iconAnimationStop);
    iconAnimInst->setTargetWindow(window);
    
}