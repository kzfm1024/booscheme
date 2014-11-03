//
// boodebug.h
//

#pragma once
#ifndef _BOODEBUG_H
#define _BOODEBUG_H

#if defined(_ENABLE_BOODEBUG_)
#define BOODEBUG(x) x
#else
#define BOODEBUG(x)
#endif

#endif // _BOODEBUG_H
