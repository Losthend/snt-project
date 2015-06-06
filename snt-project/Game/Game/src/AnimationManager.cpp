#include "../include/stdafx.h"

#include "../include/AnimationManager.h"
#include "../include/Global.h"
#include "../include/Player.h"
#include "../include/SceneObject.h"
#include <OgreRibbonTrail.h>

AnimationManager::AnimationManager()
{
	mBodyNode = gPlayer->m_sceneObject->mNode;
	mBodyEnt = gPlayer->m_sceneObject->mEntity;
	mKeyDirection = gPlayer->m_direction;
	setupBody(gSceneMgr);
	setupAnimations();
}

void AnimationManager::setupBody(Ogre::SceneManager* sceneMgr)
{
	//Crear espadas y colocarlas en la espalda
	mSword1 = sceneMgr->createEntity("SinbadSword1", "Sword.mesh");
	mSword2 = sceneMgr->createEntity("SinbadSword2", "Sword.mesh");
	mBodyEnt->attachObjectToBone("Sheath.L", mSword1);
	mBodyEnt->attachObjectToBone("Sheath.R", mSword2);

	//Crear efecto de luz
	Ogre::NameValuePairList params;
	params["numberOfChains"] = "2";
	params["maxElements"] = "80";
	mSwordTrail = (Ogre::RibbonTrail*)sceneMgr->createMovableObject("RibbonTrail", &params);
	mSwordTrail->setMaterialName("Examples/LightRibbonTrail");
	mSwordTrail->setTrailLength(20);
	mSwordTrail->setVisible(false);
	sceneMgr->getRootSceneNode()->attachObject(mSwordTrail);

	for (int i = 0; i < 2; i++)
	{
		mSwordTrail->setInitialColour(i, 1, Ogre::Real(0.8), 0);
		mSwordTrail->setColourChange(i, 0.75, 1.25, 1.25, 1.25);
		mSwordTrail->setWidthChange(i, 1);
		mSwordTrail->setInitialWidth(i, 0.5);
	}
}

void AnimationManager::setupAnimations()
{
	//Necesario para dar naturalidad a las animaciones
	mBodyEnt->getSkeleton()->setBlendMode(Ogre::SkeletonAnimationBlendMode::ANIMBLEND_CUMULATIVE);

	Ogre::String animNames[] =
	{"IdleBase", "IdleTop", "RunBase", "RunTop", "HandsClosed", "HandsRelaxed", "DrawSwords",
	"SliceVertical", "SliceHorizontal", "Dance", "JumpStart", "JumpLoop", "JumpEnd"};

	//Llenar la lista de animaciones
	for (int i = 0; i < NUM_ANIMS; i++)
	{
		mAnims[i] = mBodyEnt->getAnimationState(animNames[i]);
		mAnims[i]->setLoop(true);
		mFadingIn[i] = false;
		mFadingOut[i] = false;
	}

	//Comenzar con el estado inicial (top y bottom juntos)
	setBaseAnimation(ANIM_IDLE_BASE);
	setTopAnimation(ANIM_IDLE_TOP);

	//Relajar las manos
	mAnims[ANIM_HANDS_RELAXED]->setEnabled(true);

	mSwordsDrawn = false;
}


//Actualizar todo
void AnimationManager::addTime(Ogre::Real deltaTime)
{
	mKeyDirection = gPlayer->m_direction;
	updateAnimations(deltaTime);
	updateActions();
}

