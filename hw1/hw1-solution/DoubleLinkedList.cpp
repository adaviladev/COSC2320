#include "DoubleLinkedList.h"
#include <string>
#include <iostream>
#include <sstream>
#include <math.h>
using namespace std;

static unsigned long long GetNumber(const std::string str)
{
	unsigned long long result = 0;
	for (int i = 0; i < str.size(); i++)
	{
		result *= 10;
		result += str.at(i) - '0';
	}

	return result;
}

static std::string GetString(const unsigned long long number, const int digitsPerNode)
{
	std::string str;
	std::stringstream ss;
	ss << number;

	str = ss.str();
	int remainingZeroNumber = digitsPerNode - str.size();
	if (remainingZeroNumber > 0) 
	{
		str.insert(str.begin(), remainingZeroNumber, '0');
	}

	return str;
}

DoubleLinkedList::DoubleLinkedList(const std::string& str, const int digitsPerNode)
	: m_pHead(NULL)
	, m_pTail(NULL)
	, m_digitsPerNode(digitsPerNode)
{
	for (int i = 0; i < str.size() / digitsPerNode; i++)
	{
		std::string strInfo(str.end() - (i + 1) * digitsPerNode, str.end() - i * digitsPerNode);
		DoubleLinkedNode* node = new DoubleLinkedNode(strInfo);
		InsertNode(*node);
	}
	if (str.size() % digitsPerNode != 0)
	{
		std::string strInfo(str.begin(), str.begin() + str.size() % digitsPerNode);
		DoubleLinkedNode* node = new DoubleLinkedNode(strInfo);
		InsertNode(*node);
	}
}
DoubleLinkedList::DoubleLinkedList(void)
	: m_pHead(NULL)
	, m_pTail(NULL)
	, m_digitsPerNode(1)
{
}


DoubleLinkedList::~DoubleLinkedList(void)
{
	DoubleLinkedNode* pNode = m_pHead;
	while (pNode != NULL)
	{
		DoubleLinkedNode* pTempNode = pNode->pNextNode;
		delete pNode;
		pNode = pTempNode;
	}
	m_pHead = NULL;
	m_pTail = NULL;
}

// void DoubleLinkedList::InsertNode(DoubleLinkedNode& insertNode, const DoubleLinkedNode& node) // insert insertNode after node;
// {
// 	DoubleLinkedNode* pNode = m_pHead;
// 	while (pNode != NULL)
// 	{
// 		if (std::strcmp(pNode->strInfo.c_str(), node.strInfo.c_str()) == 0)
// 		{
// 			if (pNode->pNextNode != NULL)
// 			{
// 				insertNode.pNextNode = pNode->pNextNode;
// 				insertNode.pNextNode->pPrevNode = &insertNode;
// 			}
// 			else
// 			{
// 				m_pTail = &insertNode;
// 			}
// 			insertNode.pPrevNode = pNode;
// 			pNode->pNextNode = &insertNode;
// 			break;
// 		}
// 		pNode = pNode->pNextNode;
// 	}
// }

void DoubleLinkedList::InsertNode(DoubleLinkedNode& node)
{
	if (m_pHead != NULL)
	{
		node.pPrevNode = m_pTail;
		m_pTail->pNextNode = &node;
		m_pTail = &node;
	}
	else
	{
		m_pHead = &node;
		m_pTail = &node;
	}
}

