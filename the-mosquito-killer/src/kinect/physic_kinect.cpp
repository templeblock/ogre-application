#include "physic_kinect.h"
#include "../score_system/score_system.h"
#include "../score_system/score_object.h"
#include "../ogre_physic/ogre_physic_debug.h"
#include <iostream>
#include <fstream>
#include <json_spirit.h>
#include <Ogre.h>

const float DEF_POWER_RADIN = 10.0f;
const float MIN_RADIN = 1.5f;
const float MAX_RADIN = 2.35f;
extern bool MOSQUITO_DEBUG_MODE;
PhysicKinect::PhysicKinect(Ogre::SceneManager *scene, btDynamicsWorld *world):
	m_pSceneMgr(scene),
	m_pWorld(world)
{
	for(int i = 0; i < eKinectBodyPart; i ++)
	{
		m_pBody[i] = NULL;
		m_pShape[i] = NULL;
		m_iBodyID[i] = 0;
	}

	for(int i = 0;i < eScaleCount; i++)
	{
		m_vfScale[i] = DEF_POWER_RADIN;
	}
}

PhysicKinect::~PhysicKinect(void)
{
	release();
}

void PhysicKinect::init(DWORD id)
{
	
	m_iKinectID = id;
	float scale[3] = {0.01, 0.01, 0.01};
	float pos[3] = {0.0, 0.0, 100.0};
	float quat[4] = {1.0, 0.0, 0.0, 0.0};
	float size[3] = {2.0, 1.0, 5.0};

	std::ifstream is("../configure/kinect_device.cfg");
	if(is.is_open())
	{
		json_spirit::mValue value;
		json_spirit::read(is, value);
		json_spirit::mObject obj;
		obj = value.get_obj();
		if(obj.count("move_scale") > 0)
		{
			m_vfScale[0] = obj["move_scale"].get_array()[0].get_real();
			m_vfScale[1] = obj["move_scale"].get_array()[1].get_real();
			m_vfScale[2] = obj["move_scale"].get_array()[2].get_real();
		}

		if(obj.count("pos") > 0)
		{
			pos[0] = obj["pos"].get_array()[0].get_real();
			pos[1] = obj["pos"].get_array()[1].get_real();
			pos[2] = obj["pos"].get_array()[2].get_real();
		}
		scale[0] = obj["left_hand_ridigi_body"].get_obj()["scale"].get_array()[0].get_real();
		scale[1] = obj["left_hand_ridigi_body"].get_obj()["scale"].get_array()[1].get_real();
		scale[2] = obj["left_hand_ridigi_body"].get_obj()["scale"].get_array()[2].get_real();

		if(obj["left_hand_ridigi_body"].get_obj().count("size") > 0)
		{
			size[0] = obj["left_hand_ridigi_body"].get_obj()["size"].get_array()[0].get_real();
			size[1] = obj["left_hand_ridigi_body"].get_obj()["size"].get_array()[1].get_real();
			size[2] = obj["left_hand_ridigi_body"].get_obj()["size"].get_array()[2].get_real();
		}
		PhysicDebug *debug = NULL;
		if(MOSQUITO_DEBUG_MODE)
		{
			debug = new OgrePhysicDebug();
			((OgrePhysicDebug *)debug)->init(m_pSceneMgr);
		}
		m_pBody[eKinectLeftHand] = createRidigBody(obj["left_hand_ridigi_body"].get_obj()["mesh_name"].get_str().c_str(), 0.0, scale, pos, size, quat, debug, ScoreSystem::createScoreObject(SCORE_TYPE_HAND), 2);
		m_iBodyID[eKinectLeftHand] = NUI_SKELETON_POSITION_HAND_LEFT;

		scale[0] = obj["right_hand_ridigi_body"].get_obj()["scale"].get_array()[0].get_real();
		scale[1] = obj["right_hand_ridigi_body"].get_obj()["scale"].get_array()[1].get_real();
		scale[2] = obj["right_hand_ridigi_body"].get_obj()["scale"].get_array()[2].get_real();

		if(obj["right_hand_ridigi_body"].get_obj().count("size") > 0)
		{
			size[0] = obj["right_hand_ridigi_body"].get_obj()["size"].get_array()[0].get_real();
			size[1] = obj["right_hand_ridigi_body"].get_obj()["size"].get_array()[1].get_real();
			size[2] = obj["right_hand_ridigi_body"].get_obj()["size"].get_array()[2].get_real();
		}

		debug = NULL;
		if(MOSQUITO_DEBUG_MODE)
		{
			debug = new OgrePhysicDebug();
			((OgrePhysicDebug *)debug)->init(m_pSceneMgr);
		}
		m_pBody[eKinectRightHand] = createRidigBody(obj["right_hand_ridigi_body"].get_obj()["mesh_name"].get_str().c_str(), 0.0, scale, pos, size, quat, debug, ScoreSystem::createScoreObject(SCORE_TYPE_HAND), 2);
		m_iBodyID[eKinectRightHand] = NUI_SKELETON_POSITION_HAND_RIGHT;

		if(obj.count("left_hand") > 0)
		{
			scale[0] = obj["left_hand"].get_obj()["scale"].get_array()[0].get_real();
			scale[1] = obj["left_hand"].get_obj()["scale"].get_array()[1].get_real();
			scale[2] = obj["left_hand"].get_obj()["scale"].get_array()[2].get_real();
			if(obj["left_hand"].get_obj().count("size") > 0)
			{
				size[0] = obj["left_hand"].get_obj()["size"].get_array()[0].get_real();
				size[1] = obj["left_hand"].get_obj()["size"].get_array()[1].get_real();
				size[2] = obj["left_hand"].get_obj()["size"].get_array()[2].get_real();
			}

			m_pShape[eKinectLeftHand] = new OgreShapeBox(m_pSceneMgr);
			m_pShape[eKinectLeftHand]->init(obj["left_hand"].get_obj()["mesh_name"].get_str().c_str(), scale);
		}

		if(obj.count("right_hand") > 0)
		{
			scale[0] = obj["right_hand"].get_obj()["scale"].get_array()[0].get_real();
			scale[1] = obj["right_hand"].get_obj()["scale"].get_array()[1].get_real();
			scale[2] = obj["right_hand"].get_obj()["scale"].get_array()[2].get_real();
			if(obj["left_hand"].get_obj().count("size") > 0)
			{
				size[0] = obj["left_hand"].get_obj()["size"].get_array()[0].get_real();
				size[1] = obj["left_hand"].get_obj()["size"].get_array()[1].get_real();
				size[2] = obj["left_hand"].get_obj()["size"].get_array()[2].get_real();
			}
			m_pShape[eKinectRightHand] = new OgreShapeBox(m_pSceneMgr);
			m_pShape[eKinectRightHand]->init(obj["left_hand"].get_obj()["mesh_name"].get_str().c_str(), scale);
		}
	}
	else
	{
		m_pBody[eKinectLeftHand] = createRidigBody("sphere.mesh", 0.0, scale, pos, size, quat, NULL, ScoreSystem::createScoreObject(SCORE_TYPE_HAND), 2);
		m_iBodyID[eKinectLeftHand] = NUI_SKELETON_POSITION_HAND_LEFT;

		m_pBody[eKinectRightHand] = createRidigBody("sphere.mesh", 0.0, scale, pos, size, quat, NULL, ScoreSystem::createScoreObject(SCORE_TYPE_HAND), 2);
		m_iBodyID[eKinectRightHand] = NUI_SKELETON_POSITION_HAND_RIGHT;

		//m_pShape[eKinectLeftHand] = new OgreShapeBox(m_pSceneMgr);
		//m_pShape[eKinectLeftHand]->init("sphere.mesh", scale);

		//m_pShape[eKinectRightHand] = new OgreShapeBox(m_pSceneMgr);
		//m_pShape[eKinectRightHand]->init("sphere.mesh", scale);
		//m_pBody[NUI_SKELETON_POSITION_HIP_CENTER] = createRidigBody("sphere.mesh", 0.0, scale, pos, quat, NULL, ScoreSystem::createScoreObject(SCORE_TYPE_BODY), 2);
		//m_pBody[NUI_SKELETON_POSITION_SPINE] = createRidigBody("sphere.mesh", 0.0, scale, pos, quat, NULL, ScoreSystem::createScoreObject(SCORE_TYPE_BODY), 2);
		//m_pBody[NUI_SKELETON_POSITION_SHOULDER_CENTER] = createRidigBody("sphere.mesh", 0.0, scale, pos, quat, NULL, ScoreSystem::createScoreObject(SCORE_TYPE_BODY), 2);
		//m_pBody[NUI_SKELETON_POSITION_HEAD] = createRidigBody("sphere.mesh", 0.0, scale, pos, quat, NULL, ScoreSystem::createScoreObject(SCORE_TYPE_BODY), 2);
		//m_pBody[NUI_SKELETON_POSITION_SHOULDER_LEFT] = createRidigBody("sphere.mesh", 0.0, scale, pos, quat, NULL, ScoreSystem::createScoreObject(SCORE_TYPE_BODY), 2);
		//m_pBody[NUI_SKELETON_POSITION_ELBOW_LEFT] = createRidigBody("sphere.mesh", 0.0, scale, pos, quat, NULL, ScoreSystem::createScoreObject(SCORE_TYPE_BODY), 2);
		//m_pBody[NUI_SKELETON_POSITION_WRIST_LEFT] = createRidigBody("sphere.mesh", 0.0, scale, pos, quat, NULL, ScoreSystem::createScoreObject(SCORE_TYPE_BODY), 2);

		//m_pBody[NUI_SKELETON_POSITION_SHOULDER_RIGHT] = createRidigBody("sphere.mesh", 0.0, scale, pos, quat, NULL, ScoreSystem::createScoreObject(SCORE_TYPE_BODY), 2);
		//m_pBody[NUI_SKELETON_POSITION_ELBOW_RIGHT] = createRidigBody("sphere.mesh", 0.0, scale, pos, quat, NULL, ScoreSystem::createScoreObject(SCORE_TYPE_BODY), 2);
		//m_pBody[NUI_SKELETON_POSITION_WRIST_RIGHT] = createRidigBody("sphere.mesh", 0.0, scale, pos, quat, NULL, ScoreSystem::createScoreObject(SCORE_TYPE_BODY), 2);



		//m_pBody[NUI_SKELETON_POSITION_HIP_LEFT] = createRidigBody("sphere.mesh", 0.0, scale, pos, quat, NULL, ScoreSystem::createScoreObject(SCORE_TYPE_BODY), 2);
		//m_pBody[NUI_SKELETON_POSITION_KNEE_LEFT] = createRidigBody("sphere.mesh", 0.0, scale, pos, quat, NULL, ScoreSystem::createScoreObject(SCORE_TYPE_BODY), 2);
		//m_pBody[NUI_SKELETON_POSITION_ANKLE_LEFT] = createRidigBody("sphere.mesh", 0.0, scale, pos, quat, NULL, ScoreSystem::createScoreObject(SCORE_TYPE_BODY), 2);
		//m_pBody[NUI_SKELETON_POSITION_FOOT_LEFT] = createRidigBody("sphere.mesh", 0.0, scale, pos, quat, NULL, ScoreSystem::createScoreObject(SCORE_TYPE_BODY), 2);
		//m_pBody[NUI_SKELETON_POSITION_HIP_RIGHT] = createRidigBody("sphere.mesh", 0.0, scale, pos, quat, NULL, ScoreSystem::createScoreObject(SCORE_TYPE_BODY), 2);
		//m_pBody[NUI_SKELETON_POSITION_KNEE_RIGHT] = createRidigBody("sphere.mesh", 0.0, scale, pos, quat, NULL, ScoreSystem::createScoreObject(SCORE_TYPE_BODY), 2);
		//m_pBody[NUI_SKELETON_POSITION_ANKLE_RIGHT] = createRidigBody("sphere.mesh", 0.0, scale, pos, quat, NULL, ScoreSystem::createScoreObject(SCORE_TYPE_BODY), 2);
		//m_pBody[NUI_SKELETON_POSITION_FOOT_RIGHT] = createRidigBody("sphere.mesh", 0.0, scale, pos, quat, NULL, ScoreSystem::createScoreObject(SCORE_TYPE_BODY), 2);
	}
	is.close();
	setVisible(false);
}

