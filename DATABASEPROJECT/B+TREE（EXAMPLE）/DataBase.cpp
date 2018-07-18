#include "stdafx.h"
#include"Buffers.h"
#include "DataBase.h"
#include<fstream>
#include<string>
#include<sstream>
#include<time.h>
using namespace std;

string modifyData(string s)
{
	string data = "";
	for (auto i : s)
	{
		data += i;
		if (i == ';')return data;
	}
}

string getData(string s)
{
	string str = "";
	for (auto i : s)
	{
		if (i == ';')break;
		else str += i;
	}
	return str;
}

string getStr(int num)
{
	string s;
	stringstream tempStream;
	tempStream << num;
	tempStream >> s;
	return s;
}

int getPosition(string s)
{
	string positionStr = "";
	for (auto i : s)
	{
		if (int(i) < 58 && int(i) > 47)positionStr += i;
		else if (i == ':')break;
		else
		{
			cout << "数据不合法";
			throw (runtime_error("数据不合法"));
		}
	}
	return atoi(positionStr.c_str());
}

int getLength(string s)
{
	string lengthStr = "";
	bool flag = false;
	for (auto i : s)
	{
		if (int(i) < 58 && int(i) > 47)
		{
			if(flag)lengthStr += i;
		}
		else if (i == ';')break;
		else if (i == ':')flag = true;
		else
		{
			cout << "数据不合法";
			throw(runtime_error("数据不合法"));
		}
	}
	return atoi(lengthStr.c_str());
}

string standardizeLengthStr(string s)
{
	int len = s.length();
	if (len > MaxPreLength)throw runtime_error("数据过长");
	string standardStr;
	standardStr += s;
	for (len;len < MaxPreLength;len++)
	{
		standardStr += " ";
	}
	return standardStr;
}

string standardizeData(string s, int len)
{
	string standardizeStr = s;
	int originLen = s.length();
	if (originLen>len)throw(runtime_error("非法调用"));
	for (originLen;originLen < len;originLen++)
	{
		standardizeStr += " ";
	}
	return standardizeStr;
}

DataBase::DataBase()
{
	closed = false;
	IndexPath = "index.txt";
	DataPath = "data.txt";
	IndexStream.open(IndexPath, ios::in | ios::out | ios::binary);
	DataStream.open(DataPath, ios::in | ios::out | ios::binary);
	if (!IndexStream.is_open())IndexStream.open(IndexPath, ios::in | ios::out | ios::binary | ios::trunc);
	if (!DataStream.is_open())DataStream.open(DataPath, ios::in | ios::out | ios::binary | ios::trunc);
	buffer.reOpen(DataPath);
}

DataBase::DataBase(string indexPath,string dataPath)
{
	closed = false;
	IndexPath = indexPath;
	DataPath = dataPath;
	IndexStream.open(IndexPath, ios::in | ios::out | ios::binary);
	DataStream.open(DataPath, ios::in | ios::out | ios::binary);
	buffer.reOpen(dataPath);
}

void DataBase::reOpen(string indexPath, string dataPath)
{
	closed = false;
	IndexPath = indexPath;
	DataPath = dataPath;
	IndexStream.close();
	DataStream.close();
	IndexStream.open(IndexPath, ios::in | ios::out | ios::binary);
	DataStream.open(DataPath, ios::in | ios::out | ios::binary);
	buffer.reOpen(dataPath);
}

void DataBase::newOpen(string indexPath, string dataPath)
{
	closed = false;
	IndexPath = indexPath;
	DataPath = dataPath;
	IndexStream.open(IndexPath, ios::in | ios::out | ios::binary);
	DataStream.open(DataPath, ios::in | ios::out | ios::binary);
	if (!IndexStream.is_open())IndexStream.open(IndexPath, ios::in | ios::out | ios::binary | ios::trunc);
	if (!DataStream.is_open())DataStream.open(DataPath, ios::in | ios::out | ios::binary | ios::trunc);
	buffer.reOpen(dataPath);
}

DataBase::~DataBase(){}

void DataBase::close()
{
	closed = true;
	update();
	buffer.update();
	IndexStream.close();
	DataStream.close();
	buffer.clear();
}

void DataBase::test_storeIndex(KeyType key, DataType index)
{
	if (closed)return;
	Tree.insert(key,index);
}

