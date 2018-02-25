#pragma once

#include "stdafx.h"

typedef std::vector<char> CharacterBuffer;

class File
{
public:
	static bool read(char const * path, CharacterBuffer * output);
};