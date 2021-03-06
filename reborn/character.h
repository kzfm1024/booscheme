//
// chracter.h
//

#pragma once
#ifndef _CHARACTER_H
#define _CHARACTER_H

#include "object.h"

namespace boo
{
	class character : public object
	{
	public:
		character(int ch) : m_ch(ch) {}
		virtual ~character() {} 

		virtual std::string to_s() { return std::string(1, m_ch); }
                
	private:
		int m_ch;
	};
}

#endif // _CHARACTER_H
