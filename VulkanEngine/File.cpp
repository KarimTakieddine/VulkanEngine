#include "stdafx.h"
#include "File.h"

bool File::read(char const * path, CharacterBuffer * output)
{
	std::ifstream inputFileStream(path, std::ios::in | std::ios::binary);

	if (!inputFileStream.is_open())
	{
		return false;
	}

	inputFileStream.seekg(0, inputFileStream.end);
	size_t fileSize = inputFileStream.tellg();
	inputFileStream.seekg(0, inputFileStream.beg);

	output->resize(fileSize);

	inputFileStream.read(output->data(), fileSize);

	inputFileStream.close();

	return true;
}