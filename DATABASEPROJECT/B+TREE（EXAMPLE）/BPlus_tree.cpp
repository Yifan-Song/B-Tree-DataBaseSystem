#include "stdafx.h"
#include "BPlus_tree.h"  
#include <iostream>  
#include <algorithm>  
using namespace std;

int counter = 0;
int newKey = 0;

CBPlusTree::CBPlusTree() {
	m_Root = NULL;
	m_DataHead = NULL;
}

CBPlusTree::~CBPlusTree() {
	clear();
}

bool CBPlusTree::insert(KeyType key, const DataType& data) {
	if (inTree(key))
	{
		return false;
	}
	if (m_Root == NULL)
	{
		m_Root = new CLeafNode();
		m_DataHead = (CLeafNode*)m_Root;
		m_MaxKey = key;
	}
	if (m_Root->getKeyNum() >= MAXNUM_KEY)//根结点满则分裂
	{
		CInternalNode* newNode = new CInternalNode();
		newNode->setChild(0, m_Root);
		m_Root->split(newNode, 0); 
		m_Root = newNode;
	}
	if (key>m_MaxKey)
	{
		m_MaxKey = key;
	}
	recursive_insert(m_Root, key, data);
	return true;
}

void CBPlusTree::recursive_insert(CNode* parentNode, KeyType key, const DataType& data)
{
	if (parentNode->getType() == LEAF)//递归终点，得到叶子结点后直接插入
	{
		int maxValue = parentNode->getKeyValue(parentNode->getKeyNum() - 1);
		int minValue = parentNode->getKeyValue(0);
		CLeafNode* rightNode = ((CLeafNode*)parentNode)->getRightSibling();
		CLeafNode* leftNode = ((CLeafNode*)parentNode)->getLeftSibling();
		if (maxValue < key&&rightNode != NULL&&rightNode->getKeyValue(0) < key) rightNode->insert(key, data);
		else if (minValue > key&&leftNode != NULL)leftNode->insert(key, data);
		else ((CLeafNode*)parentNode)->insert(key, data);
	}
	else//找到子结点后继续递归
	{
		int keyIndex = parentNode->getKeyIndex(key);
		int childIndex = parentNode->getChildIndex(key, keyIndex);
		CNode* childNode = ((CInternalNode*)parentNode)->getChild(childIndex);
		if (childNode->getKeyNum() >= MAXNUM_LEAF)  // 子结点已满则进行分裂
		{
			childNode->split(parentNode, childIndex);
			if (parentNode->getKeyValue(childIndex) <= key)
			{
				childNode = ((CInternalNode*)parentNode)->getChild(childIndex + 1);
			}
		}
		recursive_insert(childNode, key, data);
	}
}

void CBPlusTree::clear()
{
	if (m_Root != NULL)
	{
		m_Root->clear();
		delete m_Root;
		m_Root = NULL;
		m_DataHead = NULL;
	}
}

bool CBPlusTree::inTree(KeyType key)
{
	return recursive_inTree(m_Root, key);
}

bool CBPlusTree::recursive_inTree(CNode *pNode, KeyType key)const
{
	if (pNode == NULL)return false;
	if (pNode->getType() == LEAF)
	{
		if (pNode->getKeyIndex(key) == -1)
		{
			return false;
		}
		else return true;
	}
	else
	{
		if (key < pNode->getKeyValue(0))
			return recursive_inTree(pNode->getChild(0), key);
		else if (key > pNode->getKeyValue(pNode->getKeyNum() - 1))
			return recursive_inTree(pNode->getChild(pNode->getKeyNum()), key);
		else {
			int i = pNode->getKeyIndex(key);
			return (recursive_inTree(pNode->getChild(i), key) || recursive_inTree(pNode->getChild(i + 1), key));
		}
	}
}

