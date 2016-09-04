//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/followpathconstraint.hpp"
#include "common/engine/system/stringutilities.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

FollowPathConstraint::FollowPathConstraint() 
{	
	mTarget = NULL;
}

//============================================================================

FollowPathConstraint::~FollowPathConstraint()
{
}

//============================================================================

void FollowPathConstraint::Load( tinyxml2::XMLNode* node )
{
	tinyxml2::XMLElement* element = node->ToElement();
		
	const char* targetName = element->Attribute( "target" );
	System::StringCopy( mTargetName, 64, targetName );
}