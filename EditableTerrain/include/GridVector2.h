#pragma once

namespace Ogre
{

	// 封装了一个由两个ushort组成的向量
	class ET_EXPORT GridVector2
	{
	public:
		GridVector2(ushort x , ushort y)
			: x(x) , y(y)
		{
		}
		GridVector2(const GridVector2& rhs)
			: x(rhs.x) , y(rhs.y)
		{
		}
		GridVector2(const Vector2& rhs)
			: x((ushort)rhs.x) , y((ushort)rhs.y)
		{
		}
		inline bool operator < ( const GridVector2& rhs ) const
		{
			if( getXY() < rhs.getXY())
				return true;
			return false;
		}
		inline bool operator <= ( const GridVector2& rhs ) const
		{
			if( getXY() <= rhs.getXY())
				return true;
			return false;
		}
		inline bool operator > ( const GridVector2& rhs ) const
		{
			if( getXY() > rhs.getXY())
				return true;
			return false;
		}
		inline bool operator >= ( const GridVector2& rhs ) const
		{
			if( getXY() >= rhs.getXY())
				return true;
			return false;
		}
		ulong getXY()const{return ((ulong)x << 16) | (ulong)y;}
		ushort getX()const{return x;}
		ushort getY()const{return y;}
		void setX(ushort sX)
		{
			x = sX;
		}
		void setY(ushort sY)
		{
			y = sY;
		}
		GridVector2 operator+(const GridVector2& adder)const
		{
			return GridVector2(getX() + adder.getX() , getY() + adder.getY());
		}
		GridVector2 operator-(const GridVector2& adder)const
		{
			return GridVector2(getX() - adder.getX() , getY() - adder.getY());
		}

		GridVector2 &operator+=(const GridVector2& adder)
		{
			setX(getX() + adder.getX());
			setY(getY() + adder.getY());
			return *this;
		}
		GridVector2 &operator-=(const GridVector2& adder)
		{
			setX(getX() - adder.getX());
			setY(getY() - adder.getY());
			return *this;
		}

		GridVector2 operator*(const GridVector2& adder)const
		{
			return GridVector2(getX() * adder.getX() , getY() * adder.getY());
		}
		GridVector2 operator/(const GridVector2& adder)const
		{
			return GridVector2(getX() / adder.getX() , getY() / adder.getY());
		}

		GridVector2 &operator*=(const GridVector2& adder)
		{
			setX(getX() * adder.getX());
			setY(getY() * adder.getY());
			return *this;
		}
		GridVector2 &operator/=(const GridVector2& adder)
		{
			setX(getX() / adder.getX());
			setY(getY() / adder.getY());
			return *this;
		}
		bool operator==(const GridVector2& other)
		{
			return x == other.x && y == other.y;
		}
		bool operator!=(const GridVector2& other)
		{
			return x != other.x && y != other.y;
		}

		/** Function for writing to a stream.
		*/
		inline friend std::ostream& operator <<
			( std::ostream& o, const GridVector2& v )
		{
			o << "GridVector2(" << v.x << ", " << v.y <<  ")";
			return o;
		}


		ushort x , y;
	};
};