void DataBase::user_store(KeyType key, DataType data)
{
	if (closed)
	{
		cout << "数据库已关闭，无法储存！\n";
		return;
	}
	string keyStr = getStr(key);
	string positionStr = getStr(endPosition);
	string dataLengthStr;
	string index;
	int keyLength = keyStr.length();
	data += ';';
	int dataLength = data.length();
	dataLengthStr = getStr(dataLength);
	index = positionStr + ":" + dataLengthStr + ";";
	bool flag = Tree.insert(key, index);
	if (!flag)
	{
		cout << "键值" << key << "已存在，存储失败！\n";
	}
	else
	{
		readBuffer.store(key, data);
		buffer.store(data);
		endPosition += dataLength;
		cout << "键值：" << key << "数据：" << data << "存储成功！\n";
	}
}

void DataBase::test_store(KeyType key, string data)
{
	if (closed)return;
	string keyStr = getStr(key);
	string positionStr = getStr(endPosition);
	string dataLengthStr;
	string index;
	int keyLength = keyStr.length();
	data += ';';
	int dataLength = data.length();
	dataLengthStr = getStr(dataLength);
	index = positionStr + ":" + dataLengthStr + ";";
	bool flag = Tree.insert(key, index);
	if (!flag)
	{
		return;
	}
	else
	{
		readBuffer.store(key, data);
		buffer.store(data);
		endPosition += dataLength;
	}
}

void DataBase::test_remove(KeyType key)
{
	if (closed)return;
	readBuffer.remove(key);
	Tree.remove(key);
}

void DataBase::user_remove(KeyType key)
{
	if (closed)
	{
		cout << "数据库已关闭，无法删除！\n";
		return;
	}
	readBuffer.remove(key);
	bool flag = Tree.remove(key);
	if (!flag)
	{
		cout << "键值" << key << "不存在，删除失败！\n";
	}
	else
	{
		cout << "删除成功！\n";
	}
}

void DataBase::test_replace(KeyType key, string newData)
{
	if (closed)return;
	if (!Tree.inTree(key))return;
	string index = test_fetchIndex(key);
	int position = getPosition(index);
	int length = getLength(index);
	newData += ";";
	int newLength = newData.length();
	if (newData.length() <= length)
	{
		buffer.update();
		if (buffer.isUpdated())
		{
			DataStream.close();
			DataStream.open(DataPath, ios::in | ios::out | ios::binary);
		}
		DataStream.seekg(position);
		newData = standardizeData(newData, length);
		DataStream.write(newData.c_str(), length);
		readBuffer.store(key, newData);
	}
	else
	{
		string newIndex;
		newIndex = getStr(endPosition) + ":" + getStr(newLength) + ";";
		buffer.store(newData);
		endPosition += newLength;
		Tree.replace(key, newIndex);
		readBuffer.store(key, newData);
	}
}

void DataBase::user_replace(KeyType key, string newData)
{
	if (closed)
	{
		cout << "数据库已关闭，无法修改！\n";
		return;
	}
	if (!Tree.inTree(key))
	{
		cout << "键值" << key << "不存在，修改失败！\n";
		return;
	}
	string index = test_fetchIndex(key);
	int position = getPosition(index);
	int length = getLength(index);
	newData += ";";
	int newLength = newData.length();
	if (newData.length() <= length)
	{
		buffer.update();
		if (buffer.isUpdated())
		{
			DataStream.close();
			DataStream.open(DataPath, ios::in | ios::out | ios::binary);
		}
		newData = standardizeData(newData, length);
		DataStream.seekg(position);
		DataStream.write(newData.c_str(), length);
		readBuffer.store(key, newData);
	}
	else
	{
		string newIndex;
		newIndex = getStr(endPosition) + ":" + getStr(newLength) + ";";
		buffer.store(newData);
		endPosition += newLength;
		Tree.replace(key, newIndex);
		readBuffer.store(key, newData);
	}
	cout << "数据更改成功\n";
}

string DataBase::test_fetch(KeyType key)
{
	if (closed)return"";
	string tempResult = readBuffer.search(key);
	if (tempResult != "")return tempResult;
	buffer.update();
	if (buffer.isUpdated())
	{
		DataStream.close();
		DataStream.open(DataPath, ios::in | ios::out | ios::binary);
	}
	bool flag = Tree.inTree(key);
	if (!flag)
	{
		return "";
	}
	else
	{
		char temp[256] = {};
		char* tempData = temp;
		string s = Tree.search(key);
		int position = getPosition(s);
		int len = getLength(s);
		DataStream.seekg(position);
		DataStream.read(tempData, len - 1);
		string data = getData(tempData);
		readBuffer.store(key, data);
		return data;
	}
}

