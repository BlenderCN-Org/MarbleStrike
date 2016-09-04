#ifndef IPOCURVE_HPP
#define IPOCURVE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/vector4.hpp"
#include "common/engine/system/array.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

#define IPO_CURVE_TUPLE \
	IPO_CURVE_ENTRY(IPO_LOCX, "LocX" ) \
	IPO_CURVE_ENTRY(IPO_LOCY, "LocY" ) \
	IPO_CURVE_ENTRY(IPO_LOCZ, "LocZ" ) \
	IPO_CURVE_ENTRY(IPO_ROTX, "RotX" ) \
	IPO_CURVE_ENTRY(IPO_ROTY, "RotY" ) \
	IPO_CURVE_ENTRY(IPO_ROTZ, "RotZ" ) \

enum IpoCurveTypes
{
#define IPO_CURVE_ENTRY(ENUM, NAME) ENUM,
	IPO_CURVE_TUPLE
#undef IPO_CURVE_ENTRY
	IPO_CURVE_MAX
};

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

struct BezierTriple
{
	Math::Vector4 mPoint[3];
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class IpoCurve
{
public:

    IpoCurve();
	~IpoCurve();

	void Update( float elapsedTime );

	void AddBezierTriple( BezierTriple triple )
	{
		mBezierTriples.PushBack( triple );
	}

	int GetNumTriples()
	{
		return mBezierTriples.GetSize();
	}

	void SetInitialValue( float value )
	{
		mValue = value;
	}

	float GetValue()
	{
		return mValue;
	}
	
public:

	Array<BezierTriple> mBezierTriples;

	float mCurrentFrame;
	int mKeyFrameIndex;
	int mNextKeyFrameIndex;

	float mValue;
};

#endif