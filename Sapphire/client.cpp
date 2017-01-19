#include <iostream>

#include <Str.h>
#include <HashMap.h>
#include <Vector2.h>

struct t1
{
	int a = 0;
	float b = 0;
};

int main()
{
	Sapphire::String str1 = "asdkasd";
	std::cout << str1.CString() << std::endl;
	Sapphire::Vector<int> v;
	v.Push(5);
	v.Push(12);
	v.Push(24);
	for (size_t i = 0; i < v.Size(); i++)
	{
		std::cout << v.At(i) << std::endl;
	}
	Sapphire::HashMap<Sapphire::String, t1> map;
	t1 t;
	t.a = 5;
	t.b = 27.5;
	Sapphire::Pair<Sapphire::String, t1> p("test", t);
	map.Insert(p);
	Sapphire::HashMap<Sapphire::String, t1>::ConstIterator cit =  map.Find("test");
	std::cout << cit->second_.a << "  " << cit->second_.b << std::endl;

	Sapphire::Vector2 v2(100, 200);
	std::cout << v2.x_ << "  " << v2.y_ << std::endl;
	getchar();
	return 0;
}