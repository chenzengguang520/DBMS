#pragma once
#include "createDatabase.h"
class Analyse
{

private:
	create* engineer = nullptr;
	std::string code;
	std::vector<std::string>words;
	std::string name;
	std::vector<std::string>columns;
	std::vector<std::string>values;
	std::map<std::string, std::string>insertMap;
	std::map<std::string, std::string> columnsName;//存储表含有的变量的
	std::map<std::pair<std::string,std::string>,int>deleteWhere;

private:
	std::vector<std::string> stringSplit(std::string str);
	std::vector<std::string> stringSplit(std::string str,std::string );
public:
	Analyse();
	~Analyse();
	void grammarAnalyse(std::string);
	void createTableData();
	void insertData();
	void deleteData();
	void updateData();
	void selectData();
	
};

Analyse::Analyse()
{
	engineer= new create();

}

inline Analyse::~Analyse()
{
	engineer->~create();
}

inline void Analyse::grammarAnalyse(std::string _code)
{
	code = _code;
	words = stringSplitPlus(*engineer, _code);
	bool flage = false;
	if (words[0] == "create")
	{
		flage = true;
		try
		{
			if (words.at(1) == "database")
			{
				engineer->createDatabase(words.at(2));
			}
			if (words.at(1) == "table")
			{
				name = words[2];
				createTableData();
			}
		}
		catch (const std::out_of_range& e) 
		{
			std::cerr << "Caught an out_of_range exception: " << e.what() << std::endl;
			return;
		}
	}
	if (words[0] == "use")
	{
		flage = true;
		if (words.size() != 2)
		{
			std::cout << " SQL error code: " << _code << std::endl;
		}
		engineer->setDataBasePath(words[1]);
	}
	if (words[0] == "insert")
	{
		flage = true;
		insertData();
	}
	if (words[0] == "delete")
	{
		flage = true;
		deleteData();
	}
	if (words[0] == "update")
	{
		flage = true;

	}
	if (words[0] == "select")
	{
		flage = true;

	}
	if (!flage)
	{
		std::cout << "SQL Error: " << words[0] << " SQL error code: " << _code << std::endl;
	}
}

inline void Analyse::insertData()
{
	try {
		if (words.at(1) != "into")
		{
			std::cout << "SQL Error: " << words[1] << " SQL error code: " << code << std::endl;
			return;
		}
	}
	catch (const std::out_of_range& e) {
		std::cerr << "Caught an out_of_range exception: " << e.what() << std::endl;
		return;
	}

	try {
		name = words.at(2);
	}
	catch (const std::out_of_range& e) {
		std::cerr << "Caught an out_of_range exception: " << e.what() << std::endl;
		return;
	}

	try {
		if (words.at(3) != "column")
		{
			std::cout << "SQL Error: " << words[3] << " SQL error code: " << code << std::endl;
			return;
		}
	}
	catch (const std::out_of_range& e) {
		std::cerr << "Caught an out_of_range exception: " << e.what() << std::endl;
		return;
	}
	int index = 4;
	std::string word;
	try {
		word = words.at(index);
	}
	catch (const std::out_of_range& e) {
		std::cerr << "Caught an out_of_range exception: " << e.what() << std::endl;
		return;
	}
	while (word != ":" && index < words.size())
	{
		columns.push_back(word);
		try {
			word = words.at(++index);
		}
		catch (const std::out_of_range& e) {
			std::cerr << "Caught an out_of_range exception: " << e.what() << std::endl;
			return;
		}
	}
	if (index == words.size() - 1)
	{
		std::cout << "need value" << std::endl;
		return;
	}
	while (++index < words.size())
	{
		values.push_back(words.at(index));
	}
	if (values.size() != columns.size())
	{
		std::cout << "values number != column number!" << std::endl;
		return;
	}
	for (int i = 0; i < values.size(); i++)
	{
		insertMap.insert({ columns[i],values[i] });
	}
	std::cout << "name = " << name << std::endl;
	for (const auto& cur : insertMap)
	{
		std::cout << "cur.first = " << cur.first << " " << "cur.second = " << cur.second << std::endl;
	}
	engineer->insert(name, insertMap);

	return;
}

