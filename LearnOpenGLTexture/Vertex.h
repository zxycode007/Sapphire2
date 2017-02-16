#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Color.h"


enum EVextexType
{
	EVT_STARNDRD,
	EVT_TCOORD,
	EVT_2TCOORD,
	EVT_TANGENTS

};

namespace Sapphire
{

	class Vertex
	{
	public:



		Vertex()
		{
			mPos = Vector3::ZERO;
			mColor = Color::WHITE;
			mNormal = Vector3::ZERO;
			mTCoord = Vector2::ZERO;

		};
		Vertex(float x, float y, float z, float nx, float ny, float nz, float r, float g, float b, float a, float u, float v)
		{
			mPos = Sapphire::Vector3(x, y, z);
			mColor = Sapphire::Color(r, g, b, a);
			mTCoord = Sapphire::Vector2(u, v);

		};
		Vertex(Sapphire::Vector3 pos, Color color, Vector3 normal, Vector2 tcoord)
		{
			mPos = pos;
			mColor = color;
			mNormal = normal;
			mTCoord = tcoord;
		}
		~Vertex() {};

		bool operator==(const Vertex& other)
		{
			return (mPos == other.getCPos() && mColor == other.getCColor() && mNormal == other.getCNormal() && mTCoord == other.getCTCoord());
		};

		bool operator!=(const Vertex& other)
		{
			return (mPos != other.getCPos() && mColor != other.getCColor() && mNormal != other.getCNormal() && mTCoord != other.getCTCoord());
		}

		EVextexType getType()
		{
			return EVT_STARNDRD;
		}

		Vector3& getPos() { return mPos; };
		Color& getColor() { return mColor; };
		Vector3& getNormal() { return mNormal; };
		Vector2& getTCoord() { return mTCoord; };
		const Vector3& getCPos() const { return mPos; };
		const Color& getCColor() const { return mColor; };
		const Vector3& getCNormal() const { return mNormal; };
		const Vector2& getCTCoord() const { return mTCoord; };

		Vertex Lerp(const Vertex& other, float d)
		{
			d = Clamp(d, 0.0f, 1.0f);
			return Vertex(mPos.Lerp(other.getCPos(), d), mColor.Lerp(other.getCColor(), d), mNormal.Lerp(other.getCNormal(), d), mTCoord.Lerp(other.getCTCoord(),d));
		}

	private:

		Vector3  mPos;
		Color  mColor;
		Vector3  mNormal;
		Vector2  mTCoord;

	};


}