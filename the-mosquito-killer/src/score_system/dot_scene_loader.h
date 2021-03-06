#ifndef DOT_SCENELOADER_H
#define DOT_SCENELOADER_H
 
// Includes
#include <OgreString.h>
#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <vector>
 
// Forward declarations
class TiXmlElement;
 
namespace Ogre
{
	// Forward declarations
	class SceneManager;
	class SceneNode;
	class Entity;
	class Light;
	class Camera; 
	class RenderWindow;
	class Viewport;
	typedef std::vector< SceneNode * > VP_SCENE_NODE;
	typedef std::vector< Entity * > VP_ENTITY;
	typedef std::vector< Light * > VP_LIGHT;
	typedef std::vector< Camera * > VP_CAMERA;

	class NodeProperty
	{
	public:
		String nodeName;
		String propertyNm;
		String valueName;
		String typeName;
 
		NodeProperty(const String &node, const String &propertyName, const String &value, const String &type)
			: nodeName(node), propertyNm(propertyName), valueName(value), typeName(type) {}
	};

	class DynamicObjectData
	{
	public:
		unsigned int m_uiEntityIndex;
		float m_fDelayReplayTime;
		float m_fReplayTime;
		int	m_iMaxTime;
		int	m_iMaxAddTime;
		std::string m_sAniName;
	};
 
	typedef std::vector< DynamicObjectData > VP_DYNAMIC_OBJECT;
	class DotSceneLoader
	{
	public:
		DotSceneLoader() : m_pSceneMgr(0), m_pWindow(0), m_pCurrentVP(0) {}
		virtual void release();
		virtual ~DotSceneLoader() {release();}
		virtual void setAllVisible(bool visible);
		virtual void update(float timePass);
		
		void parseDotScene(const String &SceneName, const String &groupName, SceneManager *yourSceneMgr, Ogre::RenderWindow* pWindow, SceneNode *pAttachNode = NULL, const String &sPrependNode = "");
		String getProperty(const String &ndNm, const String &prop);
		
	private:
		std::vector<NodeProperty> nodeProperties;
		std::vector<String> staticObjects;
		std::vector<String> dynamicObjects;
		VP_DYNAMIC_OBJECT m_vDynamicDatas;
		RenderWindow* m_pWindow;
		Viewport* m_pCurrentVP;
		VP_SCENE_NODE m_vpSceneNode;
		VP_ENTITY m_vpEntity;
		VP_LIGHT m_vpLight;
		VP_CAMERA m_vpCamera;
 
	protected:
		void processScene(TiXmlElement *XMLRoot);
 
		void processNodes(TiXmlElement *XMLNode);
		void processExternals(TiXmlElement *XMLNode);
		void processEnvironment(TiXmlElement *XMLNode);
		void processTerrain(TiXmlElement *XMLNode);
		void processUserDataReference(TiXmlElement *XMLNode, SceneNode *pParent = 0);
		void processUserDataReference(TiXmlElement *XMLNode, Entity *pEntity);
		void processOctree(TiXmlElement *XMLNode);
		void processLight(TiXmlElement *XMLNode, SceneNode *pParent = 0);
		void processCamera(TiXmlElement *XMLNode, SceneNode *pParent = 0);
		void processViewport(Camera *pCamera);
 
		void processNode(TiXmlElement *XMLNode, SceneNode *pParent = 0);
		void processLookTarget(TiXmlElement *XMLNode, SceneNode *pParent);
		void processTrackTarget(TiXmlElement *XMLNode, SceneNode *pParent);
		void processEntity(TiXmlElement *XMLNode, SceneNode *pParent);
		void processParticleSystem(TiXmlElement *XMLNode, SceneNode *pParent);
		void processBillboardSet(TiXmlElement *XMLNode, SceneNode *pParent);
		void processPlane(TiXmlElement *XMLNode, SceneNode *pParent);
 
		void processFog(TiXmlElement *XMLNode);
		void processSkyBox(TiXmlElement *XMLNode);
		void processSkyDome(TiXmlElement *XMLNode);
		void processSkyPlane(TiXmlElement *XMLNode);
		void processClipping(TiXmlElement *XMLNode);
 
		void processLightRange(TiXmlElement *XMLNode, Light *pLight);
		void processLightAttenuation(TiXmlElement *XMLNode, Light *pLight);
 
		String getAttrib(TiXmlElement *XMLNode, const String &parameter, const String &defaultValue = "");
		Real getAttribReal(TiXmlElement *XMLNode, const String &parameter, Real defaultValue = 0);
		bool getAttribBool(TiXmlElement *XMLNode, const String &parameter, bool defaultValue = false);
 
		Vector3 parseVector3(TiXmlElement *XMLNode);
		Quaternion parseQuaternion(TiXmlElement *XMLNode);
		ColourValue parseColour(TiXmlElement *XMLNode);
 
 
		SceneManager *m_pSceneMgr;
		SceneNode *m_pSceneRoot;
		String m_sGroupName;
		String m_sPrependNode;
	};
}
 
#endif // DOT_SCENELOADER_H