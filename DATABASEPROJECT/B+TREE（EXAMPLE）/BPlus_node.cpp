#include "stdafx.h"
#include "BPlus_node.h"  
#include<iostream>

CNode::CNode() {
	setType(LEAF);
	setKeyNum(0);
}

CNode::~CNode() {
	setKeyNum(0);
}

int CNode::getKeyIndex(KeyType key)const
{
	if (m_Type == LEAF)
	{
		bool flag = false;
		for (int i = 0;i < getKeyNum();i++)
		{
			if (getKeyValue(i) == key)flag = true;
		}
		if (!flag)return -1;
	}
	int left = 0;
	int right = getKeyNum();
	int current;
	while (left != right)
	{
		current = (left + right) / 2;
		KeyType currentKey = getKeyValue(current);
		if (key>currentKey)
		{
			left = current + 1;
		}
		else
		{
			right = current;
		}
	}
	return left;
}


CInternalNode::CInternalNode() :CNode() {
	setType(INTERNAL);
}

CInternalNode::~CInternalNode() {}

void CInternalNode::clear()
{
	for (int i = 0; i <= m_KeyNum; ++i)
	{
		m_Childs[i]->clear();
		delete m_Childs[i];
		m_Childs[i] = NULL;
	}
}

void CInternalNode::split(CNode* parentNode, int childIndex)
{
	CInternalNode* newNode = new CInternalNode();
	newNode->setKeyNum(MINNUM_KEY);
	int i;
	for (i = 0; i<MINNUM_KEY; ++i) 
	{
		newNode->setKeyValue(i, m_KeyValues[i + MINNUM_CHILD]);
	}
	for (i = 0; i<MINNUM_CHILD; ++i)
	{
		newNode->setChild(i, m_Childs[i + MINNUM_CHILD]);
	}
	setKeyNum(MINNUM_KEY); 
	((CInternalNode*)parentNode)->insert(childIndex, childIndex + 1, m_KeyValues[MINNUM_KEY], newNode);
}

void CInternalNode::insert(int keyIndex, int childIndex, KeyType key, CNode* childNode) {
	int i = getKeyNum();
	for (i ; i>keyIndex; --i)
	{
		setChild(i + 1, m_Childs[i]);
		setKeyValue(i, m_KeyValues[i - 1]);
	}
	if (i == childIndex)
	{
		setChild(i + 1, m_Childs[i]);
	}
	setChild(childIndex, childNode);
	setKeyValue(keyIndex, key);
	setKeyNum(m_KeyNum + 1);
}

void CInternalNode::mergeChild(CNode* parentNode, CNode* childNode, int keyIndex)
{
	insert(MINNUM_KEY, MINNUM_KEY + 1, parentNode->getKeyValue(keyIndex), ((CInternalNode*)childNode)->getChild(0));
	int i;
	for (i = 1; i <= childNode->getKeyNum(); ++i)
	{
		insert(MINNUM_KEY + i, MINNUM_KEY + i + 1, childNode->getKeyValue(i - 1), ((CInternalNode*)childNode)->getChild(i));
	}
	int num = parentNode->getKeyNum() - 1;
	for (int i = keyIndex;i < num;i++)
	{
		((CInternalNode*)parentNode)->setKeyValue(i, parentNode->getKeyValue(i + 1));
		((CInternalNode*)parentNode)->setChild(i + 1, ((CInternalNode*)parentNode)->getChild(i + 2));
	}
	parentNode->setKeyNum(num);
}

void CInternalNode::removeKey(int keyIndex, int childIndex)
{
	for (int i = 0; i<getKeyNum() - keyIndex ; ++i)
	{
		setKeyValue(keyIndex + i, getKeyValue(keyIndex + i + 1));
		setChild(childIndex + i, getChild(childIndex + i + 1));
	}
	setKeyNum(getKeyNum() - 1);
}

