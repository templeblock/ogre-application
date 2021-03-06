#ifndef _STROKE_MANAGER_H_
#define _STROKE_MANAGER_H_

#include "stroke.h"
#include <vector>
#include <list>

namespace Stroke
{
	typedef std::vector< Stroke *>	V_STROKE;
	typedef std::list< Stroke *>	LIST_STROKE;
	class StrokeDraw;
	class StrokeManager
	{
	private:
									StrokeManager(void);
		static StrokeManager *		m_pInstance;
		//V_STROKE					m_vStroke;
		LIST_STROKE					m_listStroke;
		float						m_fDelayTime;
		Point						m_preCenter;
	public:
									~StrokeManager(void);
		static StrokeManager *		getSingleton(void);
		void						release(void);
		void						update(float timePass, float **field, float *density, unsigned int size);
		void						createStroke(float existTime, const V_POINT &vPoint, StrokeDraw *drawNegative, StrokeDraw *drawPositive, const Point &center);
	};
}
#endif //_STROKE_MANAGER_H_