void PhysicKinect::release(void)
{
	for(int i = 0; i < eKinectBodyPart; i ++)
	{
		if(NULL != m_pBody[i])
		{
			m_pBody[i]->release();
			delete m_pBody[i];
			m_pBody[i] = NULL;
		}

		if(NULL != m_pShape[i])
		{
			m_pShape[i]->release();
			delete m_pShape[i];
			m_pShape[i] = NULL;
		}
	}
}

PhysicRigidBody *PhysicKinect::createRidigBody(const char *modelName, float mass, float *scale, float *pos, float *size, float *quat, PhysicDebug *debug, void *userPoint, int flag)
{
	OgreShapeBox *shape  = new OgreShapeBox(m_pSceneMgr);
	shape->init(modelName, scale);
	PhysicRigidBody *body = new PhysicRigidBody(m_pWorld);
	body->init(shape, debug, mass, pos, size, quat, userPoint, flag);
	
	return body;
}

void PhysicKinect::update(const NUI_SKELETON_DATA &data)
{
	int id = 0;
	float dist_x = 0;
	Ogre::Vector3 pos;
	m_vfOverallPos[eScaleX] = data.Position.x;
	m_vfOverallPos[eScaleY] = data.Position.y;
	m_vfOverallPos[eScaleZ] = data.Position.z;
	float scaleX = m_vfScale[eScaleX] * m_vfOverallPos[eScaleZ];
	float scaleY = m_vfScale[eScaleY] * m_vfOverallPos[eScaleZ];
	float scaleZ = m_vfScale[eScaleZ] * m_vfOverallPos[eScaleZ];
	for(int i = 0; i < NUI_SKELETON_POSITION_COUNT; i++)
	{
		m_vfSkeleton[i][eScaleX] = data.SkeletonPositions[i].x;
		m_vfSkeleton[i][eScaleY] = data.SkeletonPositions[i].y;
		m_vfSkeleton[i][eScaleZ] = data.SkeletonPositions[i].z;
		if(id < eKinectBodyPart)
		{
			if(i == m_iBodyID[id])
			{
				if(m_pBody[id] != NULL)
				{
					if(id == 0)
					{
						pos = Ogre::Vector3(m_vfSkeleton[i][eScaleX] * scaleX, 
											m_vfSkeleton[i][eScaleY] * scaleY, 
											m_vfSkeleton[i][eScaleZ] * scaleZ);
					}
					else
					{
						//if(dist_x < (m_vfSkeleton[i][eScaleX] + 0.2) * scaleX)
						//	dist_x = m_vfSkeleton[i][eScaleX] * scaleX;
						//else
						//	dist_x += (0.2 * scaleX);
						Ogre::Vector3 otherPos(m_vfSkeleton[i][eScaleX] * scaleX, 
											m_vfSkeleton[i][eScaleY] * scaleY, 
											m_vfSkeleton[i][eScaleZ] * scaleZ);
						float dist = pos.distance(otherPos);
						if(dist < 1.0 || pos.x > otherPos.x)
							pos.x += 1.0;
						else
							pos = otherPos;
					}
					m_pBody[id]->setOrigin(pos.x,
										pos.y,
										pos.z
					);
					m_pBody[id]->update(0.0);
					id++;
				}
			}
		}
	}

	float posL[3], posR[3];
	m_pBody[eKinectLeftHand]->getPos(posL);
	m_pBody[eKinectRightHand]->getPos(posR);
	float distance = abs(posL[0] - posR[0]) * scaleX;
	float radin = distance / 400;
	for(int i = 0; i < eKinectBodyPart; i++)
	{
		if(m_pBody[i] != NULL)
		{
			if(radin <= MIN_RADIN)
			{
				if(i == eKinectLeftHand)
					m_pBody[i]->setEulerZYX(0.0, 1.0, MIN_RADIN);
				else
					m_pBody[i]->setEulerZYX(0.0, -1.0, -MIN_RADIN);
			}
			else if(radin >= MAX_RADIN)
			{
				if(i == eKinectLeftHand)
					m_pBody[i]->setEulerZYX(0.0, 1.0, MAX_RADIN);
				else
					m_pBody[i]->setEulerZYX(0.0, -1.0, -MAX_RADIN);
			}
			else if(i == eKinectLeftHand)
				m_pBody[i]->setEulerZYX(0.0, 1.0, radin);
			else
				m_pBody[i]->setEulerZYX(0.0, -1.0, -radin);
		}
	}
}

