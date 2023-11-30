#include<iostream>
#include "createDatabase.h"
using namespace std;

void test()
{
	create* c = new create;
	std::vector<std::string> v = c->getDatabase();
}


int main()
{
	test();
}