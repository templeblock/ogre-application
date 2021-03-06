#ifndef _GAME_SYSTEM_H_
#define _GAME_SYSTEM_H_

#include "dot_scene_loader.h"
#include "../game_object/mosquito.h"
#include "../game_object/weapon.h"
#include "../physic/physic_shape_base.h"
#include "../physic/physic_rigid_body.h"
#include "../kinect/physic_kinect.h"
#include "../kinect/kinect_device.h"
#include "wave_system.h"
#include <list>
#include <vector>
#include <map>
#include <ogre.h>

typedef std::list< PhysicRigidBody * > V_RIGID_BODY;
typedef std::list<  PhysicShapeBase* > V_SHAPE;
typedef std::list< MosquitoBase * > V_MOSQUITO;
typedef std::list< WeaponKnife * > V_WEAPON;
class PhysicDebug;
class btDynamicsWorld;
class btManifoldPoint;
class btCollisionObject;
class DOFManager;

const float DEF_MAX_PLAY_TIME = 120.0;

namespace Ogre
{
	class SceneManager;
	class RenderWindow;
	class Overlay;
}

namespace CEGUI
{
	class Window;
}

typedef std::vector< std::string > V_STRING;
typedef std::map < std::string , CEGUI::Window *> M_CEGUI_WINDOW;
typedef std::map < std::string , Ogre::Overlay *> M_OGRE_WINDOW;

class MeshData
{
public:
	std::string m_sMeshName;
	float		m_fvSize[3];
	float		m_fvScale[3];
	float		m_fvPos[3];
	float		m_fvQuat[4];
	V_STRING m_vAniName;
};

typedef std::vector< MeshData > V_MESH_DATA;


class GameSystem
{
public:
	enum DebugHandVState
	{
		eHandMoveVNothing,
		eHandMoveUp,
		eHandMoveDown,
	};

	enum DebugHandHState
	{
		eHandMoveHNothing,
		eHandMoveRight,
		eHandMoveLeft,
	};

	enum DebugHandAttackState
	{
		eHandAttackWait,
		eHandAttacking,
		eHandAttacked,
	};

	enum HandState
	{
		eOnHandOpen = 0,
		eOnHandClose,
		eOnHandWaitAttack,
		eOnHandAttacked,
		eOnHandWaitShoot,
	};
	enum GameState
	{
		eOnMenu = 0,//主選單畫面
		eOnPlaying,//遊戲中畫面
		eOnEnd,//結算畫面
	};
	DebugHandVState								m_eDebugHandVState;
	DebugHandHState								m_eDebugHandHState;
	DebugHandAttackState						m_eDebugHandAttackState;
	float										m_vfHandDebugPos[3];
	float										m_fTwoHandDistance;
	Ogre::DotSceneLoader						m_dotSceneLoader;
	bool										m_bIsDebug;
	bool										m_bIsPause;
	float										m_vfCameraPos[3];
	float										m_vfCameraDir[3];
	V_MESH_DATA									m_vMeshData;
	V_MESH_DATA									m_vWeapeanMeshData;
private:
												GameSystem(void);
private:

