#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace Math
{ 
	template<class Type> class Rectangle
	{
	public:

		Rectangle()
		{
		}

		~Rectangle()
		{

		}

		void SetPosition( Type x, Type y )
		{
			mX = x;
			mY = y;
		}

		void SetWidth( Type width )
		{
			mWidth = width;
		}

		void SetHeight( Type height )
		{
			mHeight = height;
		}

		Type GetX() const
		{
			return mX;
		}

		Type GetY() const
		{
			return mY;
		}

		Type GetWidth() const
		{
			return mWidth;
		}

		Type GetHeight() const
		{
			return mHeight;
		}
		
	private:

		Type mX;
		Type mY;
		Type mWidth;
		Type mHeight;
	};
}

#endif
