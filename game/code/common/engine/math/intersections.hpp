///  \brief Intersection Methods
///
///  A more elaborate class description.
///

#ifndef INTERSECTIONS_HPP
#define INTERSECTIONS_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/aabb.hpp"
#include "common/engine/math/sphere.hpp"
#include "common/engine/math/vector3.hpp"
#include "common/engine/math/mathfunctions.hpp"
#include "common/engine/math/frustum.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

namespace Math
{
	class Ray;
}

class Mesh;

//////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
//////////////////////////////////////////////////////

namespace Math
{
 
  //
  // Static Sphere - Sphere.
  //
  bool IntersectStatic( const Math::Sphere& sphere1, const Math::Sphere& sphere2 );

  //
  // Static Sphere - Plane
  //
  bool IntersectStatic( const Math::Sphere& sphere, const Math::Plane& plane );

  //
  // Static AABB - AABB
  //
  bool IntersectStatic( const Math::AABB& aabb1, const Math::AABB& aabb2 );

  //
  // Static AABB - Triangle
  //
  bool IntersectStatic( const Math::AABB& aabb, const Math::Triangle& triangle );

  //
  // Static Triangle - Triangle
  //
  bool IntersectStatic( const Math::Triangle& triangle1, const Math::Triangle& triangle2 );

  //
  // Static AABB - Sphere
  //
  bool IntersectStatic( const Math::AABB& aabb, const Math::Sphere& sphere );

  //
  // Static AABB - Plane
  //
  bool IntersectStatic( const Math::AABB& aabb, const Math::Plane& plane );

  //
  // Static AABB - Point
  //
  bool IntersectStatic( const Math::AABB& aabb, const Math::Vector3 &point );

  //
  // Static Triangle - Point
  //
  bool IntersectStatic( const Math::Triangle& aabb, const Math::Vector3 &point );

  //
  // Static Sphere - Ray
  //
  bool IntersectStatic( const Math::Sphere& sphere, const Math::Ray& ray, float& nearest, float& farthest );

  //
  // Static Triangle - Ray
  //
  bool IntersectStatic( const Math::Triangle& triangle, const Math::Ray &ray, float& point );

  //
  // Static Plane - Ray
  //
  bool IntersectStatic( const Math::Plane& plane, const Math::Ray& ray, float& point );  

  //
  // Static AABB - Ray
  //
  bool IntersectStatic( const Math::AABB& aabb, const Math::Ray& ray, float& point );  

  //
  // Static Frustum - Sphere
  //
  bool IntersectStatic( const Math::Frustum& frustum, const Math::Sphere& sphere );

  //
  // Static Frustum - AABB
  //
  bool IntersectStatic( const Math::Frustum& frustum, const Math::AABB &aabb );

  //
  // Static Mesh - Ray
  //
  bool IntersectStatic( const Mesh* mesh, const Math::Matrix44 &transform, const Math::Ray &ray, float &t );

  //
  // Static 2D OOBB - OOBB
  // oobb vertices specified in counter clock-wise
  //
  bool IntersectStatic2D_OOBB_OOBB( const Math::Vector3 oobb1[4], const Math::Vector3 oobb2[4] );

  //
  // Dynamic Sphere - Sphere
  //
  bool IntersectDynamic( const Math::Sphere& sphere1, const Math::Sphere& sphere2, float& time );

  //
  // Dynamic Sphere - Plane
  //
  bool IntersectDynamic( const Math::Sphere& sphere, const Math::Plane& plane, float& time );

  //
  // Dynamic AABB - AABB
  //
  bool IntersectDynamic( const Math::AABB& aabb1, const Math::AABB& aabb2, float& time );

  //
  // Dynamic Sphere - Triangle
  //
  bool IntersectDynamic( const Math::Sphere& sphere, const Math::Triangle& triangle, float& time );

}

#endif
