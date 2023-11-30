#include<iostream>
#include "createDatabase.h"
using namespace std;

void test()
{
	create* c = new create;
	c->createDatabase("student");
	std::vector<std::string> v = c->getDatabase();
	c->createDatabase("student");
}


int main()
{
	test();
}