#include "stdafx.h"
#include<string>
#include<sstream>
#include"Buffers.h"

string getBufferData(string s)
{
	string data;
	for (auto ch : s)
	{
		if (ch == ';')return data;
		data += ch;
	}
	return data;
}

Buffer::Buffer()
{
	DataPath = "data.txt";
}

Buffer::Buffer(string datapath)
{
	DataPath = datapath;
}

Buffer::~Buffer()
{
	update();
}

void Buffer::store(string s)
{
	if (bufferVector.size() >= maxnum)update();
	bufferVector.push_back(s);
}

void Buffer::update()
{
	if (bufferVector.size() == 0)
	{
		updated = false;
		return;
	}
	updated = true;
	DataFile.open(DataPath, ios::in | ios::out | ios::binary);
	string dataStr = "";
	for (auto data : bufferVector)
	{
		dataStr += data;
	}
	bufferVector.clear();
	DataFile.seekp(0, ios::end);
	DataFile.write(dataStr.c_str(), dataStr.length());
	DataFile.close();
}

void Buffer::reOpen(string s)
{
	DataFile.close();
	DataPath = s;
}

void Buffer::clear()
{
	DataFile.close();
	bufferVector.clear();
}

bool Buffer::isUpdated()
{
	return updated;
}

void Buffer::setUpdated(bool situation)
{
	updated = situation;
}

ReadBuffer::ReadBuffer()
{

}

ReadBuffer::~ReadBuffer()
{

}

void ReadBuffer::store(KeyType key, DataType data)
{
	string newData = getBufferData(data);
	if (KeyVec.size() > maxnum)
	{
		KeyVec.pop_back();
		DataVec.pop_back();
	}
	KeyVec.insert(KeyVec.begin(), key);
	DataVec.insert(DataVec.begin(), newData);
}

string ReadBuffer::search(KeyType key)
{
	string data;
	for (int i = 0;i < KeyVec.size();i++)
	{
		if (key == KeyVec[i])
		{
			string data = DataVec[i];
			KeyVec.erase(find(KeyVec.begin(), KeyVec.end(), key));
			DataVec.erase(find(DataVec.begin(), DataVec.end(), data));
			store(key, data);
			return data;
		}
	}
	return "";
}

void ReadBuffer::remove(KeyType key)
{
	for (int i = 0;i < KeyVec.size();i++)
	{
		if (key == KeyVec[i])
		{
			KeyVec.erase(KeyVec.begin() + i);
			DataVec.erase(DataVec.begin() + i);
		}
	}
}