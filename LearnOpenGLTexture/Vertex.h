#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Color.h"




namespace Sapphire
{
	enum EVertexType
	{
		EVT_STARNDRD,
		EVT_COLOR,
		EVT_NORMAL,
		EVT_TCOORD,
		EVT_2TCOORD,
		EVT_TANGENTS,
		MAX_EVT
	};

	enum EIndexType
	{
		EIT_16BIT,
		EIT_32BIT
	};

	

	class Vertex
	{
	public:

		Vertex()
		{
			mPos = Vector3::ZERO;
			mType = EVT_STARNDRD;

		};
		Vertex(float x, float y, float z)
		{
			mPos = Sapphire::Vector3(x, y, z);
			mType = EVT_STARNDRD;

		};
		Vertex(Sapphire::Vector3 pos)
		{
			mPos = pos;
			mType = EVT_STARNDRD;
		}
		virtual ~Vertex() {};

		bool operator==(const Vertex& other)
		{
			return (mPos == other.getCPos() && mType == other.getCType());
		};

		bool operator!=(const Vertex& other)
		{
			return (mPos != other.getCPos() || mType != other.getCType());
		}

		EVertexType& getType()
		{
			return mType;
		}
		 const EVertexType& getCType() const
		{
			return mType;
		}
		Vector3& getPos() { return mPos; };
		const Vector3& getCPos() const { return mPos; };

		Vertex Lerp(const Vertex& other, float d)
		{
			d = Clamp(d, 0.0f, 1.0f);
			return Vertex(mPos.Lerp(other.getCPos(), d));
		}

	protected:

		Vector3  mPos;
		EVertexType mType;

	};

	class VertexColor : public Vertex
	{
	public:



		VertexColor()
		{
			mPos = Vector3::ZERO;
			mColor = Color::WHITE;
			mType = EVT_COLOR;

		};
		VertexColor(float x, float y, float z,float r,float g,float b,float a)
		{
			Vertex(x, y, z);
			mColor = Color(r, g, b, a);
			mType = EVT_COLOR;

		};
		VertexColor(Sapphire::Vector3 pos, Sapphire::Color color)
		{
			mPos = pos;
			mColor = color;
			mType = EVT_COLOR;
		}

		VertexColor(Sapphire::Vertex& v): Vertex(v)
		{		
			mColor = Color::WHITE;
			mType = EVT_COLOR;
		}

		virtual ~VertexColor() {};

		bool operator==(const VertexColor& other)
		{
			return (static_cast<Vertex>(*this)==other && mColor == other.getCColor() );
		};

		bool operator!=(const VertexColor& other)
		{
			return (static_cast<Vertex>(*this) != other || mColor != other.getCColor());
		}

		Color& getColor() { return mColor; };
		const Color& getCColor() const { return mColor; };



		VertexColor Lerp(const VertexColor& other, float d)
		{
			d = Clamp(d, 0.0f, 1.0f);
			return VertexColor(mPos.Lerp(other.getCPos(), d), mColor.Lerp(other.getCColor(),d));
		}

	protected:

		Color    mColor;

	};

	class VertexNormal : public VertexColor
	{
	public:



		VertexNormal()
		{
			VertexColor();
			mNormal = Vector3::ZERO;
			mType = EVT_NORMAL;
		};
		VertexNormal(float x, float y, float z, float r, float g, float b, float a, float nx, float ny, float nz)
		{
			VertexColor(x, y, z, r, g, b, a);
			mNormal = Vector3(nx, ny, nz);
			mType = EVT_NORMAL;

		};
		VertexNormal(Sapphire::Vector3 pos, Sapphire::Color color, Sapphire::Vector3 normal)
		{
			VertexColor(pos, color);
			mNormal = normal;
			mType = EVT_NORMAL;
		}

		VertexNormal(Sapphire::VertexColor& v) : VertexColor(v)
		{
			mNormal = Vector3::ZERO;
			mType = EVT_NORMAL;
		}

		virtual ~VertexNormal() {};

		bool operator==(const VertexNormal& other)
		{
			return (static_cast<VertexColor>(*this) == other && mNormal == other.getCNormal());
		};

		bool operator!=(const VertexNormal& other)
		{
			return (static_cast<VertexColor>(*this) != other || mNormal != other.getCNormal());
		}

		Vector3& getNormal()
		{
			return mNormal;
		}
		const Vector3 getCNormal() const
		{
			return mNormal;
		}

		VertexNormal Lerp(const VertexNormal& other, float d)
		{
			d = Clamp(d, 0.0f, 1.0f);
			return VertexNormal(mPos.Lerp(other.getCPos(), d), mColor.Lerp(other.getCColor(), d), mNormal.Lerp(other.getCNormal(),d));
		}

	protected:
		
		Vector3 mNormal;

	};


	class VertexTcoord : public VertexNormal
	{
	public:



		VertexTcoord()
		{
			VertexNormal();
			mType = EVT_TCOORD;
		};
		VertexTcoord(float x, float y, float z, float r, float g, float b, float a, float nx, float ny, float nz, float tu, float tv)
		{
			VertexNormal(x, y, z, r, g, b, a, nx, ny, nz);
			mTcoord = Vector2(tu, tv);
			mType = EVT_TCOORD;

		};
		VertexTcoord(Sapphire::Vector3 pos, Sapphire::Color color, Sapphire::Vector3 normal, Sapphire::Vector2 tcoord)
		{
			VertexNormal(pos, color, normal);
			mTcoord = tcoord;
			mType = EVT_TCOORD;
		}

		VertexTcoord(Sapphire::VertexNormal& v) : VertexNormal(v)
		{
			mTcoord = Vector2::ZERO;
			mType = EVT_TCOORD;
		}

		virtual ~VertexTcoord() {};

		bool operator==(const VertexTcoord& other)
		{
			return (static_cast<VertexNormal>(*this) == other && mTcoord == other.getCTcoord());
		};

		bool operator!=(const VertexTcoord& other)
		{
			return (static_cast<VertexNormal>(*this) != other || mTcoord != other.getCTcoord());
		}

		Vector2& getTcoord()
		{
			return mTcoord;
		}
		const Vector2& getCTcoord() const
		{
			return mTcoord;
		}

		VertexTcoord Lerp(const VertexTcoord& other, float d)
		{
			d = Clamp(d, 0.0f, 1.0f);
			return VertexTcoord(mPos.Lerp(other.getCPos(), d), mColor.Lerp(other.getCColor(), d), mNormal.Lerp(other.getCNormal(), d), mTcoord.Lerp(other.getCTcoord(), d));
		}

	protected:

		Vector2 mTcoord;

	};


}