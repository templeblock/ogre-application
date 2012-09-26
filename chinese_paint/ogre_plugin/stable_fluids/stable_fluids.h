#ifndef STABLE_FLUIDS_MESH_H_
#define STABLE_FLUIDS_MESH_H_

#include "../simulation_interface/simulation_interface.h"
#include <iostream>
#include <ogre.h>

class WaterInterface;
typedef std::vector<Ogre::Entity *> V_ENTITY;
typedef std::vector<Ogre::SceneNode *> V_NODE;
class StableFluids : public SimulationInterface
{
private:
	//typedef std::vector<Ogre::Vector3>		VECTOR_TARGET_POSITION;
	enum eMeshType{
		MESH_FISH = 0,
		MESH_SPHERE,
		MESH_CUBE,
		MESH_TRIANGLE,
	};
	enum eMoveType{
		MOVE_AUTO,
		MOVE_CUSTOM
	};
	Ogre::SceneManager *					m_pSceneMgr;
	Ogre::Camera *							m_pCamera;
	Ogre::SceneNode*						m_pCameraNode;
	WaterInterface*							m_pWaterInterface;
	//Ogre::Entity*							m_pFish;
	//Ogre::SceneNode*						m_pFishNode;
	eMoveType								m_eMoveType;
	eMeshType								m_eCurrentMesh;
	V_ENTITY								m_vpEntity;
	V_NODE									m_vpNode;

	Ogre::AnimationState*					m_pSwimState;
	//unsigned int							m_uiCurrentTarget;
	//VECTOR_TARGET_POSITION					m_vTarget;
	Ogre::Vector3							m_vec3Pos;
	float									m_fCurrentTime;

public:
											StableFluids(Ogre::SceneManager *sceneMgr, Ogre::Camera *camera) ;
    
    virtual									~StableFluids ();
	void									init();
	virtual void							release();
	virtual void							setupControls(OgreBites::SdkTrayManager* sdkTray);
	virtual void							sliderMoved(OgreBites::Slider* slider);
	virtual void							checkBoxToggled(OgreBites::CheckBox* checkBox);
	virtual void							itemSelected(OgreBites::SelectMenu* menu);
	virtual void							update(float timePass);
};

#endif //STABLE_FLUIDS_MESH_H_
