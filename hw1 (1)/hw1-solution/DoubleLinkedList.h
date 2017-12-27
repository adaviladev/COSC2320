#ifndef __DOUBLE_LINKED_LIST_H__
#define __DOUBLE_LINKED_LIST_H__
#include <string>
#include "FileReader.h"

struct DoubleLinkedNode 
{
	DoubleLinkedNode(const std::string& strInfo)
		: strInfo(strInfo), pPrevNode(NULL), pNextNode(NULL)
	{

	}
	~DoubleLinkedNode()
	{
		if (pPrevNode != NULL)
		{
			pPrevNode = NULL;
		}
		if (pNextNode != NULL)
		{
			pNextNode = NULL;
		}
	}
	std::string strInfo;
	DoubleLinkedNode* pPrevNode;
	DoubleLinkedNode* pNextNode;
};

class DoubleLinkedList
{
public:
	DoubleLinkedList(const std::string& str, const int digitsPerNode = 1);
	DoubleLinkedList(const DoubleLinkedList& list);
	DoubleLinkedList(void);
	~DoubleLinkedList(void);

public:
	void InsertNode(DoubleLinkedNode& insertNode, const DoubleLinkedNode& node); // insert insertNode after node;
	void InsertNode(DoubleLinkedNode& node);
	void DeleteNode(const DoubleLinkedNode& node);
	void Print(const std::string& strDirection);
	void Union(const DoubleLinkedList& inList1, const DoubleLinkedList& inList2, DoubleLinkedList& outList);
	void Intersection(const DoubleLinkedList& inList1, const DoubleLinkedList& inList2, DoubleLinkedList& outList);
	const DoubleLinkedNode* GetHeadNode() const;
	const DoubleLinkedNode* GetTailNode() const;

	DoubleLinkedList& operator = (const DoubleLinkedList& list);
// 	DoubleLinkedList& operator + (const DoubleLinkedList& list);
// 	DoubleLinkedList& operator * (const DoubleLinkedList& list);

	DoubleLinkedList operator + (const DoubleLinkedList& list);
	DoubleLinkedList operator * (const DoubleLinkedList& list);

	const std::string ToString() const;
private:
	DoubleLinkedNode* m_pHead;
	DoubleLinkedNode* m_pTail;
	int m_digitsPerNode;
};

#endif //__DOUBLE_LINKED_LIST_H__