void Analyse::createTableData()
{
	try {
		name = words.at(2);
	}
	catch (const std::out_of_range& e) {
		std::cerr << "Caught an out_of_range exception: " << e.what() << std::endl;
		return;
	}
	int pos1 = code.find("(");
	int pos2 = code.find(")");
	if (!((pos1 > 0 && pos1 < pos2) && (pos2 > pos1 && pos2 < code.size())))
	{
		std::cout << "SQL Error need ()!" << std::endl;
		return;
	}
	//需要写一个新的stringSplit按，拆分。获取括号内的内容
	//create table score (id int,score int)

	std::string afterCode = std::string(code.begin() + pos1 + 1, code.begin() + pos2);

	std::vector<std::string> variables = stringSplit(afterCode);
	//variables[0] = id int variables[1] = score int


	
	for (auto& cur : variables)
	{
		std::vector<std::string> strs = stringSplitPlus(*engineer, cur);
		columnsName[strs[0]] = strs[1];
	}
	engineer->createTable(name, columnsName);
}

std::vector<std::string> Analyse::stringSplit(std::string str)
{
	std::vector<std::string> v;
	int i = 0;
	int j = 0;
	while (j < str.size())
	{
		if (str[j] == ',')
		{
			std::string word(str.begin() + i, str.begin() + j);
			v.push_back(word);
			i = j + 1;
		}
		j++;
	}
	std::string word(str.begin() + i, str.begin() + j);
	v.push_back(word);
	return v;
}

std::vector<std::string> Analyse::stringSplit(std::string str, std::string split)
{
	std::vector<std::string> v;
	int pos = str.find(split);
	int j = 0, i = 0;
	while (pos < str.size())
	{
		std::string word(str.begin() + i, str.begin() + pos);
		pos = str.find(split);
		i = pos + split.size();
		v.push_back(word);
	}
	std::string word(str.begin() + i, str.end());
	v.push_back(word);
	return v;
}

void Analyse::deleteData()
{
	try {
		if (words.at(1) != "from")
		{
			std::cout << "SQL Error: " << words[1] << " SQL error code: " << code << std::endl;
			return;
		}
	}
	catch (const std::out_of_range& e) {
		std::cerr << "Caught an out_of_range exception: " << e.what() << std::endl;
		return;
	}

	try {
		name = words.at(2);
	}
	catch (const std::out_of_range& e) {
		std::cerr << "Caught an out_of_range exception: " << e.what() << std::endl;
		return;
	}
	int posWhere = code.find("where");
	//std::cout << "posWhere = " << posWhere << std::endl;
	if (posWhere > 0 && posWhere < code.size())
	{
		int pos1 = code.find("{");
		int pos2 = code.find("}");
		if (pos1 > 0 && pos2 > pos1 && pos2 < code.size())
		{
			std::vector<std::string>variables = stringSplit(std::string(code.begin() + pos1 + 1, code.begin() + pos2), "and");
			//variables[0] : id = 2
			for (const auto& cur : variables)
			{
				std::cout << "cur = " << cur << std::endl;
				std::vector<std::string>variable1 = stringSplitPlus(*engineer,cur);
				std::cout << "variable1[1] = " << variable1[1] << std::endl;
				if (variable1[1] == "==")
				{
					deleteWhere.insert({ std::pair(variable1[0],variable1[2]),0 });
				}
				if (variable1[1] == ">")
				{
					deleteWhere.insert({ std::pair(variable1[0],variable1[2]),1 });
				}
				if (variable1[1] == "<")
				{
					deleteWhere.insert({ std::pair(variable1[0],variable1[2]),2 });
				}
				if (variable1[1] == "<=")
				{
					deleteWhere.insert({ std::pair(variable1[0],variable1[2]),3 });
				}
				if (variable1[1] == ">=")
				{
					deleteWhere.insert({ std::pair(variable1[0],variable1[2]),4 });
				}
				if (variable1[1] == "!=")
				{
					deleteWhere.insert({ std::pair(variable1[0],variable1[2]),5 });
				}
			}
			engineer->setWhere(deleteWhere);
			engineer->deleteData(name);
		}
		else
		{
			std::cout << "{} Error!" << std::endl;
		}
	}
	else
	{
		engineer->deleteData(name);
	}
}