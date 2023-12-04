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
	std::map<std::string, std::string>updateMap;
	std::map<std::pair<std::string, std::string>, int>whereMap;
	std::vector<std::string> selectVariable;
	std::vector<std::string> selectVariableData;

private:
	std::vector<std::string> stringSplit(std::string str);
	std::vector<std::string> stringSplit(std::string str,std::string );
	bool getWhereMap(std::string);
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
	words.clear();
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
		updateData();
	}
	if (words[0] == "select")
	{
		flage = true;
		selectData();
	}
	if (!flage)
	{
		std::cout << "SQL Error: " << words[0] << " SQL error code: " << _code << std::endl;
	}
}

inline void Analyse::insertData()
{
	insertMap.clear();
	values.clear();
	columns.clear();
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

inline bool Analyse::getWhereMap(std::string whereCode)
{
	//id = 1 and id = 2
	whereMap.clear();
	std::vector<std::string>v = stringSplit(whereCode,"and");
	for (const auto& cur : v)
	{
		//id = 1
		bool flag = false;
		std::vector<std::string>variable1 = stringSplitPlus(*engineer, cur);
		if (variable1[1] == "=")
		{
			flag = true;
			whereMap.insert({ std::pair(variable1[0],variable1[2]),0 });
		}
		if (variable1[1] == ">")
		{
			flag = true;
			whereMap.insert({ std::pair(variable1[0],variable1[2]),1 });
		}
		if (variable1[1] == "<")
		{
			flag = true;
			whereMap.insert({ std::pair(variable1[0],variable1[2]),2 });
		}
		if (variable1[1] == "<=")
		{
			flag = true;
			whereMap.insert({ std::pair(variable1[0],variable1[2]),3 });
		}
		if (variable1[1] == ">=")
		{
			flag = true;
			whereMap.insert({ std::pair(variable1[0],variable1[2]),4 });
		}
		if (variable1[1] == "!=")
		{
			flag = true;
			whereMap.insert({ std::pair(variable1[0],variable1[2]),5 });
		}
		if (!flag)
		{
			whereMap.clear();
			return false;
		}
	}
	return true;
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

inline void Analyse::updateData()
{
	try {
		name = words.at(1);
		if (words.at(2) == "set")
		{
			if (words.at(4) != "=")
			{
				std::cout<< "SQL Error " << words.at(4) << "CODE Error! " << code << std::endl;
				return;
			}
			updateMap[words.at(3)] = words.at(5);
			int pos1 = code.find("{");
			int pos2 = code.find("}");
			for (const auto& cur : updateMap)
			{
				std::cout << "cur.first = " << cur.first << " " << "cur.seconde = " << cur.second << std::endl;
			}
			if (pos1 > 0 && pos1 < pos2 && pos2 < code.size())
			{
				if (getWhereMap(std::string(code.begin() + pos1 + 1,code.begin() + pos2)))
				{
					engineer->setWhere(whereMap);
					engineer->updateData(name,updateMap);
				}
				else
				{
					std::cout<<"SQL Error!"<<"Where code Error!"<<" Error code : "<<code<<std::endl;
					return;
				}
			}
			else
			{
				std::cout << "SQL Error!" << "need {} " << "Error code : " << code << std::endl;
			}
			engineer->updateData(name, updateMap);
		}
		else
		{
			std::cout << "SQL Error " << words.at(2) << "CODE Error! " << code << std::endl;
			return;
		}
	}
	catch (const std::out_of_range& e) {
		std::cerr << "Caught an out_of_range exception: " << e.what() << std::endl;
		return;
	}
}

inline void Analyse::selectData()
{
	try 
	{
		name = words.at(3);
		int pos1 = code.find(words.at(1));
		int pos2 = pos1 + words.at(1).size() + 1;
		selectVariable = stringSplit(std::string(code.begin() + pos1, code.begin() + pos2));
		int pos3 = code.find("{");
		int pos4 = code.find("}");
		if (!(pos3 >= 0 && pos3 < pos4 && pos4 <= code.size()))
		{
			std::cout << "SQL Error !" << "need {}" << std::endl;
			return;
		}
		if (getWhereMap(std::string(code.begin() + pos3 + 1,code.begin() + pos4)))
		{
			engineer->setWhere(whereMap);
			selectVariableData = engineer->selectData(name, selectVariable);
			for (const auto& cur : selectVariableData)
			{
				std::cout<<"cur = "<<cur<<std::endl;
			}
		}
		else
		{
			std::cout << "SQL Error !" << "Where Error !" << "Error code : " << code << std::endl;
			return;
		}
	}
	catch (const std::out_of_range& e) 
	{
		std::cerr << "Caught an out_of_range exception: " << e.what() << std::endl;
		return;
	}

}
