#pragma once
#include "stdafx.h"
#include "Vector2.h"
#include "Vector3.h"

namespace Sapphire
{
	class Line3d
	{
	public:

		Line3d(Vector3 p1, Vector3 p2, Color c=Color()) :start(p1), end(p2), color(c) {};
		~Line3d() {};

		bool operator==(const Line3d& rhs)
		{
			return (start == rhs.start);
		}

		void setStart(const Vector3& rhs)
		{
			start = rhs;
		}

		void setEnd(const Vector3& rhs)
		{
			end = rhs;
		}

		Vector3& getStart()
		{
			return start;
		}

		Vector3& getEnd()
		{
			return end;
		}

		Color& getColor()
		{
			return color;
		}
		void  setColor(const Color& c)
		{
			color = c;
		}

		ulong getSize()
		{
			return sizeof(Vector3) * 2 + sizeof(Color);
		}

	private:

		Vector3 start;
		Vector3 end;
		Color   color;


	};
}