void PhysicKinect::updateDebug(float data[3], float distance)
{
	NUI_SKELETON_FRAME frame = {0};
	int skeletonid = NUI_SKELETON_POSITION_HAND_LEFT;
	//float radin = distance / 13;
	for(int i = 0; i < eKinectBodyPart; i++)
	{
		if(m_pBody[i] != NULL)
		{
			float x;
			if(i == eKinectLeftHand)
				x = (data[0] - distance) / m_vfScale[eScaleX];
			else
			{
				skeletonid = NUI_SKELETON_POSITION_HAND_RIGHT;
				x = (data[0] + distance) / m_vfScale[eScaleX];
			}
			float y = data[1] / m_vfScale[eScaleY];
			float z = data[2] / m_vfScale[eScaleZ];;
			//m_pBody[i]->setOrigin(x, // + data.Position.x,
			//						y, // + data.Position.y,
			//						z // + data.Position.z
			//						);
			//m_pBody[i]->update(0.0);
			//if(radin <= MIN_RADIN)
			//{
			//	if(i == eKinectLeftHand)
			//		m_pBody[i]->setEulerZYX(0.0, 0.0, -MIN_RADIN);
			//	else
			//		m_pBody[i]->setEulerZYX(0.0, 0.0, MIN_RADIN);
			//}
			//else if(radin >= MAX_RADIN)
			//{
			//	if(i == eKinectLeftHand)
			//		m_pBody[i]->setEulerZYX(0.0, 0.0, -MAX_RADIN);
			//	else
			//		m_pBody[i]->setEulerZYX(0.0, 0.0, MAX_RADIN);
			//}
			//else if(i == eKinectLeftHand)
			//	m_pBody[i]->setEulerZYX(0.0, 0.0, -radin);
			//else
			//	m_pBody[i]->setEulerZYX(0.0, 0.0, radin);
			//m_vfSkeleton[skeletonid][eScaleX] = x;
			//m_vfSkeleton[skeletonid][eScaleY] = y;
			//m_vfSkeleton[skeletonid][eScaleZ] = z;
			frame.SkeletonData[0].SkeletonPositions[skeletonid].x = x;
			frame.SkeletonData[0].SkeletonPositions[skeletonid].y = y;
			frame.SkeletonData[0].SkeletonPositions[skeletonid].z = z;
		}
	}
	frame.SkeletonData[0].Position.x = 1.0;
	frame.SkeletonData[0].Position.y = 1.0;
	frame.SkeletonData[0].Position.z = 1.0;
	frame.SkeletonData[0].SkeletonPositions[4].x = frame.SkeletonData[0].SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].x ;
	frame.SkeletonData[0].SkeletonPositions[4].y = frame.SkeletonData[0].SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].y ;
	frame.SkeletonData[0].SkeletonPositions[4].z = frame.SkeletonData[0].SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].z ;
	update(frame.SkeletonData[0]);
}

void PhysicKinect::getPartPos(unsigned int offset, float *pos)
{
	//if(offset < eKinectBodyPart)
	//{
	//	if(m_pBody[offset] != NULL)
	//		m_pBody[offset]->getPos(pos);
	//}
	if(offset < NUI_SKELETON_POSITION_COUNT)
	{
		pos[0] = m_vfSkeleton[offset][0];
		pos[1] = m_vfSkeleton[offset][1];
		pos[2] = m_vfSkeleton[offset][2];
	}
}

float PhysicKinect::getScale(unsigned int offset)
{
	if(offset < eScaleCount)
	{
		return m_vfScale[offset] * m_vfOverallPos[eScaleZ];
	}
	return 0;
}

float PhysicKinect::getMulti(unsigned int offset)
{
	if(offset < eScaleCount)
	{
		return m_vfScale[offset];
	}
	return 0;
}

DWORD PhysicKinect::getID(void)const
{
	return m_iKinectID;
}

void PhysicKinect::setVisible(bool visible)
{
	for(int i =0; i < eKinectBodyPart; i ++)
	{
		if(m_pBody[i] != NULL)
		{
			m_pBody[i]->setVisible(visible);
		}
	}
}