bool CBPlusTree::remove(KeyType key)
{
	if (!inTree(key))
	{
		return false;
	}
	if (m_Root->getKeyNum() == 1)//特殊情况
	{
		if (m_Root->getType() == LEAF)
		{
			clear();
			return true;
		}
		else
		{
			CNode *pChild1 = ((CInternalNode*)m_Root)->getChild(0);
			CNode *pChild2 = ((CInternalNode*)m_Root)->getChild(1);
			if (pChild1->getKeyNum() == MINNUM_KEY && pChild2->getKeyNum() == MINNUM_KEY)
			{
				pChild1->mergeChild(m_Root, pChild2, 0);
				delete m_Root;
				m_Root = pChild1;
			}
		}
	}
	newKey = key;
	recursive_remove(m_Root, key);
	return true;
}

void CBPlusTree::recursive_remove(CNode* parentNode, KeyType key)
{
	counter++;
	int keyIndex = parentNode->getKeyIndex(key);
	int childIndex = parentNode->getChildIndex(key, keyIndex);
	if (parentNode->getType() == LEAF)//递归终点找到目标叶子节点后删除
	{
		if (key == m_MaxKey&&keyIndex>0)
		{
			m_MaxKey = parentNode->getKeyValue(keyIndex - 1);
		}
		if (keyIndex == -1)
		{
			int leftIndex = -1;
			int rightIndex = -1;
			if (((CLeafNode*)parentNode)->getLeftSibling())leftIndex = ((CLeafNode*)parentNode)->getLeftSibling()->getKeyIndex(key);
			if (((CLeafNode*)parentNode)->getRightSibling())rightIndex = ((CLeafNode*)parentNode)->getRightSibling()->getKeyIndex(key);
			if (leftIndex != -1)((CLeafNode*)parentNode)->getLeftSibling()->removeKey(leftIndex,leftIndex);
			else if (rightIndex != -1)((CLeafNode*)parentNode)->getRightSibling()->removeKey(rightIndex, rightIndex);
			else throw runtime_error("Error");
		}
		else parentNode->removeKey(keyIndex, childIndex);
		if (childIndex == 0 && m_Root->getType() != LEAF && parentNode != m_DataHead)//如果键值在内部结点中存在，也要相应的替换内部结点  
		{
			changeKey(m_Root, key, parentNode->getKeyValue(0));
		}
	}
	else //得到内结点后继续递归
	{
		CNode *pChildNode = ((CInternalNode*)parentNode)->getChild(childIndex);
		if (pChildNode->getKeyNum() == MINNUM_KEY)//节点键值数目达到下限值，进行操作  
		{
			CNode *pLeft = childIndex>0 ? ((CInternalNode*)parentNode)->getChild(childIndex - 1) : NULL;
			CNode *pRight = childIndex<parentNode->getKeyNum() ? ((CInternalNode*)parentNode)->getChild(childIndex + 1) : NULL;
			if (pLeft && pLeft->getKeyNum()>MINNUM_KEY)//先考虑借节点
			{
				pChildNode->borrowFrom(pLeft, parentNode, childIndex - 1, LEFT);
			}
			else if (pRight && pRight->getKeyNum()>MINNUM_KEY)
			{
				pChildNode->borrowFrom(pRight, parentNode, childIndex, RIGHT);
			}
			else if (pLeft)//左右兄弟节点都不可借，考虑合并
			{
;				pLeft->mergeChild(parentNode, pChildNode, childIndex - 1);
				pChildNode = pLeft;
			}
			else if (pRight)
			{
				pChildNode->mergeChild(parentNode, pRight, childIndex);
			}
		}
		recursive_remove(pChildNode, key);
	}
}

void CBPlusTree::changeKey(CNode *pNode, KeyType oldKey, KeyType newKey)
{
	if (pNode != NULL && pNode->getType() != LEAF)
	{
		int keyIndex = pNode->getKeyIndex(oldKey);
		if (keyIndex<pNode->getKeyNum() && oldKey == pNode->getKeyValue(keyIndex))
		{
			pNode->setKeyValue(keyIndex, newKey);
		}
		else
		{
			changeKey(((CInternalNode*)pNode)->getChild(keyIndex), oldKey, newKey);
		}
	}
}