// void DoubleLinkedList::DeleteNode(const DoubleLinkedNode& node)
// {
// 	DoubleLinkedNode* pNode = m_pHead;
// 	while (pNode != NULL)
// 	{
// 		if (std::strcmp(pNode->strInfo.c_str(), node.strInfo.c_str()) == 0)
// 		{
// 			if (pNode == m_pHead)
// 			{
// 				m_pHead = pNode->pNextNode;
// 				pNode->pNextNode = NULL;
// 				delete pNode;
// 				pNode = NULL;
// 				break;
// 			}
// 			else if (pNode == m_pTail)
// 			{
// 				m_pTail = pNode->pPrevNode;
// 				m_pTail->pNextNode = NULL;
// 				delete pNode;
// 				pNode = NULL;
// 				break;
// 			}
// 			else
// 			{
// 				pNode->pPrevNode->pNextNode = pNode->pNextNode;
// 				pNode->pNextNode->pPrevNode = pNode->pPrevNode;
// 				pNode->pPrevNode = NULL;
// 				pNode->pNextNode = NULL;
// 				delete pNode;
// 				pNode = NULL;
// 				break;
// 			}
// 		}
// 		pNode = pNode->pNextNode;
// 	}
// }
// 
// void DoubleLinkedList::Print(const std::string& strDirection)
// {
// 	if (std::strcmp(strDirection.c_str(), "forward") == 0)
// 	{
// 		DoubleLinkedNode* pNode = m_pHead;
// 		while (pNode != NULL)
// 		{
// 			cout << pNode->strInfo << endl;
// 			pNode = pNode->pNextNode;
// 		}
// 		cout << endl;
// 	}
// 	else if (std::strcmp(strDirection.c_str(), "backward") == 0)
// 	{
// 		DoubleLinkedNode* pNode = m_pTail;
// 		while (pNode != NULL)
// 		{
// 			cout << pNode->strInfo << endl;
// 			pNode = pNode->pPrevNode;
// 		}
// 		cout << endl;
// 	}
// }
// 
// bool IsNodeExist(const DoubleLinkedNode& node, DoubleLinkedNode* pNode)
// {
// 	DoubleLinkedNode* pBackwardNode = pNode;
// 	while (pBackwardNode != NULL)
// 	{
// 		if (std::strcmp(pBackwardNode->strInfo.c_str(), node.strInfo.c_str()) == 0)
// 		{
// 			return true;
// 		}
// 		pBackwardNode = pBackwardNode->pPrevNode;
// 	}
// 
// 	return false;
// }
// 
// void Merge(DoubleLinkedNode* pNode1, DoubleLinkedNode *pNode2, DoubleLinkedList& outList)  
// {   
// 	if (pNode1 == NULL && pNode2 == NULL)  
// 	{
// 		return;  
// 	}
// 	else if ( pNode1 == NULL)  
// 	{
// 		if(!IsNodeExist(*pNode2, (DoubleLinkedNode*)outList.GetTailNode()))
// 		{
// 			DoubleLinkedNode* pNewNode = new DoubleLinkedNode(pNode2->strInfo);
// 			outList.InsertNode(*pNewNode);
// 		}
// 		Merge(pNode1, pNode2->pNextNode, outList);
// 	}
// 	else
// 	{
// 		if(!IsNodeExist(*pNode1, (DoubleLinkedNode*)outList.GetTailNode()))
// 		{
// 			DoubleLinkedNode* pNewNode = new DoubleLinkedNode(pNode1->strInfo);
// 			outList.InsertNode(*pNewNode);
// 		}
// 		Merge(pNode1->pNextNode, pNode2, outList);
// 	}  
// }  
// 
// void DoubleLinkedList::Union(const DoubleLinkedList& inList1, const DoubleLinkedList& inList2, DoubleLinkedList& outList)
// {
// 	DoubleLinkedNode* pHead = (DoubleLinkedNode*)outList.GetHeadNode();
// 	DoubleLinkedNode* pHead1 = (DoubleLinkedNode*)inList1.GetHeadNode();
// 	DoubleLinkedNode* pHead2 = (DoubleLinkedNode*)inList2.GetHeadNode();
// 	Merge(pHead1, pHead2, outList);
// }
// 
// void Unique(DoubleLinkedNode* pNode1, DoubleLinkedNode *pNode2, DoubleLinkedList& outList)  
// {   
// 	if (pNode1 == NULL || pNode2 == NULL)  
// 	{
// 		return;  
// 	}
// 	else
// 	{
// 		if(!IsNodeExist(*pNode1, pNode2))
// 		{
// 			;
// 		}
// 		else
// 		{
// 			if(!IsNodeExist(*pNode1, (DoubleLinkedNode*)outList.GetTailNode()))
// 			{
// 				DoubleLinkedNode* pNewNode = new DoubleLinkedNode(pNode1->strInfo);
// 				outList.InsertNode(*pNewNode);
// 			}
// 		}
// 		Unique(pNode1->pNextNode, pNode2, outList);
// 	}  
// }  
// 
// void DoubleLinkedList::Intersection(const DoubleLinkedList& inList1, const DoubleLinkedList& inList2, DoubleLinkedList& outList)
// {
// 	//DoubleLinkedNode* pHead = (DoubleLinkedNode*)outList.GetHeadNode();
// 	DoubleLinkedNode* pHead1 = (DoubleLinkedNode*)inList1.GetHeadNode();
// 	DoubleLinkedNode* pTail2 = (DoubleLinkedNode*)inList2.GetTailNode();
// 	Unique(pHead1, pTail2, outList);
// }