//Actualizar las animaciones
void AnimationManager::updateAnimations(Ogre::Real deltaTime)
{
	Ogre::Real baseAnimSpeed = 1;
	Ogre::Real topAnimSpeed = 1;

	mTimer += deltaTime;

	if (mTopAnimID == ANIM_DRAW_SWORDS)
	{
		//Animacion de las espadas
		topAnimSpeed = mSwordsDrawn ? Ogre::Real(-1) : Ogre::Real(1);

		//Cuando deba agarrar las espadas con las manos
		if (mTimer >= mAnims[mTopAnimID]->getLength() / 2 &&
			mTimer - deltaTime < mAnims[mTopAnimID]->getLength() / 2)
		{
			//Pasa las espadas a las manos
			mBodyEnt->detachAllObjectsFromBone();
			mBodyEnt->attachObjectToBone(mSwordsDrawn ? "Sheath.L" : "Handle.L", mSword1);
			mBodyEnt->attachObjectToBone(mSwordsDrawn ? "Sheath.R" : "Handle.R", mSword2);
			//Cambia el estado de las manos
			mAnims[ANIM_HANDS_CLOSED]->setEnabled(!mSwordsDrawn);
			mAnims[ANIM_HANDS_RELAXED]->setEnabled(mSwordsDrawn);

			//Alterna el movimiento de la espada
			if (mSwordsDrawn)
			{
				mSwordTrail->setVisible(false);
				mSwordTrail->removeNode(mSword1->getParentNode());
				mSwordTrail->removeNode(mSword2->getParentNode());
			}
			else
			{
				mSwordTrail->setVisible(true);
				mSwordTrail->addNode(mSword1->getParentNode());
				mSwordTrail->addNode(mSword2->getParentNode());
			}
		}

		if (mTimer >= mAnims[mTopAnimID]->getLength())
		{
			//Animacion finalizada, se devuelve a su estado original
			if (mBaseAnimID == ANIM_IDLE_BASE) setTopAnimation(ANIM_IDLE_TOP);
			else
			{
				setTopAnimation(ANIM_RUN_TOP);
				mAnims[ANIM_RUN_TOP]->setTimePosition(mAnims[ANIM_RUN_BASE]->getTimePosition());
			}
			mSwordsDrawn = !mSwordsDrawn;
		}
	}
	else if (mTopAnimID == ANIM_SLICE_VERTICAL || mTopAnimID == ANIM_SLICE_HORIZONTAL)
	{
		if (mTimer >= mAnims[mTopAnimID]->getLength())
		{
			//Animacion terminada
			if (mBaseAnimID == ANIM_IDLE_BASE) setTopAnimation(ANIM_IDLE_TOP);
			else
			{
				setTopAnimation(ANIM_RUN_TOP);
				mAnims[ANIM_RUN_TOP]->setTimePosition(mAnims[ANIM_RUN_BASE]->getTimePosition());
			}
		}

		//No mover las caderas de lado a lado
		if (mBaseAnimID == ANIM_IDLE_BASE) baseAnimSpeed = 0;
	}
	else if (mBaseAnimID == ANIM_JUMP_START)
	{
		if (mTimer >= mAnims[mBaseAnimID]->getLength())
		{
			//Termina la animacion
			setBaseAnimation(ANIM_JUMP_LOOP, true);
		}
	}
	else if (mBaseAnimID == ANIM_JUMP_END)
	{
		if (mTimer >= mAnims[mBaseAnimID]->getLength())
		{
			//Aterriza correctamente
			if (mKeyDirection == Ogre::Vector3::ZERO)
			{
				setBaseAnimation(ANIM_IDLE_BASE);
				setTopAnimation(ANIM_IDLE_TOP);
			}
			else
			{
				setBaseAnimation(ANIM_RUN_BASE, true);
				setTopAnimation(ANIM_RUN_TOP, true);
			}
		}
	}

	// Incrementa los tiemposs actuales de base y top
	if (mBaseAnimID != ANIM_NONE) mAnims[mBaseAnimID]->addTime(deltaTime * baseAnimSpeed);
	if (mTopAnimID != ANIM_NONE) mAnims[mTopAnimID]->addTime(deltaTime * topAnimSpeed);

	//Transición entre animaciones
	fadeAnimations(deltaTime);
}

