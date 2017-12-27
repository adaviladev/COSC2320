#include "FileReader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
FileReader::FileReader(const char* pFilename)
	: m_pFilename(NULL)
{
	unsigned int fileNameLength = strlen(pFilename) + 1;
	m_pFilename = new char[fileNameLength];
	memcpy(m_pFilename, pFilename, fileNameLength);
	m_pFilename[fileNameLength - 1] = '\0';
}

// FileReader::FileReader()
// 	: m_pFilename(NULL)
// {
// 
// }

FileReader::~FileReader()
{
	if (m_pFilename != NULL)
	{
		delete [] m_pFilename;
		m_pFilename = NULL;
	}
}

void FileReader::GetOperation(std::vector<Operation>& operation)
{
	std::ifstream fileStream(m_pFilename);
	std::string strLine(4096, 0);
	while (fileStream.getline((char*)strLine.data(), 4096))
	{
		// Read operator
		strLine.resize(strLine.size());
		std::string::iterator iterOp =  std::find(strLine.begin(), strLine.end(), '+');
		Operator op = UNKNOWN;
		if (iterOp != strLine.end())
		{
			op = PLUS;
		} 
		else
		{
			iterOp =  std::find(strLine.begin(), strLine.end(), '*');
			if (iterOp != strLine.end())
			{
				op = MULTIPLY;
			} 
		}
		if (op == UNKNOWN)
		{
			std::cout << "No operator" << std::endl;
		}

		// read numbers
		*iterOp = ' ';
		std::stringstream ss(strLine.c_str());
		std::string str1;
		std::string str2;
		ss >> str1 >> str2;
		const Operation operation_t(str1, str2, op);
		operation.push_back(operation_t);

		memset((char*)strLine.data(), 0, 4096);
	}
}
