#pragma once
#include "stdafx.h"
#include "mem.h"

namespace skeet2
{
	class CheatMeta // base type to store info about cheats
	{
	public:
		std::string m_Name;
		std::string m_DisplayName;
		std::map<std::string, long double> m_options;
		float m_Version = 0;

		virtual bool Enable();
	};
}