const DoubleLinkedNode* DoubleLinkedList::GetHeadNode() const
{
	if (this == NULL)
	{
		return NULL;
	}
	return m_pHead;
}

const DoubleLinkedNode* DoubleLinkedList::GetTailNode() const
{
	if (this == NULL)
	{
		return NULL;
	}
	return m_pTail;
}
DoubleLinkedList::DoubleLinkedList(const DoubleLinkedList& list)
{
	*this = list;
}

DoubleLinkedList& DoubleLinkedList::operator = (const DoubleLinkedList& list)
{
	if (this == &list)
	{
		return *this;
	}
	DoubleLinkedList* pDoubleLinkedList = new DoubleLinkedList;
	const DoubleLinkedNode* pHead = list.GetHeadNode();
	const DoubleLinkedNode* pTail = list.GetTailNode();
	DoubleLinkedNode* pNode = (DoubleLinkedNode*)pHead;
	while (pNode != NULL)
	{
		DoubleLinkedNode* pNewNode = new DoubleLinkedNode(*pNode);
		pDoubleLinkedList->InsertNode(*pNewNode);
		pNode = pNode->pNextNode;
	}
	m_pHead = (DoubleLinkedNode* )pDoubleLinkedList->GetHeadNode();
	m_pTail = (DoubleLinkedNode* )pDoubleLinkedList->GetTailNode();
	pDoubleLinkedList->m_digitsPerNode = list.m_digitsPerNode;
	m_digitsPerNode = list.m_digitsPerNode;
	return *pDoubleLinkedList;
}

// DoubleLinkedList& DoubleLinkedList::operator + (const DoubleLinkedList& list)
// {
// 	DoubleLinkedList* pDoubleLinkedList = new DoubleLinkedList;
// 	Union(*this, list, *pDoubleLinkedList);
// 	return *pDoubleLinkedList;
// }
// 
// DoubleLinkedList& DoubleLinkedList::operator * (const DoubleLinkedList& list)
// {
// 	DoubleLinkedList* pDoubleLinkedList = new DoubleLinkedList;
// 	Intersection(*this, list, *pDoubleLinkedList);
// 	return *pDoubleLinkedList;
// }
const std::string DoubleLinkedList::ToString() const
{
	std::string str;

	DoubleLinkedNode* pNode = m_pHead;
	while (pNode != NULL)
	{
		str.insert(str.begin(), pNode->strInfo.begin(), pNode->strInfo.end());
		pNode = pNode->pNextNode;
	}

	// erase '0' in the string's begin position
	for (int i = 0; i < str.size()/*m_digitsPerNode*/; /*i++*/)
	{
		if (*str.begin() == '0')
		{
			str.erase(str.begin());
		}
		else
		{
			break;
		}
	}

	if (str.empty())
	{
		str = std::string("0");
	}

	return str;
}

