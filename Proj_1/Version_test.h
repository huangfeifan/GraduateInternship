#pragma once
#ifndef VERSION_TEST_H
#define VERSION_TEST_H
#if _MSC_FULL_VER == 170050522 || _MSC_FULL_VER == 170050727 


#endif  // ends compiler version check

#ifdef LROUND
inline long lround(double d)
{
	return (d >= 0) ? long(d + 0.5) : long(d - 0.5);
}
#endif

#endif  // ends header guard

