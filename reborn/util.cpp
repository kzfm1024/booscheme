//
// util.cpp
//

#include "util.h"

namespace booscheme
{
	boolean* TRUE()
	{
		static boolean* t;
		if (!t) t = new boolean(true);
		return t;
	}

	boolean* FALSE()
	{
		static boolean* f;
		if (!f) f = new boolean(false);
		return f;
	}

	boolean* BOOLEAN(bool x)
	{
		return x ? TRUE() : FALSE();
	}
}