DoubleLinkedList DoubleLinkedList::operator + (const DoubleLinkedList& list)
{
	DoubleLinkedList resultList;
	resultList.m_digitsPerNode = list.m_digitsPerNode;
	DoubleLinkedNode* pNode1 = this->m_pHead;
	DoubleLinkedNode* pNode2 = list.m_pHead;

	const unsigned long long MAX_PER_NODE = pow (10, m_digitsPerNode);
	unsigned long over = 0;
	while (pNode1 != NULL && pNode2 != NULL)
	{
		unsigned long long ret = GetNumber(pNode1->strInfo) + GetNumber(pNode2->strInfo) + over;
		if (ret >= MAX_PER_NODE)
		{
			over = ret / MAX_PER_NODE;
			ret = ret % MAX_PER_NODE;
		}
		else
		{
			over = 0;
		}
		
		DoubleLinkedNode* node = new DoubleLinkedNode(GetString(ret, m_digitsPerNode));
		resultList.InsertNode(*node);

		pNode1 = pNode1->pNextNode;
		pNode2 = pNode2->pNextNode;
	}

	while (pNode1 != NULL)
	{
		unsigned long long ret = GetNumber(pNode1->strInfo) + over;
		if (ret >= MAX_PER_NODE)
		{
			over = ret / MAX_PER_NODE;
			ret = ret % MAX_PER_NODE;
		}
		else
		{
			over = 0;
		}
		DoubleLinkedNode* node = new DoubleLinkedNode(GetString(ret, m_digitsPerNode));
		resultList.InsertNode(*node);

		pNode1 = pNode1->pNextNode;
	}

	while (pNode2 != NULL)
	{
		unsigned long long ret = GetNumber(pNode2->strInfo) + over;
		if (ret >= MAX_PER_NODE)
		{
			over = ret / MAX_PER_NODE;
			ret = ret % MAX_PER_NODE;
		}
		else
		{
			over = 0;
		}
		DoubleLinkedNode* node = new DoubleLinkedNode(GetString(ret, m_digitsPerNode));
		resultList.InsertNode(*node);

		pNode2 = pNode2->pNextNode;
	}

	if (over == 1)
	{
		DoubleLinkedNode* node = new DoubleLinkedNode("1");
		resultList.InsertNode(*node);
	}

	return resultList;
}

DoubleLinkedList DoubleLinkedList::operator * (const DoubleLinkedList& list)
{
	DoubleLinkedList resultList;
	resultList.m_digitsPerNode = list.m_digitsPerNode;
	
	DoubleLinkedList list1;
	DoubleLinkedNode* pNode2 = list.m_pHead;

	const unsigned long long MAX_PER_NODE = pow (10, m_digitsPerNode);
	
	int count = -1;
	while (pNode2 != NULL)
	{
		count++;
		DoubleLinkedList list2;
		list2.m_digitsPerNode = m_digitsPerNode;
		DoubleLinkedNode* pNode1 = this->m_pHead;
		unsigned long over = 0;
		while (pNode1 != NULL)
		{
			unsigned long long ret = GetNumber(pNode1->strInfo) * GetNumber(pNode2->strInfo) + over;
			if (ret >= MAX_PER_NODE)
			{
				over = ret / MAX_PER_NODE;
				ret = ret % MAX_PER_NODE;
			}
			else
			{
				over = 0;
			}

			DoubleLinkedNode* node = new DoubleLinkedNode(GetString(ret, m_digitsPerNode));
			list2.InsertNode(*node);

			pNode1 = pNode1->pNextNode;
		}

		if (over > 0)
		{
			DoubleLinkedNode* node = new DoubleLinkedNode(GetString(over, m_digitsPerNode));
			list2.InsertNode(*node);
		}

		if (pNode2 != list.m_pHead)
		{
			std::string str(list2.ToString().c_str());
			str.insert(str.end(), m_digitsPerNode * count, '0');
			DoubleLinkedList newList(str, m_digitsPerNode);
			resultList = resultList + newList;
		}
		else
		{
			resultList = list2;
		}

		pNode2 = pNode2->pNextNode;
	}

	
	return resultList;
}
