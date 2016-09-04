#ifndef CUBIC_BEZIER_SPLINE_HPP
#define CUBIC_BEZIER_SPLINE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/vector3.hpp"
#include "common/engine/system/fixedarray.hpp"
#include "tinyxml2.h"
#include <vector>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

struct BezierPoint
{
	Math::Vector3 mPosition;
	Math::Vector3 mHandleLeft;
	Math::Vector3 mHandleRight;
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class CubicBezierSpline
{
public:

	static float CubicBezierEvaluate( float t, float p0, float p1, float p2, float p3 );

	CubicBezierSpline();
	~CubicBezierSpline();

	void SetNumPoints( int size );

	Math::Vector3 Evaluate( float t );

	void Load( tinyxml2::XMLNode* node );

	float GetTotalLength();
	float GetPercentageFromLength( float length );

private:

	struct PositionTableData
	{
		float mT;
		float mLength;
	};

	FixedArray<BezierPoint*> mBezierPoints;
	std::vector<PositionTableData> mPositionTable;
	float mTotalLength;
    bool mIsCyclic;
};

#endif