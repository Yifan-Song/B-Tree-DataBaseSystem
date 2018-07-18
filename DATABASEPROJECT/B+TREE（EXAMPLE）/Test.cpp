// B+TREE（EXAMPLE）.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "DataBase.h"
#include <string>
#include <iostream>
#include <time.h>
#include <algorithm>
using namespace std;

double randomInt(int num)
{
	return 1 + rand() % num;
}

char randomCh()
{
	if (rand() % 2 == 0)
	{
		return char(65 + rand() % 26);
	}
	else
	{
		return char(97 + rand() % 26);
	}
}

string randomData(int len)
{
	string s = "";
	int length = randomInt(len);
	char ch;
	for (int i = 0;i < length;i++)
	{
		ch = randomCh();
		s += ch;
	}
	return s;
}

string fixedLengthData(int len)
{
	string s = "";
	char ch;
	for (int i = 0;i < len;i++)
	{
		ch = randomCh();
		s += ch;
	}
	return s;
}

void UI()
{
	string tempStr;
	DataBase db;
	while (true)
	{
		cout << "是否使用默认数据库文件？（Y/N）\n";
		getline(cin, tempStr);
		if (tempStr != "Y"&&tempStr != "N"&&tempStr != "y"&&tempStr != "n")
		{
			cout << "非法输入，请重试。\n";
			continue;
		}
		else if (tempStr == "Y" || tempStr == "y")
		{
			cout << "读取文件中……请稍候\n";
			db.Read();
			cout << "读取完毕。\n";
			break;
		}
		else
		{
			string indexPath;
			string dataPath;
			cout << "请输入索引文件地址：\n";
			getline(cin, indexPath);
			cout << "请输入数据文件地址：\n";
			getline(cin, dataPath);
			db.reOpen(indexPath, dataPath);
			cout << "读取文件中……请稍候\n";
			if (!db.isOpen())
			{
				cout << "输入的文件地址不存在，是否在该地址新建文件？（Y/N）\n";
				getline(cin, tempStr);
				if (tempStr != "Y"&&tempStr != "N"&&tempStr != "y"&&tempStr != "n")
				{
					cout << "非法输入，请重试。\n";
					continue;
				}
				else if (tempStr == "Y" || tempStr == "y")
				{
					db.newOpen(indexPath, dataPath);
					if (!db.isOpen())
					{
						cout << "输入的文件地址不合法，请重试。\n";
						continue;
					}
					db.Read();
					cout << "新建并读取完毕。\n";
					break;
				}
				else continue;
			}
			else
			{
				db.Read();
				cout << "读取完毕。\n";
				break;
			}
		}
	}
	while (true)
	{
		cout << "请选择要进行的操作：\n(查找数据输入1/储存数据输入2/删除数据输入3/修改数据输入4/保存并退出输入5)\n";
		getline(cin, tempStr);
		if (tempStr == "1")
		{
			int id;
			cout << "请输入要查找的id：\n";
			getline(cin, tempStr);
			id = int(atoi(tempStr.c_str()));
			if (getStr(id) != tempStr)
			{
				cout << "输入不合法，id只能为整数，请重试。\n";
				continue;
			}
			db.user_fetch(id);
			continue;
		}
		if (tempStr == "2")
		{
			int id;
			cout << "请输入要储存的id：\n";
			getline(cin, tempStr);
			id = int(atoi(tempStr.c_str()));
			if (getStr(id) != tempStr)
			{
				cout << "输入不合法，id只能为整数，请重试。\n";
				continue;
			}
			cout << "请输入要储存的数据：\n";
			getline(cin, tempStr);
			db.user_store(id, tempStr);
			continue;
		}
		if (tempStr == "3")
		{
			int id;
			cout << "请输入要删除的数据的id：\n";
			getline(cin, tempStr);
			id = int(atoi(tempStr.c_str()));
			if (getStr(id) != tempStr)
			{
				cout << "输入不合法，id只能为整数，请重试。\n";
				continue;
			}
			db.user_remove(id);
			continue;
		}
		if (tempStr == "4")
		{
			int id;
			cout << "请输入要修改的数据的id：\n";
			getline(cin, tempStr);
			id = int(atoi(tempStr.c_str()));
			if (getStr(id) != tempStr)
			{
				cout << "输入不合法，id只能为整数，请重试。\n";
				continue;
			}
			cout << "请输入要修改的数据：\n";
			getline(cin, tempStr);
			db.user_replace(id, tempStr);
			continue;
		}
		if (tempStr == "5")
		{
			cout << "保存中……请勿关闭窗口。\n";
			db.close();
			cout << "保存关闭。\n";
			break;
		}
		else
		{
			cout << "输入不合法，请重试。\n";
			continue;
		}
	}
}

