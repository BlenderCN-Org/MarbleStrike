#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/assert.hpp"
#include "common/engine/math/vector3.hpp"

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace Math
{ 
	class Triangle
	{
	public:

		Triangle();
		Triangle( Math::Vector3& v0, Math::Vector3& v1, Math::Vector3& v2 );    
		~Triangle();

		Math::Vector3& operator[]( unsigned int i )
		{
			Assert( i >= 0 && i < 3, "Access array out of bounds" );
			return mVertex[i];
		}

		Math::Vector3 operator[]( unsigned int i ) const
		{
			Assert( i >= 0 && i < 3, "Access array out of bounds" );
			return mVertex[i];
		}

		Math::Vector3 GetNormal() const 
		{ 
			return mNormal;
		}

		void SetNormal( const Math::Vector3& normal ) 
		{       
			mNormal = normal; 
		}

	private:

		Math::Vector3 mVertex[3];
		Math::Vector3 mNormal;	
	};
}

#endif
