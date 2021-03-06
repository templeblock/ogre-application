#ifndef _OGRE_PHYSIC_APPLACTION_H_
#define _OGRE_PHYSIC_APPLACTION_H_

#include "ogre_application.h"

#include "OgreBulletDynamicsRigidBody.h"				 // for OgreBullet
#include "Shapes/OgreBulletCollisionsStaticPlaneShape.h" // for static planes
#include "Shapes/OgreBulletCollisionsBoxShape.h"		 // for Boxes
#include "Shapes/OgreBulletCollisionsTerrainShape.h"


//#########################################
//#######  OgrePhysicApplaction   #########
//#########################################

namespace CL
{
	class VehiclesAttrib;
}

class btSoftRigidDynamicsWorld;
class btDynamicsWorld;

class OgrePhysicApplication : public OgreApplication
{
public:
	enum VIEW_TYPE
	{
		VIEW_PERSPACTIVE,
		VIEW_CAR,
	};

private:
	static const float DEF_HIGH_LAND;
	VIEW_TYPE mView;
	Ogre::Timer mTimer;
	//Physic
 	OgreBulletDynamics::DynamicsWorld *mWorld;	// OgreBullet World
 	OgreBulletCollisions::DebugDrawer *mDebugDrawer;
 	int mNumEntitiesInstanced;
	float mMoveSpeed;
 	std::deque<OgreBulletDynamics::RigidBody *>         mBodies;
 	std::deque<OgreBulletCollisions::CollisionShape *>  mShapes;
	float *mpTerrainHeightDataConvert;
	btRigidBody *mpBody;
	btHeightfieldTerrainShape* mpHeightShape;
	CL::VehiclesAttrib *pVehiclesAttrib;

	//this is the most important class
	btSoftRigidDynamicsWorld			*mpSoftWorld;
	btDynamicsWorld						*mpDynamicsWorld;
	btDefaultCollisionConfiguration		*mpCollisionConfiguration;
	btCollisionDispatcher				*mpDispatcher;
	btAxisSweep3						*mpBroadphase;
	btSequentialImpulseConstraintSolver *mpSolver;

	//pick 3d mesh
	enum
	{
		AXIS_TYPE_X,
		AXIS_TYPE_Y,
		AXIS_TYPE_Z,
		AXIS_TYPE_COUNT,
	};
	Ogre::SceneNode *mpCurNode;
	float mOriginHighLand;
	Ogre::SceneNode *mpAxisNode;
	Ogre::SceneNode *mpAxisPartNode[AXIS_TYPE_COUNT];
	Ogre::Entity *mpAxisEntity[AXIS_TYPE_COUNT];



public:
	OgrePhysicApplication();
	virtual ~OgrePhysicApplication();

	//release resource
	virtual void release(void);

	//create resource
	virtual void createScene(void);
	virtual void createUI(void);
	virtual void createPhysic(void);
	void createAxis();
	void createDomino();

	//operator fun
	virtual void createBody(void);
	virtual void createBody(const std::string &modelName, 
		const float &restitution, const float &friction, const float &mass);
	virtual void createVehicles(const std::string &vehicleName, const std::string &wheelsName);
	

	

	// Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	virtual bool frameStarted(const Ogre::FrameEvent& evt);
	bool onVehiclesframeStarted(const Ogre::FrameEvent& evt);
	void onViewCar();
	void processPitch();
	void processYaw();
	void processRoll();

	virtual bool frameEnded(const Ogre::FrameEvent& evt);

    // OIS::KeyListener
    virtual bool keyPressed( const OIS::KeyEvent &arg );
	bool onVehiclesKeyPressed( const OIS::KeyEvent &arg );

    virtual bool keyReleased( const OIS::KeyEvent &arg );
	bool onVehiclesKeyReleased( const OIS::KeyEvent &arg );


    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	//callback event
	bool onQuit(const CEGUI::EventArgs &e);
	bool onViewCar(const CEGUI::EventArgs &e);
	bool onViewWorld(const CEGUI::EventArgs &e);
	bool onMouseEnterUIEvent(const CEGUI::EventArgs &e);
	bool onMouseLeaveUIEvent(const CEGUI::EventArgs &e);
	bool onChangeForceSliderValue(const CEGUI::EventArgs &e);
	//bool onChangeTurnSliderValue(const CEGUI::EventArgs &e);
	bool onChangeMassSliderValue(const CEGUI::EventArgs &e);
	bool onChangeForceEditValue(const CEGUI::EventArgs &e);
	//bool onChangeTurnEditValue(const CEGUI::EventArgs &e);
	bool onChangeMassEditValue(const CEGUI::EventArgs &e);
	bool onChangeMeshListSelect(const CEGUI::EventArgs &e);

	bool onPitch(const CEGUI::EventArgs &e);
	bool onYaw(const CEGUI::EventArgs &e);
	bool onRoll(const CEGUI::EventArgs &e);
};

//#########################################
//################  END   #################
//#########################################

#endif //_OGRE_PHYSIC_APPLACTION_H_