bool functionTest1(int num)
{
	bool pass = true;
	DataBase db;
	for (int i = 1; i <= num; i++)
	{
		string data = "test" + getStr(i);
		db.test_store(i, data);
	}
	for (int i = 1; i <= num; i++)
	{
		string data = "test" + getStr(i);
		if (db.test_fetch(i) != data)pass = false;
	}
	for (int i = num + 1; i <= 2 * num; i++)
	{
		if (db.test_fetch(i) != "")pass = false;
	}
	for (int i = 1; i <= num; i++)
	{
		string data = "test" + getStr(i + 5);
		db.test_replace(i, data);
	}
	for (int i = 1; i <= num; i++)
	{
		string data = "test" + getStr(i + 5);
		if (db.test_fetch(i) != data)pass = false;
	}
	for (int i = 1; i <= num / 2; i++)
	{
		db.test_remove(i);
	}
	for (int i = 1; i <= num / 2; i++)
	{
		if (db.test_fetch(i) != "")pass = false;
	}
	for (int i = num / 2 + 1; i <= num; i++)
	{
		string data = "test" + getStr(i + 5);
		if (db.test_fetch(i) != data)pass = false;
	}
	db.close();
	return pass;
}

bool functionTest2(int num)
{
	bool flag = true;
	DataBase db;
	for (int i = 1;i <= 10000;i++)
	{
		db.test_store(i, "test" + getStr(i));
	}
	for (int i = 1;i <= num;i++)
	{
		db.test_store(i + 10000, "test" + getStr(i));
		if (db.test_fetch(i + 10000) != "test" + getStr(i))flag = false;
		db.test_store(i + 10000, "test");
		if (db.test_fetch(i + 10000) != "test" + getStr(i))flag = false;
		db.test_remove(i + 10000);
		if (db.test_fetch(i + 10000) != "")flag = false;
		db.test_replace(i + 10000, "test");
		if (db.test_fetch(i + 10000) != "")flag = false;
		db.test_remove(i + 10000);
		if (db.test_fetch(i + 10000) != "")flag = false;
		db.test_store(i + 10000, "test" + getStr(i));
		if (db.test_fetch(i + 10000) != "test" + getStr(i))flag = false;
		db.test_replace(i + 10000, "test");
		if (db.test_fetch(i + 10000) != "test")flag = false;
	}
	return flag;
}

bool functionTest3(int num)
{
	bool flag = true;
	DataBase db;
	db.Read();
	for (int i = 1;i <= 5 * num;i++)
	{
		int n = randomInt(num);
		db.test_fetch(n);
		if (i % 37 == 0)
		{
			db.test_remove(n);
		}
		if (i % 11 == 0)
		{
			int newId = i + 1000;
			string newData = randomData(10);
			db.test_store(newId, newData);
			if (db.test_fetch(newId) != newData)flag = false;
		}
		if (i % 17 == 0)
		{
			string oldData = db.test_fetch(n);
			if (i % 2 == 0)
			{
				db.test_replace(n, oldData);
			}
			else
			{
				db.test_replace(n, oldData + "Q");
			}
		}
	}
	db.close();
	return flag;
}

void storeTest()
{
	double dur;
	clock_t start, end1, end2;
	start = clock();
	DataBase db;
	for (int i = 1;i <= 1000000;i++)
	{
		string data = "test" + getStr(i);
		db.test_store(i, data);
	}
	end1 = clock();
	dur = (double)(end1 - start);
	cout << endl << "CycleTime:" << (dur / CLOCKS_PER_SEC) << endl;
	db.close();
	end2 = clock();
	dur = (double)(end2 - end1);
	cout << endl << "CloseTime:" << (dur / CLOCKS_PER_SEC) << endl;
}

void fetchTest()
{
	double dur;
	clock_t start, end1, end2, end3;
	start = clock();
	DataBase db;
	db.Read();
	end3 = clock();
	dur = (double)(end3 - start);
	cout << endl << "ReadTime:" << (dur / CLOCKS_PER_SEC) << endl;
	for (int i = 1;i <= 1000000;i++)
	{
		db.test_fetch(i);
	}
	end1 = clock();
	dur = (double)(end1 - end3);
	cout << endl << "CycleTime:" << (dur / CLOCKS_PER_SEC) << endl;
	db.close();
	end2 = clock();
	dur = (double)(end2 - end1);
	cout << endl << "CloseTime:" << (dur / CLOCKS_PER_SEC) << endl;
}