DataType CBPlusTree::search(KeyType key)
{
	return recursive_search(m_Root, key);
}

DataType CBPlusTree::recursive_search(CNode* pNode, KeyType key)const
{
	if (pNode->getType() == LEAF)
	{
		bool flag = false;
		for (int i = 0;i < pNode->getKeyNum();i++)
		{
			if (pNode->getKeyValue(i) == key)flag = true;
		}
		if (!flag)return "NONE";
		else return pNode->getData(pNode->getKeyIndex(key));
	}
	else
	{
		if (key < pNode->getKeyValue(0))
			return recursive_search(pNode->getChild(0), key);
		else if (key > pNode->getKeyValue(pNode->getKeyNum() - 1))
			return recursive_search(pNode->getChild(pNode->getKeyNum()), key);
		else {
			int i = pNode->getKeyIndex(key);
			if (recursive_search(pNode->getChild(i), key) == "NONE")return recursive_search(pNode->getChild(i + 1), key);
			else return recursive_search(pNode->getChild(i), key);
		}
	}
}

bool CBPlusTree::replace(KeyType key, DataType index)
{
	CNode* node = getNode(key);
	if (node == NULL)return false;
	int keyIndex = node->getKeyIndex(key);
	if (keyIndex == -1)return false;
	((CLeafNode*)node)->setData(keyIndex, index);
	return true;
}

void CBPlusTree::new_print()
{
	recursive_printFromRoot(m_Root);
}

void CBPlusTree::recursive_printFromRoot(CNode* pNode)
{
	if (pNode->getType() == LEAF)
	{
		for (int i = 0;i < pNode->getKeyNum();i++)
		{
			cout << pNode->getKeyValue(i) << endl;
		}
	}
	else
	{
		for (int i = 0;i < pNode->getKeyNum() + 1;i++)
		{
			cout << "子树数目为：" << pNode->getKeyNum() + 1 << "当下进入第" << i + 1 << "个子树\n";
			recursive_printFromRoot(pNode->getChild(i));
		}
	}
}

void CBPlusTree::print()const
{
	printInConcavo(m_Root, 10);
}

void CBPlusTree::printInConcavo(CNode *pNode, int count) const {
	if (pNode != NULL)
	{
		int i, j;
		for (i = 0; i<pNode->getKeyNum(); ++i)
		{
			if (pNode->getType() != LEAF)
			{
				printInConcavo(((CInternalNode*)pNode)->getChild(i), count - 2);
			}
			for (j = count; j >= 0; --j)
			{
				cout << "-";
			}
			cout << pNode->getKeyValue(i) << endl;
		}
		if (pNode->getType() != LEAF)
		{
			printInConcavo(((CInternalNode*)pNode)->getChild(i), count - 2);
		}
	}
}

CNode* CBPlusTree::getRoot()
{
	return m_Root;
}

CNode* CBPlusTree::getNode(KeyType key)
{
	return recursive_getNode(key, m_Root);
}

CNode* CBPlusTree::recursive_getNode(KeyType key, CNode* pNode)
{
	if (pNode == NULL)return NULL;
	if (pNode->getType() == LEAF)
	{
		if (pNode->getKeyIndex(key) == -1)
		{
			return NULL;
		}
		else return pNode;
	}
	else
	{
		if (key < pNode->getKeyValue(0))
			return recursive_getNode(key, pNode->getChild(0));
		else if (key > pNode->getKeyValue(pNode->getKeyNum() - 1))
			return recursive_getNode(key, pNode->getChild(pNode->getKeyNum()));
		else {
			int i = pNode->getKeyIndex(key);
			{
				CNode* node = recursive_getNode(key, pNode->getChild(i));
				if (node == NULL)return recursive_getNode(key, pNode->getChild(i + 1));
				else return node;
			}
		}
	}
}