void DataBase::user_fetch(KeyType key)
{
	buffer.update();
	if (buffer.isUpdated())
	{
		DataStream.close();
		DataStream.open(DataPath, ios::in | ios::out | ios::binary);
	}
	if (closed)
	{
		cout << "数据库已关闭，无法查找！\n";
		return;
	}
	string tempResult = readBuffer.search(key);
	if (tempResult != "")
	{
		cout << "数据为：" << tempResult << endl;
		return;
	}
	bool flag = Tree.inTree(key);
	if (!flag)
	{
		cout << "键值" << key << "不存在，查找失败！\n";
	}
	else
	{
		char temp[256] = {};
		char* tempData = temp;
		string s = Tree.search(key);
		int position = getPosition(s);
		int len = getLength(s);
		DataStream.seekg(position);
		DataStream.read(tempData, len - 1);
		string data = getData(tempData);
		readBuffer.store(key, data);
		cout << "数据为：" << data << endl;
	}
}

string DataBase::test_fetchIndex(KeyType key)
{
	if (closed)return"";
	bool flag = Tree.inTree(key);
	if (!flag)
	{
		return "";
	}
	else
	{
		string index = Tree.search(key);
		return index;
	}
}

void DataBase::Read()
{
	if (closed)return;
	if (!isOpen())return;
	int tempId = 1;
	int endPosition = 0;
	char ch = IndexStream.get();
	if (IndexStream.eof())
	{
		char c = DataStream.get();
		if (DataStream.eof())return;
		else DataStream.putback(c);
		while (true)
		{
			DataStream.seekg(endPosition);
			int length;
			string index;
			string tempStr;
			string data = "";

			while (true)
			{
				char temp[256] = {};
				char* tempS = temp;
				DataStream.read(tempS, 11);
				tempStr = tempS;
				string tempData = modifyData(tempStr);
				if (tempData != "" || tempData[0] == ';')
				{
					data += tempData;
					break;
				}
				else
				{
					data += tempStr;
				}
			}

			index = getStr(endPosition) + ":" + getStr(data.length()) + ";";
			test_storeIndex(tempId, index);
			tempId++;
			endPosition += data.length();
			DataStream.clear();
			DataStream.seekg(endPosition);
			char ch = DataStream.get();
			if (DataStream.eof())break;
			else DataStream.putback(ch);
		}
	}
	else
	{
		IndexStream.putback(ch);
		while (true)
		{
			int length;
			int key;
			string index;
			string tempStr;
			char temp[256] = {};
			char* tempIndex = temp;
			IndexStream.read(tempIndex, MaxPreLength + 1);
			length = atoi(tempIndex);
			IndexStream.read(tempIndex, length);
			tempStr = tempIndex;
			key = atoi(tempIndex);
			index = tempStr.substr(tempStr.find_first_of(":") + 1, tempStr.length());
			test_storeIndex(key, index);
			endPosition += getLength(index);
			char ch = IndexStream.get();
			if (IndexStream.eof())break;
			else IndexStream.putback(ch);
		}
	}
}


void DataBase::update()
{
	IndexStream.close();
	IndexStream.open(IndexPath, ios::in | ios::out | ios::binary | ios::trunc);
	string tempStr = recursive_update(Tree.getRoot());
	IndexStream.seekp(0, ios::end);
	IndexStream.write(tempStr.c_str(), tempStr.length());
}

string DataBase::recursive_update(CNode* pNode)
{
	string tempStr;
	if (pNode != NULL)
	{
		if (pNode->getType() == LEAF)
		{
			string tempStr = "";
			for (int i = 0;i < pNode->getKeyNum();i++)
			{
				string index;
				index = getStr(pNode->getKeyValue(i)) + ":" + pNode->getData(i);
				index = standardizeLengthStr(getStr(index.length())) + ":" + index;
				tempStr += index;
			}
			return tempStr;
		}
		else
		{
			for (int i = 0;i < pNode->getKeyNum()+1;i++)
			{
				tempStr += recursive_update(pNode->getChild(i));
			}
		}
	}
	return tempStr;
}

bool DataBase::isOpen()
{
	return (IndexStream.is_open() && DataStream.is_open());
}

void DataBase::clear()
{
	IndexStream.close();
	IndexStream.open(IndexPath, ios::in | ios::out | ios::binary | ios::trunc);
	IndexStream.close();
	DataStream.close();
	DataStream.open(DataPath, ios::in | ios::out | ios::binary | ios::trunc);
	DataStream.close();
}