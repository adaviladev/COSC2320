#ifndef __FILE_READER_H__
#define __FILE_READER_H__
#include <vector>
#include "DoubleLinkedList.h"

enum Operator
{
	PLUS,
	MULTIPLY,
	UNKNOWN
};
//class DoubleLinkedList;

class Operation
{
public:
    Operation()
	{

	}
	Operation(const std::string& str1, const std::string& str2, const Operator& op)
		: m_str1(str1)
		, m_str2(str2)
		, m_operator(op)
	{

	}
	~Operation()
	{

	}

	std::string m_str1;
	std::string m_str2;
	Operator m_operator;
};

class FileReader
{
public:
	FileReader(const char* pFilename);
//	FileReader();
	~FileReader();

public:
	void GetOperation(std::vector<Operation>& operation);

private:
	char* m_pFilename;
};

#endif // __FILE_READER_H__
