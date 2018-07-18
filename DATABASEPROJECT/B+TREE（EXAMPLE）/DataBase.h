#pragma once
#include "Buffers.h"
#include "BPlus_tree.h"
#include<iostream>
#include<fstream>
#include<string>
#include<queue>

class DataBase
{
public:
	DataBase();
	DataBase(string indexPath, string dataPath);
	~DataBase();
	void close();
	void test_store(KeyType key, string data);
	void user_store(KeyType key, DataType data);
	void test_replace(KeyType key, DataType newData);
	void user_replace(KeyType key, DataType newData);
	void test_remove(KeyType key);
	void user_remove(KeyType key);
	string test_fetch(KeyType key);
	void user_fetch(KeyType key);
	void Read();
	void reOpen(string indexPath, string dataPath);
	bool isOpen();
	void newOpen(string indexPath, string dataPath);
	void clear();
private:
	bool closed;
	int endPosition = 0;
	CBPlusTree Tree;
	fstream IndexStream;
	fstream DataStream;
	string IndexPath;
	string DataPath;
	Buffer buffer;
	ReadBuffer readBuffer;
	string test_fetchIndex(KeyType key);
	void update();
	void test_storeIndex(KeyType key, DataType index);
	string recursive_update(CNode* pNode);
};

bool isNumChar(char ch);
int getPosition(string s);
int getLength(string s);
string getStr(int num);

const int MaxPreLength = 3;