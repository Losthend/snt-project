#ifndef __AnimationManager_h_
#define __AnimationManager_h_

#define NUM_ANIMS 13           // number of animations the character has
#define ANIM_FADE_SPEED 7.5f   // animation crossfade speed in % of full weight per second

class AnimationManager
{
private:

	//Animaciones
	enum AnimID
	{
		ANIM_IDLE_BASE,
		ANIM_IDLE_TOP,
		ANIM_RUN_BASE,
		ANIM_RUN_TOP,
		ANIM_HANDS_CLOSED,
		ANIM_HANDS_RELAXED,
		ANIM_DRAW_SWORDS,
		ANIM_SLICE_VERTICAL,
		ANIM_SLICE_HORIZONTAL,
		ANIM_DANCE,
		ANIM_JUMP_START,
		ANIM_JUMP_LOOP,
		ANIM_JUMP_END,
		ANIM_NONE
	};

public:
	
	AnimationManager();
	void addTime(Ogre::Real deltaTime);
	bool mSwordsDrawn;

private:

	void setupBody(Ogre::SceneManager* sceneMgr);
	void setupAnimations();
	void updateAnimations(Ogre::Real deltaTime);
	void updateActions();
	void fadeAnimations(Ogre::Real deltaTime);
	void setBaseAnimation(AnimID id, bool reset = false);
	void setTopAnimation(AnimID id, bool reset = false);

	Ogre::SceneNode* mBodyNode;
	Ogre::Entity* mBodyEnt;

	Ogre::Entity* mSword1;
	Ogre::Entity* mSword2;
	Ogre::RibbonTrail* mSwordTrail;

	Ogre::AnimationState* mAnims[NUM_ANIMS];    //Lista de animaciones
	AnimID mBaseAnimID;							// Animacion base actual
	AnimID mTopAnimID;							// Animacion top actual
	bool mFadingIn[NUM_ANIMS];					// Animaciones introducidas
	bool mFadingOut[NUM_ANIMS];					// Animaciones quitadas

	Ogre::Vector3 mKeyDirection;      // Direccion del jugador
	
	Ogre::Real mTimer;                // Tiempo para controlar las animaciones
};

//---------------------------------------------------------------------------

#endif // #ifndef __AnimationManager_h_

//---------------------------------------------------------------------------
