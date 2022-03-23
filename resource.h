#pragma once
#include <list>
#include "main.h"

class CResource
{
protected:
	char							m_FileName[ MAX_PATH ];

	static std::list<CResource*>	m_Pool;

public:
	CResource(){}
	virtual ~CResource(){}

	virtual void Load(const char* FileName) = 0;
	virtual void Unload() = 0;
};