	float										m_fCurrentBlood;
	int											m_iPlayerBlood;
	static GameSystem *							g_instance;
	WaveSystem									m_waveSystem;
	btDynamicsWorld*							m_pWorld;
	Ogre::SceneManager *						m_pSceneMgr;
	Ogre::RenderWindow *						m_pWindow;
	//V_RIGID_BODY								m_vRigidBody;
	V_MOSQUITO									m_vMosquito;
	//V_SHAPE										m_vShape;
	V_WEAPON									m_vWeapon;
	float										m_fTimePass;
	float										m_fTrackingTime;
	GameState									m_eState;
	//Hand control
	HandState									m_eHandState;
	float										m_fHandCloseTime;
	float										m_fShootTimePass;
	float										m_fRightHandZPos;
	Ogre::RaySceneQueryResult					m_mosquitoQueryResult;
	//kinect
	PhysicKinect *								m_vpPlayer/*[NUI_SKELETON_COUNT]*/;
	DWORD										m_iCurrentID;
	//CEGUI::Window *								m_pSheet;
	M_CEGUI_WINDOW								m_mSheet;
	M_OGRE_WINDOW								m_mOverlay;
	//準心
	//Ogre::RaySceneQuery*						mRaySceneQuery;
	//Ogre::Entity*								EntSight;
	//Ogre::SceneNode*							NodeSight;
	//Ogre::Entity*								shouldEnt;
	//Ogre::SceneNode*							shouldNode;
	//float										Shoulder[3];
	WeaponKnife*								ChooesKnife;//當作選取武器的快捷鍵 add
	WeaponKnife*								ChooesBook;//當作選取武器的快捷鍵 add
	WeaponKnife*								ChooesBomb; //當作選取武器的快捷鍵 add
	//Ogre::ManualObject *						m_pHintObject;//用來指引用的
	Ogre::Entity*								m_pHintObject;
	Ogre::SceneNode *							m_pHintNode;
	DOFManager*									m_pCompositer;
	Ogre::ParticleSystem *						m_pSelectWeapon;
	Ogre::SceneNode *							m_pSelectWeaponNode;
	Ogre::BillboardSet *						m_pBillboardSet;
	Ogre::Billboard*							m_pBillboard[3];
	Ogre::SceneNode *							m_pBillboardNode;
	float										m_fBillboardTime;
	Ogre::BillboardSet *						m_pDrinkBillboardSet;
	Ogre::Billboard*							m_pDrinkBillboard;
	Ogre::SceneNode *							m_pDrinkBillboardNode;
	float										m_fDrinkBillboardTime;

public:
												~GameSystem(void);
	static GameSystem *							getInstance(void);
	void										init(btDynamicsWorld* world, Ogre::SceneManager *sceneMgr, Ogre::RenderWindow *pWindow, Ogre::Root *mRoot, Ogre::Viewport *mViewport);
	void										initWeapon(void);
	void										initMeshData(void);
	void										release(void);
	void										releaseMosquito(void);
	void										releaseWeapon(void);
	void										releaseCharacter(void);
	void										restart(unsigned int stageID = 0);
	float										getTimePass(void)const;
	HandState									getHandState(void)const;
	void										setHandState(HandState state);

	void										createShape(const char *modelName, float *scale, float *pos, float *quat);
	void										createWeapon(WEAPON_TYPE type, const char *modelName, float mass, float *scale, float *pos, float *size, float *quat, int score, int otherData, float *tar);
	PhysicRigidBody *							createRidigBody(const char *modelName, float mass, float *scale, float *pos, float *quat, PhysicDebug *debug = NULL, void *userPoint = NULL, int flag = 0);
	void										createMosquito(MOSQUITO_TYPE type, unsigned int moveType, float speed, unsigned int meshID, float mass, float *scale, float *pos, float *quat, int score, int otherData = 0);
	void										randomShoot(MOSQUITO_TYPE type = eMosquitoBase);
	void										initScene(void);//產生場景
	void										initPlayer(void);//產生空的玩家類別
	void										initPlayer(unsigned int playerCount);//初始化人數
	void										update(float timePass, KinectDevice *deivce);//根據遊戲狀態分配要進入哪個流程
	void										updateMosquito(float timePass);//更新蚊子的狀態及位置
	void										updateWeapon(float timePass);//更新蚊子的狀態及位置
	void										updateMenu(float timePass, KinectDevice *deivce);//主要menu畫面
	void										updatePlaying(float timePass, KinectDevice *deivce);//遊戲中主要的流程
	void										updateEnd(float timePass, KinectDevice *deivce);//結算畫面
	void										updatePlayer(KinectDevice *device, float timePass);
	void										updatePlayerDebug(float timePass);//若是沒有kinect採取的動作
	void										updateHandState(float timePass);//依據雙手位置判斷遊戲動作狀態
	void										updateSmoke(float timePass);
	void										updateDrink(float timePass);
	void										notifyMosquitoAlert(void);
	bool										checkPlayerState(KinectDevice *device, float timePass, float limite = 0.0f);
	bool										checkShootPose(void);
	float										getFullTime(void)const;
	void										setGameState(GameState state);
	void										setDrinkTime(void);
	void										setAllVisible(bool visible);
	void										testCollision(void);
	void										reduceBlood(void);
	void										killAllMosquito(ScoreBase *object0);

	static bool									MaterialProcessedCallback(btManifoldPoint& cp,btCollisionObject* body0,btCollisionObject* body1);
	static bool									MaterialCombinerCallback(btManifoldPoint& cp,	const btCollisionObject* colObj0,int partId0,int index0,const btCollisionObject* colObj1,int partId1,int index1);

	int											NumBomb;
	int											NumBook;
	int											CurrentWeapon; //目前選取的武器 add
	void										setSelectWeapon(ScoreBase *weapon);
	
};

#endif //_GAME_SYSTEM_H_
