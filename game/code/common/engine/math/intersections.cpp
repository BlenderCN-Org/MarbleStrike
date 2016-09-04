//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/intersections.hpp"
#include "common/engine/math/triangle.hpp"
#include "common/engine/math/ray.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/render/mesh.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace Math
{

    bool IntersectStatic( const Math::Sphere &sphere1, const Math:: Sphere &sphere2 )
    {
        Math::Vector3 distance = sphere1.GetPosition() - sphere2.GetPosition();
		float lengthSquared = Math::Vector3::Dot( distance, distance );
        float totalRadius = sphere1.GetRadius() + sphere2.GetRadius();

        if ( lengthSquared > ( totalRadius * totalRadius ) ) 
        {
            return false;
        }
        else
        {
            return true;
        }    
    }

    //===========================================================================

    bool IntersectStatic( const Math::Sphere& sphere, const Plane& plane )
    {
        float distance = plane.DistanceToPlane( sphere.GetPosition() );

        if ( distance <= sphere.GetRadius() )
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    //===========================================================================

    bool IntersectStatic( const Math::AABB& aabb1, const Math::AABB& aabb2 )
    {
        Math::Vector3 min1 = aabb1.GetMin();
        Math::Vector3 max1 = aabb1.GetMax();
        Math::Vector3 min2 = aabb2.GetMin();
        Math::Vector3 max2 = aabb2.GetMax();

        if ( min1[0] <= max2[0] && min1[1] <= max2[1] && min1[2] <= max2[2] &&
            max1[0] >= min2[0] && max1[1] >= min2[1] && max1[2] >= min2[2] )
        {
            return true;
        }
        else
        {
            return false;
        }
    }

	//===========================================================================

	int planeBoxOverlap( const Math::Vector3 &normal, float d, const Math::Vector3 &maxbox )
	{
		int q;
		Math::Vector3 vmin;
		Math::Vector3 vmax;

		for ( q = 0; q <= 2; q++ )
		{
			if ( normal[q] > 0.0f )
			{
				vmin[q] = -maxbox[q];
				vmax[q] = maxbox[q];
			}
			else
			{
				vmin[q] = maxbox[q];
				vmax[q] = -maxbox[q];
			}
		}
		if ( Math::Vector3::Dot( normal, vmin ) + d > 0.0f )
		{
			return 0;
		}

		if ( Math::Vector3::Dot( normal, vmax ) + d >= 0.0f ) 
		{
			return 1;
		}

		return 0;
	}

	bool IntersectStatic( const Math::AABB& aabb, const Math::Triangle& triangle )
	{		
		Math::Vector3 v0;
		Math::Vector3 v1;
		Math::Vector3 v2;

		Math::Vector3 e0;
		Math::Vector3 e1;
		Math::Vector3 e2;

		float min,max,d,p0,p1,p2,rad,fex,fey,fez;
		Math::Vector3 normal;

		v0 = triangle[0] - aabb.GetPosition();
		v1 = triangle[1] - aabb.GetPosition();
		v2 = triangle[2] - aabb.GetPosition();
				
		// Compute triangle edges
		e0 = v1 - v0;
		e1 = v2 - v1;
		e2 = v0 - v2;		
		
		// Test the 9 tests first (this was faster)
		fex = fabs( e0[0] );
		fey = fabs( e0[1] );
		fez = fabs( e0[2] );
		{
			p0 = e0[2] * v0[1] - e0[1] * v0[2];
			p2 = e0[2] * v2[1] - e0[1] * v2[2];
			if ( p0 < p2 ) 
			{
				min = p0; 
				max = p2;
			} 
			else 
			{ 
				min = p2; 
				max = p0;
			}
			
			rad = fez * aabb.GetHalfSides()[1] + fey * aabb.GetHalfSides()[2];
			if ( min > rad || max < -rad ) 
			{
				return false;
			}
		}
		{			
			p0 = -e0[2] * v0[0] + e0[0] * v0[2];
			p2 = -e0[2] * v2[0] + e0[0] * v2[2];
			if ( p0 < p2 ) 
			{
				min = p0; 
				max = p2;
			} 
			else 
			{
				min = p2; 
				max = p0;
			}
			rad = fez * aabb.GetHalfSides()[0] + fex * aabb.GetHalfSides()[2];
			if ( min > rad || max < -rad ) 
			{
				return false;
			}
		}
		{
			p1 = e0[1] * v1[0] - e0[0] * v1[1];
			p2 = e0[1] * v2[0] - e0[0] * v2[1];
			if ( p2 < p1 ) 
			{ 
				min = p2; 
				max = p1;
			} 
			else 
			{
				min = p1; 
				max = p2;
			}
			rad = fey * aabb.GetHalfSides()[0] + fex * aabb.GetHalfSides()[1];
			if ( min > rad || max < -rad ) 
			{
				return false;
			}
		}
		//AXISTEST_X01(e0[2], e0[1], fez, fey);
		//AXISTEST_Y02(e0[2], e0[0], fez, fex);
		//AXISTEST_Z12(e0[1], e0[0], fey, fex);

		fex = fabs(e1[0]);
		fey = fabs(e1[1]);
		fez = fabs(e1[2]);
		{
			p0 = e1[2] * v0[1] - e1[1] * v0[2];
			p2 = e1[2] * v2[1] - e1[1] * v2[2];
			if ( p0 < p2 ) 
			{
				min = p0; 
				max = p2;
			} 
			else 
			{ 
				min = p2; 
				max = p0;
			}

			rad = fez * aabb.GetHalfSides()[1] + fey * aabb.GetHalfSides()[2];
			if ( min > rad || max < -rad ) 
			{
				return false;
			}
		}
		{			
			p0 = -e1[2] * v0[0] + e1[0] * v0[2];
			p2 = -e1[2] * v2[0] + e1[0] * v2[2];
			if ( p0 < p2 ) 
			{
				min = p0; 
				max = p2;
			} 
			else 
			{
				min = p2; 
				max = p0;
			}
			rad = fez * aabb.GetHalfSides()[0] + fex * aabb.GetHalfSides()[2];
			if ( min > rad || max < -rad ) 
			{
				return false;
			}
		}
		{			
			p0 = e1[1] * v0[0] - e1[0] * v0[1];
			p1 = e1[1] * v1[0] - e1[0] * v1[1];
			if ( p0 < p1 ) 
			{ 
				min = p0; 
				max = p1;
			} 
			else 
			{
				min = p1; 
				max = p0;
			}
			rad = fey * aabb.GetHalfSides()[0] + fex * aabb.GetHalfSides()[1];
			if ( min > rad || max < -rad ) 
			{
				return false;
			}
		}
		//AXISTEST_X01(e1[2], e1[1], fez, fey);
		//AXISTEST_Y02(e1[2], e1[0], fez, fex);
		//AXISTEST_Z0(e1[1], e1[0], fey, fex);

		fex = fabs(e2[0]);
		fey = fabs(e2[1]);
		fez = fabs(e2[2]);
		{
			p0 = e2[2] * v0[1] - e2[1] * v0[2];
			p1 = e2[2] * v1[1] - e2[1] * v1[2];
			if ( p0 < p1 ) 
			{
				min = p0; 
				max = p1;
			} 
			else 
			{
				min = p1; 
				max = p0;
			}
			rad = fez * aabb.GetHalfSides()[1] + fey * aabb.GetHalfSides()[2];
			if ( min > rad || max < -rad ) 
			{
				return false;
			}
		}
		{
			p0 = -e2[2] * v0[0] + e2[0] * v0[2];
			p1 = -e2[2] * v1[0] + e2[0] * v1[2];
			if ( p0 < p1 ) 
			{
				min = p0; 
				max = p1;
			} 
			else 
			{ 
				min = p1; 
				max = p0;
			}
			rad = fez * aabb.GetHalfSides()[0] + fex * aabb.GetHalfSides()[2];
			if ( min > rad || max < -rad ) 
			{
				return false;
			}
		}
		{
			p1 = e2[1] * v1[0] - e2[0] * v1[1];
			p2 = e2[1] * v2[0] - e2[0] * v2[1];
			if ( p2 < p1 ) 
			{ 
				min = p2; 
				max = p1;
			} 
			else 
			{
				min = p1; 
				max = p2;
			}
			rad = fey * aabb.GetHalfSides()[0] + fex * aabb.GetHalfSides()[1];
			if ( min > rad || max < -rad ) 
			{
				return false;
			}
		}
		//AXISTEST_X2(e2[2], e2[1], fez, fey);
		//AXISTEST_Y1(e2[2], e2[0], fez, fex);
		//AXISTEST_Z12(e2[1], e2[0], fey, fex);
		
		//  first test overlap in the {x,y,z}-directions
		//  find min, max of the triangle each direction, and test for overlap in
		//  that direction -- this is equivalent to testing a minimal AABB around 
		//  the triangle against the AABB
		
		{
			min = max = v0[0];
			if ( v1[0] < min ) 
			{
				min = v1[0];
			}
			if ( v1[0] > max ) 
			{
				max = v1[0];
			}
			if ( v2[0] < min ) 
			{
				min = v2[0];
			}
			if ( v2[0] > max ) 
			{
				max = v2[0];
			}
		}
		//FINDMINMAX(v0[X],v1[X],v2[X],min,max);
		if ( min > aabb.GetHalfSides()[0] || max < -aabb.GetHalfSides()[0] ) 
		{
			return false;
		}

		{
			min = max = v0[1];
			if ( v1[1] < min ) 
			{
				min = v1[1];
			}
			if ( v1[1] > max ) 
			{
				max = v1[1];
			}
			if ( v2[1] < min ) 
			{
				min = v2[1];
			}
			if ( v2[1] > max ) 
			{
				max = v2[1];
			}
		}
		//FINDMINMAX(v0[Y],v1[Y],v2[Y],min,max);
		if ( min > aabb.GetHalfSides()[1] || max < -aabb.GetHalfSides()[1] ) 
		{
			return false;
		}

		{
			min = max = v0[1];
			if ( v1[2] < min ) 
			{
				min = v1[2];
			}
			if ( v1[2] > max ) 
			{
				max = v1[2];
			}			
			if ( v2[2] < min ) 
			{
				min = v2[2];
			}
			if ( v2[2] > max ) 
			{
				max = v2[2];
			}
		}
		//FINDMINMAX(v0[Z],v1[Z],v2[Z],min,max);
		if ( min > aabb.GetHalfSides()[2] || max < -aabb.GetHalfSides()[2] ) 
		{
			return false;

		}
		
		// Test if the box intersects the plane of the triangle
		// compute plane equation of triangle: normal*x+d=0
		normal = Math::Vector3::Cross( e0, e1 );
		d = Math::Vector3::Dot( -normal, v0 );  // plane eq: normal.x+d=0
		if ( !planeBoxOverlap( normal, d, aabb.GetHalfSides() ) ) 
		{
			return false;
		}

		return true;
	}

    //===========================================================================

    bool IntersectStatic( const Math::Triangle& /*triangle1*/, const Math::Triangle& /*triangle2*/ )
    {
        return true;
    }

    //===========================================================================

    bool IntersectStatic( const Math::AABB& aabb, const Math::Sphere& sphere )
    {  
        Math::Vector3 min = aabb.GetMin();
        Math::Vector3 max = aabb.GetMax();

        float d = 0;
        for ( int i = 0; i < 3; i++ )
        {
            if ( sphere.GetPosition()[i] < min[i] )
            {
                d = d + ( sphere.GetPosition()[i] - min[i] ) * ( sphere.GetPosition()[i] - min[i] );
            }
            else if ( sphere.GetPosition()[i] > max[i] )
            {        
                d = d + ( sphere.GetPosition()[i] - max[i] ) * ( sphere.GetPosition()[i] - max[i] );
            }
        }

        if ( d > sphere.GetRadius() * sphere.GetRadius() )
        {
            return false;
        }

        return true;
    }

     //===========================================================================

    bool IntersectStatic( const Math::AABB& aabb, const Math::Plane& plane )
    {  
        Math::Vector3 min = aabb.GetMin();
        Math::Vector3 max = aabb.GetMax();
        Math::Vector3 c = ( max + min ) / 2.0f;
        Math::Vector3 h = ( max - min ) / 2.0f;
        Math::Vector3 n = -plane.GetNormal();
        float e = h[0] * fabs( n[0] )  + h[1] * fabs( n[1] ) + h[2] * fabs( n[2] );
        float s = Math::Vector3::Dot( c, n ) - plane.GetCoeff()[3];
        if ( s - e > 0 )
        {
            return false;
        }

        return true;
    }

    //===========================================================================

    bool IntersectStatic( const Math::AABB& aabb, const Math::Vector3& point )
    {
        Math::Vector3 min = aabb.GetMin();
        Math::Vector3 max = aabb.GetMax();

        if ( point[0] >= min[0] && point[0] <= max[0] &&
            point[1] >= min[1] && point[1] <= max[1] &&
            point[2] >= min[2] && point[2] <= max[2] )
        {
            return true;
        }
        else
        {
            return false;		
        }
    }

	//===========================================================================

	bool IntersectStatic( const Math::Triangle& triangle, const Math::Vector3& point )
	{
		// Compute vectors        
		Math::Vector3 v0 = triangle[1] - triangle[0];
		Math::Vector3 v1 = triangle[2] - triangle[0];
		Math::Vector3 v2 = point - triangle[0];

		// Compute dot products
		float dot00 = Math::Vector3::Dot( v0, v0 );
		float dot01 = Math::Vector3::Dot( v0, v1 );
		float dot02 = Math::Vector3::Dot( v0, v2 );
		float dot11 = Math::Vector3::Dot( v1, v1 );
		float dot12 = Math::Vector3::Dot( v1, v2 );

		// Compute barycentric coordinates
		float invDenom = 1.0f / ( dot00 * dot11 - dot01 * dot01 );
		float u = ( dot11 * dot02 - dot01 * dot12 ) * invDenom;
		float v = ( dot00 * dot12 - dot01 * dot02 ) * invDenom;

		// Check if point is in triangle
		return ( u > 0 ) && ( v > 0 ) && ( u + v <= 1 );
	}

    //===========================================================================

    bool IntersectStatic( const Math::Sphere& sphere, const Ray& ray, float& nearest, float& farthest )
    {
		Math::Vector3 diff = ray.GetOrigin() - sphere.GetPosition();
        float a = Math::Vector3::Dot( ( ray.GetDirection() ), ray.GetDirection() );
        float b = 2.0f * Math::Vector3::Dot( ( ray.GetDirection() ), diff );
        float c = Math::Vector3::Dot( diff, diff ) - ( sphere.GetRadius() * sphere.GetRadius() );

        float discriminant = b*b - 4*a*c;

        if ( discriminant >= 0 ) 
        {
            float inverse = 1/( 2*a );
            float t1 = static_cast<float>( ( -b - sqrt( discriminant ) ) * inverse );
            float t2 = static_cast<float>(  ( -b + sqrt( discriminant ) ) * inverse );

            if ( t1 >= 0  || t2 >= 0 )
            {
                if ( t1 < 0.001f ) t1 = Math::INFINITY_NUM;
                if ( t2 < 0.001f ) t2 = Math::INFINITY_NUM;

                if ( t1 < t2 )
                {
                    nearest = t1;
                    farthest = t2;
                }
                else 
                {
                    nearest = t2;
                    farthest = t1;
                }

                return true;
            }
        }

        return false;

    }

    //===========================================================================

    bool IntersectStatic( const Math::Triangle& triangle, const Ray& ray, float& point )
    {
        float dot = Math::Vector3::Dot( ray.GetDirection(), triangle.GetNormal() );
        if ( dot >= 0 ) 
        {
            return false;
        }

        float a = triangle[0][0] - triangle[1][0];
        float b = triangle[0][1] - triangle[1][1];
        float c = triangle[0][2] - triangle[1][2];
        float d = triangle[0][0] - triangle[2][0];
        float e = triangle[0][1] - triangle[2][1];
        float f = triangle[0][2] - triangle[2][2];
        float g = ray.GetDirection()[0];
        float h = ray.GetDirection()[1];
        float ii = ray.GetDirection()[2];
        float jj = triangle[0][0] - ray.GetOrigin()[0];
        float k = triangle[0][1] - ray.GetOrigin()[1];
        float l = triangle[0][2] - ray.GetOrigin()[2];

        float m = 1.0f / ( a * ( e * ii - h * f ) + b * ( g * f - d * ii ) + c * ( d * h - e * g ) );
        float beta = ( jj * ( e * ii - h * f ) + k * ( g * f - d * ii )+ l * ( d * h - e * g ) ) * m;
        float gamma = ( ii * ( a * k - jj * b ) + h * ( jj * c - a * l ) + g * ( b * l - k * c ) ) * m;
        float t = -( f * ( a * k - jj * b ) + e * ( jj * c - a * l ) + d * ( b * l - k * c ) ) * m;

        if ( beta + gamma <= 1.001 && beta >= 0 && gamma >= 0 ) 
        {		            
            point = t;
            return true;		
        }

        return false;  
    }

    //===========================================================================

    bool IntersectStatic( const Plane& plane, const Ray& ray, float& point )
    {

        // N + d = 0
        // point = o + tdir
        // N.(o+tdir) = -d
        // N.o + N.tdir = -d
        // -d - N.o  / N.dir = t    

        float d = Math::Vector3::Dot( plane.GetNormal(), plane.GetPosition() ) * -1; 
        float top = -d + Math::Vector3::Dot( plane.GetNormal(), ray.GetOrigin() );
        float bottom = Math::Vector3::Dot( plane.GetNormal(), ray.GetDirection() );

        if ( bottom != 0 ) 
        {
            float t = top / bottom;

            if ( t >= 0 )
            {
                point = t;
                return true;
            }
        }

        return false;
    }

    //===========================================================================

    bool IntersectStatic( const Math::AABB& aabb, const Math::Ray& ray, float& point )
    {
        float tmin = -Math::INFINITY_NUM;
        float tmax = Math::INFINITY_NUM;

        Math::Vector3 min = aabb.GetMin();
        Math::Vector3 max = aabb.GetMax();

        for ( int i = 0; i < 3; i++ )
        {
            if ( ray.GetDirection()[i] != 0 )
            {
                float t1 = ( max[i] - ray.GetOrigin()[i] ) / ray.GetDirection()[i];
                float t2 = ( min[i] - ray.GetOrigin()[i] ) / ray.GetDirection()[i];

                if ( t1 > t2 )
                {
                    // Swap
                    float temp = t1;
                    t1 = t2;
                    t2 = temp;
                }

                if ( t1 > tmin ) tmin = t1;
                if ( t2 < tmax ) tmax = t2;

                if ( tmin > tmax ) return false;
                if ( tmax < 0 ) return false;
            }
        }

        if ( tmin > 0 )
        {
            point = tmin;
        }
        else
        {
            point = tmax;
        }

        return true;
    }

    //===========================================================================

    bool IntersectStatic( const Math::Frustum& frustum, const Math::Sphere& sphere )
    {    
        for ( unsigned int i = 0; i < 6; i++ ) 
        {
            float distance = Math::Vector3::Dot( 
				frustum.GetPlane( i ).GetNormal(), 
				sphere.GetPosition() ) 
				+ frustum.GetPlane( i ).GetCoeff()[3];

            if ( distance <= -sphere.GetRadius() )      
            {
                return false;
            }
        }

        return true;
    }

    //===========================================================================

    bool IntersectStatic( const Math::Frustum& frustum, const Math::AABB &aabb )
    {
        for ( int i = 0; i < 6; ++i )
        {
            Math::Plane plane = frustum.GetPlane( i );
            bool result = IntersectStatic( aabb, plane );
            if ( result == false )
            {
                return false;
            }
        }

        return true;
    }

	//===========================================================================

	bool IntersectStatic( const Mesh* mesh, const Math::Matrix44 &transform, const Math::Ray &ray, float &t )
	{	
		bool intersect = false;

		int faceIndicesStride = mesh->GetFaceIndicesStride();
		int positionsStride = mesh->GetPositionsStride();

		unsigned int* faceIndices = mesh->GetFaceIndices();
		float* positions = mesh->GetPositions();

		int numPrimitives = mesh->GetNumPrimitives();
		for ( int i = 0; i < numPrimitives; ++i )
		{
			unsigned int i0 = faceIndices[ i * faceIndicesStride ];
			unsigned int i1 = faceIndices[ i * faceIndicesStride + 1 ];
			unsigned int i2 = faceIndices[ i * faceIndicesStride + 2 ];
			Math::Vector3 v0(
				positions[ i0 * positionsStride ],
				positions[ i0 * positionsStride + 1],
				positions[ i0 * positionsStride + 2]
			);
			Math::Vector3 v1(
				positions[ i1 * positionsStride ],
				positions[ i1 * positionsStride + 1],
				positions[ i1 * positionsStride + 2]
			);
			Math::Vector3 v2( 
				positions[ i2 * positionsStride ],
				positions[ i2 * positionsStride + 1],
				positions[ i2 * positionsStride + 2]
			);
			v0 = transform * v0;
			v1 = transform * v1;
			v2 = transform * v2;
			Math::Triangle triangle( v0, v1, v2 );

			float tempT = 0;
			if ( Math::IntersectStatic( triangle, ray, tempT ) )
			{
				if ( tempT < t )
				{
					t = tempT;
				}
				intersect = true;
			}
		}

		return intersect;
	}

	//===========================================================================

	bool IntersectStatic2D_OOBB_OOBB( const Math::Vector3 oobb1[4], const Math::Vector3 oobb2[4] )
	{
		Math::Vector3 axis1[4];
		axis1[0] = oobb1[1] - oobb1[0];
		axis1[1] = oobb1[2] - oobb1[1];
		axis1[2] = oobb1[3] - oobb1[2];
		axis1[3] = oobb1[0] - oobb1[3];

		Math::Vector3 axis2[4];
		axis2[0] = oobb2[1] - oobb2[0];
		axis2[1] = oobb2[2] - oobb2[1];
		axis2[2] = oobb2[3] - oobb2[2];
		axis2[3] = oobb2[0] - oobb2[3];

		for ( int i = 0; i < 4; ++i )
		{
			Math::Vector3 axis( axis1[i][1], -axis1[i][0], 0 );
			axis = Math::Vector3::Normalize( axis );

			float amin = 9999;
			float amax = -9999;

			for ( int index = 0; index < 4; ++index )
			{
				float project = Math::Vector3::Dot( axis, oobb1[index] );
				if ( project < amin )
				{
					amin = project;
				}
				if ( project > amax )
				{
					amax = project;
				}		
			}

			float bmin = 9999;
			float bmax = -9999;

			for ( int index = 0; index < 4; ++index )
			{
				float project = Math::Vector3::Dot( axis, oobb2[index] );
				if ( project < bmin )
				{
					bmin = project;
				}
				if ( project > bmax )
				{
					bmax = project;
				}		
			}
			if ( amin > bmax || bmin > amax )
			{
				return false;
			}
		}


		for ( int i = 0; i < 4; ++i )
		{
			Math::Vector3 axis( axis2[i][1], -axis2[i][0], 0 );
			axis = Math::Vector3::Normalize( axis );

			float amin = 9999;
			float amax = -9999;

			for ( int index = 0; index < 4; ++index )
			{
				float project = Math::Vector3::Dot( axis, oobb1[index] );
				if ( project < amin )
				{
					amin = project;
				}

				if ( project > amax )
				{
					amax = project;
				}		
			}

			float bmin = 9999;
			float bmax = -9999;

			for ( int index = 0; index < 4; ++index )
			{
				float project = Math::Vector3::Dot( axis, oobb2[index] );
				if ( project < bmin )
				{
					bmin = project;
				}

				if ( project > bmax )
				{
					bmax = project;
				}		
			}

			if ( amin > bmax || bmin > amax )
			{
				return false;
			}
		}

		return true;
	}

    //===========================================================================

    bool IntersectDynamic( const Math::Sphere& sphere1, const Math::Sphere& sphere2, float& time )
    {
        Math::Vector3 v1 = sphere1.GetPosition() - sphere1.GetOldPosition();
        Math::Vector3 v2 = sphere2.GetPosition() - sphere2.GetOldPosition();
        Math::Vector3 velocity21 = v2 - v1;

        Math::Vector3 length21 = sphere2.GetOldPosition() - sphere1.GetOldPosition();
        float radiustotal = sphere1.GetRadius() + sphere2.GetRadius();

        float a = Math::Vector3::Dot( velocity21, velocity21 );
        float b = Math::Vector3::Dot( velocity21, length21 ) * 2.0f;
        float c = Math::Vector3::Dot( length21, length21 ) - ( radiustotal * radiustotal );

        //length21.Math::Vector3::Dot( length21 ) > radiustotal * radiustotal )
        if ( IntersectStatic( sphere1, sphere2 ) == true )
        {
            time = 0;
            return true;
        }

        float q = ( b * b ) - ( 4.0f * a * c );
        if ( q >= 0 )
        {
            float sq = sqrtf( q );
            float d = 1.0f / ( 2.0f * a );
            float r1 = ( -b + sq ) * d;
            float r2 = ( -b - sq ) * d;

            if ( r1 > r2 )
            {
                time = r2;
            }
            else
            {
                time = r1;
            }

            if ( time <= 1 && time >= 0 )
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    //===========================================================================

    bool IntersectDynamic( const Math::Sphere& sphere, const Math::Plane& plane, float& time )
    {
        float startDistance = plane.DistanceToPlane( sphere.GetOldPosition() );
        float endDistance = plane.DistanceToPlane( sphere.GetPosition() );

        if ( fabs( startDistance ) < sphere.GetRadius() )
        { 
            time = 0;
            return true;
        }

        if ( startDistance > sphere.GetRadius() && endDistance < sphere.GetRadius() )
        {
            time = ( ( startDistance - sphere.GetRadius() ) / ( startDistance - endDistance ) ); 
            return true;
        }

        return false;
    }

    //===========================================================================

    bool IntersectDynamic( const Math::AABB& aabb1, const Math::AABB& aabb2, float& time )
    {
        Math::AABB currentAABB1 = aabb1;
        currentAABB1.SetPosition( currentAABB1.GetOldPosition() );
        Math::AABB currentAABB2 = aabb2;
        currentAABB2.SetPosition( currentAABB2.GetOldPosition() );

        Math::Vector3 aabb1min = currentAABB1.GetMin();
        Math::Vector3 aabb1max = currentAABB1.GetMax();

        Math::Vector3 aabb2min = currentAABB2.GetMin();
        Math::Vector3 aabb2max = currentAABB2.GetMax();

        Math::Vector3 aabb1velocity = aabb1.GetPosition() - aabb1.GetOldPosition();
        Math::Vector3 aabb2velocity = aabb2.GetPosition() - aabb2.GetOldPosition();

        Math::Vector3 relativeVelocity = aabb2velocity - aabb1velocity;

		if ( Math::Vector3::Magnitude( relativeVelocity ) == 0 )
        {
            time = 0;
            return false;
        }

        Math::Vector3 u_0;
        Math::Vector3 u_1( 1, 1, 1 );

        for ( int i = 0; i < 3; i++ )
        {
            if ( aabb1max[i] < aabb2min[i] && relativeVelocity[i] < 0 )
            {
                u_0[i] = ( aabb1max[i] - aabb2min[i] ) / relativeVelocity[i];
            }
            else if ( aabb2max[i] < aabb1min[i] && relativeVelocity[i] > 0 )
            {
                u_0[i] = ( aabb1min[i] - aabb2max[i] ) / relativeVelocity[i];
            }

            if ( aabb2max[i] > aabb1min[i] && relativeVelocity[i] < 0 )
            { 
                u_1[i] = ( aabb1min[i] - aabb2max[i] ) / relativeVelocity[i]; 
            }
            else if ( aabb1max[i] > aabb2min[i] && relativeVelocity[i] > 0 )
            {
                u_1[i] = ( aabb1max[i] - aabb2min[i] ) / relativeVelocity[i]; 
            }
        }

        float u0 = Math::Max( u_0[0], Math::Max( u_0[1], u_0[2] ) );    
        float u1 = Math::Min( u_1[0], Math::Min( u_1[1], u_1[2] ) );

        if ( u0 <= u1 )
        {
            time = u0;
            return true;
        }
        else
        {
            return false;
        }
    }

    //===========================================================================

    bool IntersectDynamic( const Math::Sphere& sphere, const Math::Triangle& triangle, float& time )
    {
        Math::Vector3 normal = triangle.GetNormal();
        Math::Plane plane( triangle[0], normal );


        if ( IntersectDynamic( sphere, plane, time ) )
        {
            float epsilon = 0.1f;
            Math::Vector3 v = sphere.GetPosition() - sphere.GetOldPosition();
            Math::Vector3 c = sphere.GetOldPosition() + v * time;

            float angle;
			Math::Vector3 vec0 = Math::Vector3::Normalize( ( c - triangle[0] ) );
			Math::Vector3 vec1 = Math::Vector3::Normalize( ( c - triangle[1] ) );
			Math::Vector3 vec2 = Math::Vector3::Normalize( ( c - triangle[2] ) );

            angle =
                static_cast<float>( 
				acos( Math::Vector3::Dot( vec0, vec1 ) ) + 
                acos( Math::Vector3::Dot( vec1, vec2 ) ) + 
                acos( Math::Vector3::Dot( vec2, vec0 ) ) 
				) ;

            float diff = angle - Math::PIMUL2;
            if ( fabs( diff ) < epsilon )
            {
                return true;
            }

            Math::Vector3 p[3];
            p[0]= triangle[0];
            p[1] = triangle[1];
            p[2] = triangle[2];
            bool check[3];
            check[0] = false;
            check[1] = false;
            check[2] = false;
            float currentTime = Math::INFINITY_NUM;

            for ( int i = 0; i < 3; i++ )
            {
                Math::Vector3 ke = p[(i+1)%3] - p[i];
                Math::Vector3 kg = p[i] - sphere.GetOldPosition();
                Math::Vector3 ks = v;
                float kee = Math::Vector3::Dot( ke, ke );
                float kes = Math::Vector3::Dot( ke, ks );
                float kgs = Math::Vector3::Dot( kg, ks );
                float keg = Math::Vector3::Dot( ke, kg );
                float kgg = Math::Vector3::Dot( kg, kg );
                float kss = Math::Vector3::Dot( ks, ks );
                float a = kee * kss - kes * kes;
                float b = 2.0f * ( keg * kes - kee * kgs );
                float cc = kee * ( kgg - sphere.GetRadius() * sphere.GetRadius() ) - keg * keg;
                float x = ( b * b ) - ( 4.0f * a * cc );
                if ( x >= 0 )
                {
                    float sign = -1;
                    if ( b >= 0 )
                    {
                        sign = 1;
                    }

                    float q = -0.5f * ( b + sign * sqrtf( x ) );          
                    float t0 = q / a;
                    float t1 = cc / q;

                    float temp = 0;
                    if ( t0 > t1 )
                    {
                        temp = t0;
                        t0 = t1;
                        t1 = temp;
                    }

                    if ( t0 <= 0 && t1 >= 0 && t1 <= 1 )
                    {  
                        if ( t1 < currentTime )
                        {
                            currentTime = t1;
                            Math::Vector3 ct = sphere.GetOldPosition() + v * currentTime;
                            float d = Math::Vector3::Dot( ( ct - p[i] ), ke ) / kee;
                            if ( d >= 0 && d <= 1.1 )
                            {
                                check[i] = true;
                            }
                        }
                    }
                    else if ( t0 <= 0 && t1 >= 1 )
                    {            
                        currentTime = 0;
                        Math::Vector3 ct = sphere.GetOldPosition() + v * currentTime;
                        float d = Math::Vector3::Dot( ( ct - p[i] ), ke ) / kee;
                        if ( d >= 0 && d <= 1.1 )
                        {
                            check[i] = true;
                        }
                    }
                    else if ( t0 >= 0 && t0 <= 1 && t1 >= 0 && t1 <= 1 )
                    {
                        if ( t0 < currentTime )
                        {
                            currentTime = t0;
                            Math::Vector3 ct = sphere.GetOldPosition() + v * currentTime;
                            float d = Math::Vector3::Dot( ( ct - p[i] ), ke ) / kee;
                            if ( d >= 0 && d <= 1.1 )
                            { 
                                check[i] = true;
                            }
                        }
                    }
                    else if ( t0 >= 0 && t0 <= 1 && t1 >= 1 )
                    {
                        if ( t0 < currentTime )
                        {
                            currentTime = t0;
                            Math::Vector3 ct = sphere.GetOldPosition() + v * currentTime;
                            float d = Math::Vector3::Dot( ( ct - p[i] ), ke ) / kee;
                            if ( d >= 0 && d <= 1.1 )
                            {
                                check[i] = true;
                            }
                        }
                    }
                }
            }

            if ( check[0] == true || check[1] == true || check[2] == true )
            {
                time = currentTime;
                return true;
            }
        }

        return false;
    }

}