void replaceTest()
{
	double dur;
	clock_t start, end1, end2, end3;
	start = clock();
	DataBase db;
	db.Read();
	end3 = clock();
	dur = (double)(end3 - start);
	cout << endl << "ReadTime:" << (dur / CLOCKS_PER_SEC) << endl;
	for (int i = 1;i <= 500000;i++)
	{
		db.test_replace(i, "t");
	}
	for (int i = 500001;i <= 1000000;i++)
	{
		db.test_replace(i, "testtest");
	}
	end1 = clock();
	dur = (double)(end1 - end3);
	cout << endl << "CycleTime:" << (dur / CLOCKS_PER_SEC) << endl;
	db.close();
	end2 = clock();
	dur = (double)(end2 - end1);
	cout << endl << "CloseTime:" << (dur / CLOCKS_PER_SEC) << endl;
}

void removeTest()
{
	double dur;
	clock_t start, end1, end2, end3;
	start = clock();
	DataBase db;
	db.Read();
	end3 = clock();
	dur = (double)(end3 - start);
	cout << endl << "ReadTime:" << (dur / CLOCKS_PER_SEC) << endl;
	for (int i = 1;i <= 1000000;i++)
	{
		db.test_remove(i);
	}
	end1 = clock();
	dur = (double)(end1 - end3);
	cout << endl << "CycleTime:" << (dur / CLOCKS_PER_SEC) << endl;
	db.close();
	end2 = clock();
	dur = (double)(end2 - end1);
	cout << endl << "CloseTime:" << (dur / CLOCKS_PER_SEC) << endl;
}

void readTest()
{
	double dur;
	clock_t start, end1, end2;
	start = clock();
	DataBase db;
	db.Read();
	end1 = clock();
	dur = (double)(end1 - start);
	cout << endl << "ReadTime:" << (dur / CLOCKS_PER_SEC) << endl;
	db.close();
	end2 = clock();
	dur = (double)(end2 - end1);
	cout << endl << "CloseTime:" << (dur / CLOCKS_PER_SEC) << endl;
}

void timeTest(int num)
{
	double dur;
	clock_t start, end1, end2, end3, end4, end5;
	start = clock();
	DataBase db;
	for (int i = 1;i <= num;i++)
	{
		string data = "test";
		db.test_store(i, data);
	}
	end1 = clock();
	dur = (double)(end1 - start);
	cout << endl << "StoreTime:" << (dur / CLOCKS_PER_SEC) << endl;
	for (int i = 1;i <= num;i++)
	{
		db.test_fetch(i);
	}
	end2 = clock();
	dur = (double)(end2 - end1);
	cout << endl << "FetchTime:" << (dur / CLOCKS_PER_SEC) << endl;
	for (int i = 1;i <= num;i++)
	{
		if (i >= num / 2)
		{
			db.test_replace(i, "testtest");
		}
		else db.test_replace(i, "t");
	}
	end3 = clock();
	dur = (double)(end3 - end2);
	cout << endl << "ReplaceTime:" << (dur / CLOCKS_PER_SEC) << endl;
	for (int i = 1;i <= num;i++)
	{
		db.test_remove(i);
	}
	end4 = clock();
	dur = (double)(end4 - end3);
	cout << endl << "RemoveTime:" << (dur / CLOCKS_PER_SEC) << endl;
	db.close();
	db.clear();
	end5 = clock();
	dur = (double)(end5 - end4);
	cout << endl << "CloseTime:" << (dur / CLOCKS_PER_SEC) << endl;
}

void fullTimeTest()
{
	for (int i = 100;i <= 1000000;i *= 10)
	{
		cout << "数据量：" << i << endl;
		timeTest(i);
	}
}

void userBehaviorTest(int num)
{
	double dur;
	clock_t start, end1;
	start = clock();
	DataBase db;
	for (int i = 0; i < num; i++)
	{
		db.test_store(i, "test");
		db.test_fetch(i);
	}
	end1 = clock();
	dur = (double)(end1 - start);
	cout << endl << "CycleTime:" << (dur / CLOCKS_PER_SEC) << endl;
	db.close();
	db.clear();
}

void fullUserTest()
{
	for (int i = 100;i <= 1000000;i *= 10)
	{
		cout << "数据量：" << i << endl;
		userBehaviorTest(i);
	}
}

int main()
{
	fullTimeTest();
}