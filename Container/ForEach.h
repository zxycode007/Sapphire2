#pragma once

#include "Vector.h"
#include "List.h"
#include "HashSet.h"
#include "HashMap.h"

#include <algorithm>


// VS2010+ and other compilers: use std::begin(), std::end() & range based for
// C++11 features need to be enabled
#if !defined(_MSC_VER) || _MSC_VER > 1600
#define foreach(VAL, VALS) for (VAL : VALS)
// Fallback solution for VS2010. Will have problem with break statement.
// See https://github.com/Sapphire/Sapphire/issues/561
#else

namespace Sapphire
{

template<typename T>
struct false_wrapper {
	false_wrapper(const T& value) : value(value) { }
	operator bool() const { return false; }
	T value;
};

template<typename T>
false_wrapper<T> make_false_wrapper(const T& value) {
	return false_wrapper<T>(value);
}

// vector support functions

template <class T>
Sapphire::RandomAccessIterator<T> Begin(Sapphire::Vector<T> &v) {
	return v.Begin();
}
template <class T>
Sapphire::RandomAccessIterator<T> Begin(Sapphire::Vector<T> *v) {
	return v->Begin();
}

template <class T>
Sapphire::RandomAccessConstIterator<T> Begin(const Sapphire::Vector<T> &v) {
	return v.Begin();
}
template <class T>
Sapphire::RandomAccessConstIterator<T> Begin(const Sapphire::Vector<T> *v) {
	return v->Begin();
}

template <class T>
Sapphire::RandomAccessIterator<T> End(Sapphire::Vector<T> &v) {
	return v.End();
}
template <class T>
Sapphire::RandomAccessIterator<T> End(Sapphire::Vector<T> *v) {
	return v->End();
}

template <class T>
Sapphire::RandomAccessConstIterator<T> End(const Sapphire::Vector<T> &v) {
	return v.End();
}
template <class T>
Sapphire::RandomAccessConstIterator<T> End(const Sapphire::Vector<T> *v) {
	return v->End();
}

// podvector support functions

template <class T>
Sapphire::RandomAccessIterator<T> Begin(Sapphire::PODVector<T> &v) {
	return v.Begin();
}
template <class T>
Sapphire::RandomAccessIterator<T> Begin(Sapphire::PODVector<T> *v) {
	return v->Begin();
}

template <class T>
Sapphire::RandomAccessConstIterator<T> Begin(const Sapphire::PODVector<T> &v) {
	return v.Begin();
}
template <class T>
Sapphire::RandomAccessConstIterator<T> Begin(const Sapphire::PODVector<T> *v) {
	return v->Begin();
}

template <class T>
Sapphire::RandomAccessIterator<T> End(Sapphire::PODVector<T> &v) {
	return v.End();
}
template <class T>
Sapphire::RandomAccessIterator<T> End(Sapphire::PODVector<T> *v) {
	return v->End();
}

template <class T>
Sapphire::RandomAccessConstIterator<T> End(const Sapphire::PODVector<T> &v) {
	return v.End();
}
template <class T>
Sapphire::RandomAccessConstIterator<T> End(const Sapphire::PODVector<T> *v) {
	return v->End();
}

}

#define foreach(VAL, VALS) \
    if (const auto& _foreach_begin = Sapphire::make_false_wrapper(Sapphire::Begin(VALS))) { } else \
    if (const auto& _foreach_end = Sapphire::make_false_wrapper(Sapphire::End(VALS))) { } else \
    for (auto it = _foreach_begin.value; it != _foreach_end.value; ++it) \
    if (bool _foreach_flag = false) { } else \
    for (VAL = *it; !_foreach_flag; _foreach_flag = true)

#endif