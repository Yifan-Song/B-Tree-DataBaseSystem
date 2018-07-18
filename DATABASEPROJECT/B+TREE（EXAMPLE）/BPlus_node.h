#pragma once
#define NULL 0
#include<string>

using  namespace std;
enum NODE_TYPE { INTERNAL, LEAF };
enum SIBLING_DIRECTION { LEFT, RIGHT };
typedef int KeyType; 
typedef string DataType; 

const int ORDER = 30; 
const int MINNUM_KEY = ORDER - 1;
const int MAXNUM_KEY = 2 * ORDER - 1; 
const int MINNUM_CHILD = MINNUM_KEY + 1;
const int MAXNUM_CHILD = MAXNUM_KEY + 1;
const int MINNUM_LEAF = MINNUM_KEY;
const int MAXNUM_LEAF = MAXNUM_KEY;

class CNode {
public:
	CNode();
	virtual ~CNode();
	NODE_TYPE getType() const { return m_Type; }
	void setType(NODE_TYPE type) { m_Type = type; }
	int getKeyNum() const { return m_KeyNum; }
	void setKeyNum(int n) { m_KeyNum = n; }
	KeyType getKeyValue(int i) const { return m_KeyValues[i]; }
	void setKeyValue(int i, KeyType key) { m_KeyValues[i] = key; }
	int getKeyIndex(KeyType key)const;  
	virtual void removeKey(int keyIndex, int childIndex) = 0;  
	virtual void split(CNode* parentNode, int childIndex) = 0;
	virtual void mergeChild(CNode* parentNode, CNode* childNode, int keyIndex) = 0;
	virtual void clear() = 0;
	virtual void borrowFrom(CNode* destNode, CNode* parentNode, int keyIndex, SIBLING_DIRECTION d) = 0;
	virtual int getChildIndex(KeyType key, int keyIndex)const = 0;
	virtual CNode* getChild(int i) const = 0;
	virtual DataType getData(int i) const = 0;
protected:
	NODE_TYPE m_Type;
	int m_KeyNum;
	KeyType m_KeyValues[MAXNUM_KEY];
};

class CInternalNode : public CNode {
public:
	CInternalNode();
	virtual ~CInternalNode();
	virtual CNode* getChild(int i) const { return m_Childs[i]; }
	virtual DataType getData(int i) const { return NULL; };
	void setChild(int i, CNode* child) { m_Childs[i] = child; }
	void insert(int keyIndex, int childIndex, KeyType key, CNode* childNode);
	virtual void split(CNode* parentNode, int childIndex);
	virtual void mergeChild(CNode* parentNode, CNode* childNode, int keyIndex);
	virtual void removeKey(int keyIndex, int childIndex);
	virtual void clear();
	virtual void borrowFrom(CNode* destNode, CNode* parentNode, int keyIndex, SIBLING_DIRECTION d);
	virtual int getChildIndex(KeyType key, int keyIndex)const;
private:
	CNode* m_Childs[MAXNUM_CHILD];
};

class CLeafNode : public CNode {
public:
	CLeafNode();
	virtual ~CLeafNode();
	CLeafNode* getLeftSibling() const { return m_LeftSibling; }
	void setLeftSibling(CLeafNode* node) { m_LeftSibling = node; }
	CLeafNode* getRightSibling() const { return m_RightSibling; }
	void setRightSibling(CLeafNode* node) { m_RightSibling = node; }
	virtual DataType getData(int i) const { return m_Datas[i]; }
	virtual CNode* getChild(int i) const { return NULL; };
	void setData(int i, const DataType& data) { m_Datas[i] = data; }
	void insert(KeyType key, const DataType& data);
	virtual void split(CNode* parentNode, int childIndex);
	virtual void mergeChild(CNode* parentNode, CNode* childNode, int keyIndex);
	virtual void removeKey(int keyIndex, int childIndex);
	virtual void clear();
	virtual void borrowFrom(CNode* destNode, CNode* parentNode, int keyIndex, SIBLING_DIRECTION d);
	virtual int getChildIndex(KeyType key, int keyIndex)const;
private:
	CLeafNode* m_LeftSibling;
	CLeafNode* m_RightSibling;
	DataType m_Datas[MAXNUM_LEAF];
};