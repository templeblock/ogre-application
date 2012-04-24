#ifndef _MOSQUITO_H_
#define _MOSQUITO_H_

namespace Ogre
{
	class SceneManager;
}
class PhysicRigidBody;
class MoveBase;
class btDynamicsWorld;
class ScoreBase;
//*******************************************************
//***************  MOSQUITO_BASE  ***********************
//*******************************************************
enum MOSQUITO_TYPE
{
	eMosquitoBase = 0,
	eMosquitoSplit,
	eMosquitoFat,
};
class MosquitoBase
{
private:
	Ogre::SceneManager *		m_pSceneMgr;
	btDynamicsWorld *			m_pWorld;
	PhysicRigidBody*			m_pBody;
	MoveBase*					m_pMove;
	ScoreBase*					m_pScore;
protected:
	bool						m_bDestory;
public:
								MosquitoBase(void);
	virtual						~MosquitoBase(void);
	virtual void				init(Ogre::SceneManager *scene, btDynamicsWorld *world);
	virtual void				create(const char *modelName, float mass, float *scale, float *pos, float *quat, int score);
	virtual void				release(void);
	virtual void				update(void);
	virtual MOSQUITO_TYPE		getType(void)				{return eMosquitoBase;}
	virtual bool				isDestory(void)const		{return m_bDestory;}
	virtual void				setDestory(void)			{m_bDestory = 1;}
};
//*******************************************************
//***************  MOSQUITO_SPLIT  **********************
//*******************************************************
class MosquitoSplit: public MosquitoBase
{
private:
	int							m_iSplitNumber;
public:
								MosquitoSplit(void);
	virtual						~MosquitoSplit(void);
	virtual void				init(Ogre::SceneManager *scene, btDynamicsWorld *world);
	virtual void				create(const char *modelName, float mass, float *scale, float *pos, float *quat, int score);
	virtual void				release(void);
	virtual void				update(void);
	void						setSplitNumber(int number)	{m_iSplitNumber = number;}
	int							getSplitNumber()			{return m_iSplitNumber;}
	virtual MOSQUITO_TYPE		getType(void)				{return eMosquitoSplit;}
};
//*******************************************************
//****************  MOSQUITO_FAT  ***********************
//*******************************************************
class MosquitoFat: public MosquitoBase
{
private:
	int							m_iBloodNumber;
public:
								MosquitoFat(void);
	virtual						~MosquitoFat(void);
	virtual void				init(Ogre::SceneManager *scene, btDynamicsWorld *world);
	virtual void				create(const char *modelName, float mass, float *scale, float *pos, float *quat, int score);
	virtual void				release(void);
	virtual void				update(void);
	virtual MOSQUITO_TYPE		getType(void)				{return eMosquitoFat;}
	void						setBloodNumber(int number)	{m_iBloodNumber = number;}
	void						decreaseBlood()				{m_iBloodNumber--;}
	virtual bool				isDestory(void)const		{return m_bDestory || m_iBloodNumber <= 0;}
};
//*******************************************************
//********************  END  ****************************
//*******************************************************
#endif //_MOSQUITO_H_