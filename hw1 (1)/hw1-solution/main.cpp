// DoubleLinkInt.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <sstream>
#include <iostream>
#include "FileReader.h"
//input=xyz.txt;digitsPerNode=2
void ParseCommand(const char* command, std::string& filename, int& digitsPerNode);
int main(int argc, char* argv[])
{
	std::string filename;
	int digitsPerNode;
	ParseCommand(argv[1], filename, digitsPerNode);

	// Read Operations
	FileReader fileReader(filename.c_str());
	std::vector<Operation> operation;
	fileReader.GetOperation(operation);

	for (std::vector<Operation>::const_iterator iter = operation.begin(); iter != operation.end(); ++iter)
	{
		// Build double linked list from number 
		DoubleLinkedList list1(iter->m_str1, digitsPerNode);
		DoubleLinkedList list2(iter->m_str2, digitsPerNode);
		if (iter->m_operator == PLUS)
		{
			DoubleLinkedList list = list1 + list2;
			std::cout << iter->m_str1 << "+" << iter->m_str2 << "=" << list.ToString() << std::endl;
		} 
		else if (iter->m_operator == MULTIPLY)
		{
			DoubleLinkedList list = list1 * list2;
			std::cout << iter->m_str1 << "*" << iter->m_str2 << "=" << list.ToString() << std::endl;
		}
	}
	return 0;
}
// input=xyz.txt;digitsPerNode=2
void ParseCommand(const char* command, std::string& filename, int& digitsPerNode)
{
	std::string strCommand(command);
	std::size_t posEqual1 = strCommand.find_first_of("=");
	std::size_t posEqual2 = strCommand.find_last_of("=");
	std::size_t posCommas = strCommand.find_first_of(";");

	filename = strCommand.substr(posEqual1 + 1, posCommas - posEqual1 - 1);
	std::string strDigitsPerNode = strCommand.substr(posEqual2 + 1);

	std::stringstream ss(strDigitsPerNode.c_str());

	ss >> digitsPerNode;
}
