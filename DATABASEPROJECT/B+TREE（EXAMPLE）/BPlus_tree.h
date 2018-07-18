#pragma once

#include "BPlus_node.h"  
#include <vector>  
using namespace std;

class CBPlusTree {
public:
	CBPlusTree();
	~CBPlusTree();
	bool insert(KeyType key, const DataType& data);
	bool remove(KeyType key);
	DataType search(KeyType key);
	bool inTree(KeyType key);
	bool replace(KeyType key, DataType newData);
	void clear();
	void new_print();
	void print() const;
	CNode* getRoot();
	bool new_remove(KeyType data);
	CNode* getNode(KeyType key);
private:
	CNode* recursive_getNode(KeyType key, CNode* pNode);
	void recursive_insert(CNode* parentNode, KeyType key, const DataType& data);
	void recursive_remove(CNode* parentNode, KeyType key);
	bool recursive_inTree(CNode *pNode, KeyType key)const;
	DataType recursive_search(CNode* pNode, KeyType key)const;
	void changeKey(CNode *pNode, KeyType oldKey, KeyType newKey);
	void recursive_printFromRoot(CNode* pNode);
	void printInConcavo(CNode *pNode, int count)const;
	CNode* m_Root;
	CLeafNode* m_DataHead;
	KeyType m_MaxKey;
};