void CInternalNode::borrowFrom(CNode* siblingNode, CNode* parentNode, int keyIndex, SIBLING_DIRECTION d)
{
	switch (d)
	{
	case LEFT:
	{
		insert(0, 0, parentNode->getKeyValue(keyIndex), ((CInternalNode*)siblingNode)->getChild(siblingNode->getKeyNum()));
		parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(siblingNode->getKeyNum() - 1));
		siblingNode->removeKey(siblingNode->getKeyNum() - 1, siblingNode->getKeyNum());
	}
	break;
	case RIGHT:
	{
		insert(getKeyNum(), getKeyNum() + 1, parentNode->getKeyValue(keyIndex), ((CInternalNode*)siblingNode)->getChild(0));
		parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(0));
		siblingNode->removeKey(0, 0);
	}
	break;
	default:
		break;
	}
}

int CInternalNode::getChildIndex(KeyType key, int keyIndex)const
{
	if (key == getKeyValue(keyIndex))
	{
		return keyIndex + 1;
	}
	else
	{
		return keyIndex;
	}
}

CLeafNode::CLeafNode() :CNode() {
	setType(LEAF);
	setLeftSibling(NULL);
	setRightSibling(NULL);
}

CLeafNode::~CLeafNode() {}

void CLeafNode::clear(){}

void CLeafNode::insert(KeyType key, const DataType& data)
{
	int i = m_KeyNum;
	for (i ; i >= 1 && m_KeyValues[i - 1]>key; --i)
	{
		setKeyValue(i, m_KeyValues[i - 1]);
		setData(i, m_Datas[i - 1]);
	}
	setKeyValue(i, key);
	setData(i, data);
	setKeyNum(m_KeyNum + 1);
}

void CLeafNode::split(CNode* parentNode, int childIndex)
{
	CLeafNode* newNode = new CLeafNode();
	setKeyNum(MINNUM_LEAF);
	newNode->setKeyNum(MINNUM_LEAF + 1);
	newNode->setRightSibling(getRightSibling());
	setRightSibling(newNode);
	newNode->setLeftSibling(this);
	int i;
	for (i = 0; i<MINNUM_LEAF + 1; ++i)
	{
		newNode->setKeyValue(i, m_KeyValues[i + MINNUM_LEAF]);
	}
	for (i = 0; i<MINNUM_LEAF + 1; ++i)
	{
		newNode->setData(i, m_Datas[i + MINNUM_LEAF]);
	}
	((CInternalNode*)parentNode)->insert(childIndex, childIndex + 1, m_KeyValues[MINNUM_LEAF], newNode);
}

void CLeafNode::mergeChild(CNode* parentNode, CNode* childNode, int keyIndex)
{
	for (int i = 0; i<childNode->getKeyNum(); ++i)
	{
		insert(childNode->getKeyValue(i), ((CLeafNode*)childNode)->getData(i));
	}
	setRightSibling(((CLeafNode*)childNode)->getRightSibling());
	parentNode->removeKey(keyIndex, keyIndex + 1);
}

void CLeafNode::removeKey(int keyIndex, int childIndex)
{
	for (int i = keyIndex; i<getKeyNum() - 1; ++i)
	{
		setKeyValue(i, getKeyValue(i + 1));
		setData(i, getData(i + 1));
	}
	setKeyNum(getKeyNum() - 1);
}

void CLeafNode::borrowFrom(CNode* siblingNode, CNode* parentNode, int keyIndex, SIBLING_DIRECTION d)
{
	switch (d)
	{
	case LEFT:
	{
		insert(siblingNode->getKeyValue(siblingNode->getKeyNum() - 1), ((CLeafNode*)siblingNode)->getData(siblingNode->getKeyNum() - 1));
		siblingNode->removeKey(siblingNode->getKeyNum() - 1, siblingNode->getKeyNum() - 1);
		parentNode->setKeyValue(keyIndex, getKeyValue(0));
	}
	break;
	case RIGHT:
	{
		insert(siblingNode->getKeyValue(0), ((CLeafNode*)siblingNode)->getData(0));
		siblingNode->removeKey(0, 0);
		parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(0));
	}
	break;
	default:
		break;
	}
}

int CLeafNode::getChildIndex(KeyType key, int keyIndex)const
{
	return keyIndex;
}