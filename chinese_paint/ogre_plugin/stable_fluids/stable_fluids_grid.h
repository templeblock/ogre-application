#ifndef _STABLE_FLUIDS_GRID_H_
#define _STABLE_FLUIDS_GRID_H_
#include "../simulation_interface/water_interface.h"
#include <ogre.h>

class StableFluidsGrid : public WaterInterface
{
private:
	//float									m_fTimeSpeed;
	float									m_fDiff;
	float									m_fVisc;
	float									m_fForce;
	float									m_fSource;
	float *									m_vfHeightMap[3] ; // 3 map to do wave equation
	float *									m_vfDumpening;
	float *									m_vbIntersectGrid;
	int										m_iCurrentMap ;
	Ogre::HardwarePixelBufferSharedPtr		m_heightMap;
	Ogre::PixelBox*							m_pPixelBox;
	Ogre::ManualObject *					m_pManuObj;
	Ogre::SceneManager *					m_pSceneMgr;
	Ogre::SceneNode *						m_pManualNode;
	Ogre::Rectangle2D *						m_pMiniScreen;
	Ogre::SceneNode	*						m_pMiniScreenNode;
	Ogre::ParticleSystem *					m_pPS;
	Ogre::SceneNode	*						m_pPSNode;

	//fish vertex data
	size_t									m_sVertexCount;
	size_t									m_sIndex_count;
	int										m_iCurrentVertex ;
	Ogre::Vector3*							m_vVertices[3];
	unsigned long*							m_vIndices;
	float *									m_vfEnforceU;
	int *									m_viEnforceUCount;
	float *									m_vfEnforceV;
	int *									m_viEnforceVCount;

	float									m_fLastTimeStamp ;
	float									m_fLastAnimationTimeStamp;
	float									m_fLastFrameTime ;


public:
	enum MAP_DISPLAY_TYPE
	{
		DISPLAY_DENSITY_MAP,
		DISPLAY_BOOLEAN_GRID,
	};
	MAP_DISPLAY_TYPE						m_eMapDisplayType;
	float *									m_vfU;
	float *									m_vfV;
	float *									m_vfUPrev;
	float *									m_vfVPrev;
	int										m_iGridNumber;
	int										m_iGridSize;
	//float *								m_vfDens;
	//float *								m_vfDensPrev;
private:

public:
											StableFluidsGrid(unsigned int number);
											~StableFluidsGrid(void);
	void									init(Ogre::SceneManager *mgr);
	void									release(void);
	void									updateParticle(float timePass);//draw particle
	void									updateMesh(float timePass);
	void									updateDebug();//draw velocity field
	void									push(int N, float x, float y, float addx, float addy, float depth, bool absolute, float *current);//set presure to 00 10 01 11 
	void									push(float x, float y, float depth, bool absolute=false) ;//set pressure to wave equation and add force to velocity
	void									clear(void);//clear velocity
	void									updateMeshData(Ogre::SceneNode *node, Ogre::Entity *entity);//get mesh vertex data from mesh
	void									calcMeshFace();//compute fish boundary to m_vbIntersectGrid
	void									calcMeshEnforce();//compute fish animation force to m_vfEnforceU and m_vfEnforceV
	void									setMeshBoundary();//set m_vbIntersectGrid to m_vfU and m_vfV
	void									setMeshEnforce(float timePass);//set m_vfEnforceU and m_vfEnforceV to m_vfU and m_vfV
};

#endif //_STABLE_FLUIDS_GRID_H_