void AnimationManager::updateActions() 
{
	//SALTAR
	if (gPlayer->m_jump && (mTopAnimID == ANIM_IDLE_TOP || mTopAnimID == ANIM_RUN_TOP))
	{
		//Saltar
		setBaseAnimation(ANIM_JUMP_START, true);
		setTopAnimation(ANIM_NONE);
		mTimer = 0;
		gPlayer->m_jump = false;
		gPlayer->m_inJump = true;
		gPlayer->m_fall = true;
	}

	//CAER: Terminar la animacion de saltar
	if ( gPlayer->m_fall && gPlayer->fallManager() && mBaseAnimID == ANIM_JUMP_LOOP )
	{
		setBaseAnimation(ANIM_JUMP_END, true);
		mTimer = 0;
		gPlayer->m_fall = false;
		gPlayer->m_inJump = false;
	}

	//Con cualquier movimiento
	if (!mKeyDirection.isZeroLength() && mBaseAnimID == ANIM_IDLE_BASE)
	{
		//Comenzar a correr
		setBaseAnimation(ANIM_RUN_BASE, true);
		if (mTopAnimID == ANIM_IDLE_TOP) setTopAnimation(ANIM_RUN_TOP, true);
	}
	else if (mKeyDirection.isZeroLength() && mBaseAnimID == ANIM_RUN_BASE)
	{
		//Deja de correr
		setBaseAnimation(ANIM_IDLE_BASE);
		if (mTopAnimID == ANIM_RUN_TOP) setTopAnimation(ANIM_IDLE_TOP);
	}


	//Sacar/guardar espadas
	if (gPlayer->m_SwordsDrawn && (mTopAnimID == ANIM_IDLE_TOP || mTopAnimID == ANIM_RUN_TOP))
	{
		setTopAnimation(ANIM_DRAW_SWORDS, true);
		mTimer = 0;
		gPlayer->m_SwordsDrawn = false;
	}

	//Ataques
	if (mSwordsDrawn && gPlayer->m_attack && (mTopAnimID == ANIM_IDLE_TOP || mTopAnimID == ANIM_RUN_TOP))
	{
		int rand = std::rand()%100;
		if(rand < 50)
			setTopAnimation(ANIM_SLICE_VERTICAL, true);
		else
			setTopAnimation(ANIM_SLICE_HORIZONTAL, true);
		mTimer = 0;
	}

	/*
	//Tecla E
	else if (evt.key == OIS::KC_E && !mSwordsDrawn)
	{
		if (mTopAnimID == ANIM_IDLE_TOP || mTopAnimID == ANIM_RUN_TOP)
		{
			//Bailar
			setBaseAnimation(ANIM_DANCE, true);
			setTopAnimation(ANIM_NONE);
			//Deshabilitar movimiento de las manos
			mAnims[ANIM_HANDS_RELAXED]->setEnabled(false);
		}
		else if (mBaseAnimID == ANIM_DANCE)
		{
			//No bailar
			setBaseAnimation(ANIM_IDLE_BASE);
			setTopAnimation(ANIM_IDLE_TOP);
			//Habilitar el movimiento de las manos
			mAnims[ANIM_HANDS_RELAXED]->setEnabled(true);
		}
	}
	*/
}

void AnimationManager::fadeAnimations(Ogre::Real deltaTime)
{
	for (int i = 0; i < NUM_ANIMS; i++)
	{
		if (mFadingIn[i])
		{
			//Introducir animacion
			Ogre::Real newWeight = mAnims[i]->getWeight() + deltaTime * ANIM_FADE_SPEED;
			mAnims[i]->setWeight(Ogre::Math::Clamp<Ogre::Real>(newWeight, 0, 1));
			if (newWeight >= 1) mFadingIn[i] = false;
		}
		else if (mFadingOut[i])
		{
			//Sacar animacion
			Ogre::Real newWeight = mAnims[i]->getWeight() - deltaTime * ANIM_FADE_SPEED;
			mAnims[i]->setWeight(Ogre::Math::Clamp<Ogre::Real>(newWeight, 0, 1));
			if (newWeight <= 0)
			{
				mAnims[i]->setEnabled(false);
				mFadingOut[i] = false;
			}
		}
	}
}

void AnimationManager::setBaseAnimation(AnimID id, bool reset)
{
	if (mBaseAnimID >= 0 && mBaseAnimID < NUM_ANIMS)
	{
		//Si tenemos una animacion antigua, se quita
		mFadingIn[mBaseAnimID] = false;
		mFadingOut[mBaseAnimID] = true;
	}

	mBaseAnimID = id;

	if (id != ANIM_NONE)
	{
		//Si tenemos una animacion nueva, se introduce
		mAnims[id]->setEnabled(true);
		mAnims[id]->setWeight(0);
		mFadingOut[id] = false;
		mFadingIn[id] = true;
		if (reset) mAnims[id]->setTimePosition(0);
	}
}
void AnimationManager::setTopAnimation(AnimID id, bool reset)
{
	if (mTopAnimID >= 0 && mTopAnimID < NUM_ANIMS)
	{
		//Si tenemos una animacion antigua, se quita
		mFadingIn[mTopAnimID] = false;
		mFadingOut[mTopAnimID] = true;
	}

	mTopAnimID = id;

	if (id != ANIM_NONE)
	{
		//Si tenemos una animacion nueva, se introduce
		mAnims[id]->setEnabled(true);
		mAnims[id]->setWeight(0);
		mFadingOut[id] = false;
		mFadingIn[id] = true;
		if (reset) mAnims[id]->setTimePosition(0);
	}
}