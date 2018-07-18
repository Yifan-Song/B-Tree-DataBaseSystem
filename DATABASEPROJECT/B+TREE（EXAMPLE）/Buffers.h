#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include"BPlus_tree.h"
using namespace std;

class Buffer
{
public:
	Buffer();
	Buffer(string datapath);
	~Buffer();
	void store(string data);
	void update();
	void reOpen(string datapath);
	void clear();
	bool isUpdated();
	void setUpdated(bool situation);
private:
	bool updated = false;
	fstream DataFile;
	int maxnum = 10000;
	vector<string> bufferVector;
	string DataPath;
};

class ReadBuffer
{
public:
	ReadBuffer();
	~ReadBuffer();
	void remove(KeyType key);
	void store(KeyType key, DataType data);
	string search(KeyType key);
private:
	vector<int> KeyVec;
	vector<string> DataVec;
	int maxnum = 20;
};