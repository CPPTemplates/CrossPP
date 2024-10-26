#pragma once
#include <assert.h>
#include <stdexcept>
#include <string>
#include "globalFunctions.h"
#include "array/wstringFunctions.h"
#include "macrotricks.h"

#ifdef _DEBUG
//#define EXTRADEBUGCHECKS
#ifdef EXTRADEBUGCHECKS
//#if defined(_DEBUG) && defined(EXTRADEBUGCHECKS)
# define assumeInRelease1(shouldBeTrue)   throwIfFalse(shouldBeTrue)
# define assumeInRelease2(shouldBeTrue, errorMessageIfFalse)   throwIfFalse((shouldBeTrue), (errorMessageIfFalse))
#else//no checks, no nothing
# define assumeInRelease1(shouldBeTrue)
# define assumeInRelease2(shouldBeTrue, errorMessageIfFalse)
#endif
#else
#if _MSC_VER
# define assumeInRelease1(shouldBeTrue)   __assume(shouldBeTrue)
# define assumeInRelease2(shouldBeTrue, errorMessageIfFalse)   __assume(shouldBeTrue)
#elif __clang__
//when an expression can't be evaluated at compile time, we just replace it with 'true'
# define assumeInRelease1(shouldBeTrue)   __builtin_assume(__builtin_constant_p(shouldBeTrue) ? shouldBeTrue : true)
# define assumeInRelease2(shouldBeTrue, errorMessageIfFalse)   __builtin_assume(__builtin_constant_p(shouldBeTrue) ? shouldBeTrue : true)
#else
# define assumeInRelease1(shouldBeTrue)   [[assume(shouldBeTrue)]]
# define assumeInRelease2(shouldBeTrue, errorMessageIfFalse)   [[assume(shouldBeTrue)]]
#endif
#endif

//doesn't modify outside world, but can still access it
//always returns the same value for the same input
#define pure __attribute__((pure))

//this function can only use the variables passed in. you can't even access the values of pointers passed in!
#define pureInputFunc __attribute__((const))

#define assumeInRelease(...) GetMacroOverLoad(__VA_ARGS__, noFunctionFound, assumeInRelease2, assumeInRelease1)(__VA_ARGS__)

//https://docs.microsoft.com/en-us/cpp/intrinsics/assume?view=msvc-170

//#define assumeInRelease(e) (((e) || assert(__FILE__, __LINE__)), [[assume(e)]] )

inline void throwIfFalse(cbool& shouldBeTrue, std::wstring errorMessageIfFalse = std::wstring())
{
	if (!shouldBeTrue)
	{
		throw std::runtime_error(WStringToString(errorMessageIfFalse));
	}
}


//for if the compiler gets smarter.

/*constexpr __forceinline void assumeInRelease(cbool& shouldBeTrue, std::wstring errorMessageIfFalse = std::wstring())
{
#ifdef _DEBUG
	if (!shouldBeTrue)
	{
		throw "should not be able to come here";
	}
#else
	[[assume(shouldBeTrue)]];
#endif // _DEBUG
}*/