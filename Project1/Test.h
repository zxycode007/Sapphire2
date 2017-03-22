#include <iostream>
#include <stdio.h>
#include <string>
#include "Sapphire.h"
#include "Variant.h"
#include "Context.h"
#include "Object.h"

namespace Sapphire
{
	using namespace std;

	class CustomEvenTObj : public Object
	{
		SAPPHIRE_OBJECT(CustomEvenTObj, Object);
	public:

		CustomEvenTObj(Context* context) :Object(context)
		{
		 
		}

		void invoke(StringHash eventName, VariantMap& map)
		{
			VariantMap::Iterator it = map.Find("what");
			if(it != map.End())
			{ 
				const String& str = it->second_.GetString();
				cout << " CustomEventObject执行对象 What is " << str.CString() << endl;
			}
			cout << "CustomEventObject执行对象" << endl;
		}
	
	private:


	};

	class CustomObj : public Object
	{
	
		SAPPHIRE_OBJECT(CustomObj, Object);

	public:
	 
		CustomObj(Context* context):Object(context)
		{
			c = context;
		}

		void setA(int a)
		{
			this->a = a;
		}
		void setB(int b)
		{
			this->b = b;
		}
		void print()
		{
			cout << a << b << endl;
		}
	private:
		int a;
		int b;
		Context* c;
	};

}
