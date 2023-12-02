#include<iostream>
#include "createDatabase.h"
using namespace std;

void test()
{
	create* c = new create();
	c->setDataBasePath("student");
	map<string, string>m;
	m["age"] = "int";
	m["score"] = "string";
	c->createTable("score",m);
	map<string, string>data;
	data["age"] = "5";
	data["score"] = "12";
	c->insert("score", data);
	c->showData("score");
	map<pair<string, string>, int>m1;
	m1.insert({ pair("age","5"),0});
	c->setWhere(m1);
	c->deleteData("score");
 	c->~create();

}


int main()
{
	test();
}