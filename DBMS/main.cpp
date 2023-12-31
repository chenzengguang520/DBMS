﻿#include<iostream>
#include "grammarAnalyse.h"
#include "Timer.h"
#include "User.h"
using namespace std;

void test()
{
	create* c = new create();
	c->setDataBasePath("student");
	map<string, string>m;
	m["age"] = "int";
	m["score"] = "string";
	c->createTable("score", m);
	map<string, string>data;
	data["age"] = "12";
	data["score"] = "92";
	c->insert("score", data);
	c->showData("score");
	map<pair<string, string>, int>m1;
	m1.insert({ pair("age","5"),0 });
	c->setWhere(m1);
	c->deleteData("score");

	map<pair<string, string>, int>m2;
	m2.insert({ pair("age","9"),0 });
	map<string, string>m3;
	m3.insert({ "score","100" });
	c->setWhere(m2);
	c->updateData("score", m3);

	map<pair<string, string>, int>m4;
	m4.insert({ pair("age","9"),0 });
	c->setWhere(m4);
	vector<string>v = { "age" ,"score" };
	vector<string>data1 = c->selectData("score", v);
	for (const auto& cur : data1)
	{
		cout << cur << endl;
	}

	c->~create();

}


void analyseTest()
{
	Analyse* analyser = new Analyse();
	Timer* timer = new Timer();
	User* user = new User();
	string account, password;
	cout << "please input account :";
	getline(std::cin, account);
	cout << "please input password :";
	getline(std::cin, password);
	while(!user->verification(account, password))
	{
		cout << "account ot password Error !" << endl;
		cout << "please input account :";
		getline(std::cin, account);
		cout << "please input password :";
		getline(std::cin, password);
	}
	while (1)
	{
		cout << "cyw DBMS SQL>> ";
		string code;
		getline(std::cin, code);
		if (code == "quit")
			break;
		if (code == "clear")
		{
			system("cls");
			continue;
		}
		analyser->grammarAnalyse(code);
		timer->logRecord(code);
		
	}
	analyser->~Analyse();
	timer->~Timer();
}

void timeTest()
{
	Timer* timer = new Timer();
	std::string date = "2023-12-06";
	timer->logRecovery(date);
}

int main()
{
	analyseTest();
}