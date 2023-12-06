#pragma once
#include "createDatabase.h"
#include "Table.h"
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
class Analyse
{
private:
	std::vector<std::string>codes;
	Tabler* tabler = nullptr;
	Table* table = nullptr;
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
	std::vector<std::string>tables;
	std::vector<std::string>databases;
	std::set<std::string>columnName;
	std::set<std::string>tableName;
	std::string date;
private:
	void writeFile(std::string code);
	std::vector<std::string>readFile(std::string filePath);
	std::vector<std::string> stringSplit(std::string str);
	std::vector<std::string> stringSplit(std::string str,std::string );
	bool getWhereMap(std::string);
	void getColumnName();
public:
	Analyse();
	~Analyse();
	friend std::vector<std::string> stringSplitBlank(Analyse* e,std::string);
	void grammarAnalyse(std::string);
	void createTableData();
	void insertData();
	void deleteData();
	void updateData();
	void selectData();
	void showTable();	
	void recoveryData();

	std::string getTime();
	void logRecord(std::string code);
	void logRecovery(std::string date);
};

Analyse::Analyse()
{
	engineer= new create();
	tabler = new Tabler();
	table = new Table();

}

inline Analyse::~Analyse()
{
	engineer->~create();
}

inline std::vector<std::string>stringSplitBlank(Analyse* e,std::string str)
{

	return stringSplitPlus(*(e->engineer),str);
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
		for (const auto& cur : engineer->getTable())
		{
			int pos = cur.find(".txt");
			tableName.insert(std::string(cur.begin(), cur.begin() + pos));
		}
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
	if (words[0] == "show")
	{
		flage = true;
		showTable();
	}
	if (words[0] == "recovery")
	{
		flage = true;
		recoveryData();
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
	getColumnName();
	for (int i = 0; i < values.size(); i++)
	{
		if (columnName.find(stringSplitPlus(*engineer,columns[i])[0]) == columnName.end())
		{
			std::cout << "SQL Error ! " << "Error code : " << columns[i] << std::endl;
			return;
		}
		insertMap.insert({ columns[i],values[i] });
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
	if (!(pos1 > 0 && pos2 > pos1 && pos2 < code.size()))
	{
		std::cout << "SQL Error need ()!" << std::endl;
		return;
	}

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

inline void Analyse::getColumnName()
{
	for (const auto& cur : engineer->getColumn(name))
	{
		columnName.insert(stringSplitPlus(*engineer, cur)[0]);
	}
}
void Analyse::deleteData()
{
	try 
	{
		if (words.at(1) != "from")
		{
			std::cout << "SQL Error: " << words[1] << " SQL error code: " << code << std::endl;
			return;
		}
	}
	catch (const std::out_of_range& e) 
	{
		std::cerr << "Caught an out_of_range exception: " << e.what() << std::endl;
		return;
	}

	try 
	{
		name = words.at(2);
	}
	catch (const std::out_of_range& e) 
	{
		std::cerr << "Caught an out_of_range exception: " << e.what() << std::endl;
		return;
	}
	int posWhere = code.find("where");
	if (posWhere > 0 && posWhere < code.size())
	{
		int pos1 = code.find("{");
		int pos2 = code.find("}");
		if (pos1 > 0 && pos2 > pos1 && pos2 < code.size())
		{
			std::vector<std::string>variables = stringSplit(std::string(code.begin() + pos1 + 1, code.begin() + pos2), "and");
			for (const auto& cur : variables)
			{
				std::vector<std::string>variable1 = stringSplitPlus(*engineer,cur);
				if (variable1[1] == "=")
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
			int pos = code.find("where");
			bool flag = false;
			if(pos > 0 && pos < code.size())
			{
				flag = true;
			}
			updateMap[words.at(3)] = words.at(5);
			int pos1 = code.find("{");
			int pos2 = code.find("}");
			if (flag && pos1 > 0 && pos1 < pos2 && pos2 < code.size())
			{
				if (getWhereMap(std::string(code.begin() + pos1 + 1,code.begin() + pos2)))
				{
					engineer->setWhere(whereMap);
					engineer->updateData(name,updateMap);
					whereMap.clear();
					engineer->setWhere(whereMap);
					return;
				}
				else
				{
					std::cout<<"SQL Error!"<<"Where code Error!"<<" Error code : "<<code<<std::endl;
					return;
				}
			}
			else
			{
				if (flag)
				{
					std::cout << "SQL Error!" << "need {} " << "Error code : " << code << std::endl;
					return;
				}
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
		for (const auto& cur : engineer->getColumn(name))
		{
			columnName.insert(stringSplitPlus(*engineer,cur)[0]);
		}
		int pos1 = code.find(words.at(1));
		int pos2 = pos1 + words.at(1).size() + 1;
		selectVariable = stringSplit(std::string(code.begin() + pos1, code.begin() + pos2));
		if (selectVariable.size() == 1 && stringSplitPlus(*engineer, selectVariable[0])[0] == "*")
		{
			std::set<std::string>s1;
			for (const auto& cur : engineer->getColumn(name))
			{
				s1.insert(stringSplitPlus(*engineer, cur)[0]);
			}
			std::vector<std::string>v1;
			for (const auto& cur : s1)
			{
				v1.push_back(cur);
			}
			selectVariable = v1;
		}
		for (const auto& cur : selectVariable)
		{
			if (columnName.find(stringSplitPlus(*engineer,cur)[0]) == columnName.end())
			{
				std::cout << "SQL Error ! " << "Not Found " << cur << std::endl;
				return;
			}
		}
		int pos5 = code.find("where");
		bool flag = false;
		if(pos5 >= 0 && pos5 <= code.size())
		{
			flag = true;
		}
		int pos3 = code.find("{");
		int pos4 = code.find("}");
		if (!(pos3 >= 0 && pos3 < pos4 && pos4 <= code.size()) && flag)
		{
			std::cout << "SQL Error !" << "need {}" << std::endl;
			return;
		}
		if (flag && getWhereMap(std::string(code.begin() + pos3 + 1,code.begin() + pos4)))
		{
			engineer->setWhere(whereMap);
		}
		else
		{
			if (flag)
			{
				std::cout << "SQL Error !" << "Where Error !" << "Error code : " << code << std::endl;
				return;
			}
		}
		selectVariableData = engineer->selectData(name, selectVariable);
		std::map<std::string, std::vector<std::string>> m;
		for (const auto& cur : selectVariableData)
		{
			std::vector<std::string>words11 = stringSplitPlus(*engineer, cur);
			m[words11[2]].push_back(words11[0] + " " + words11[1]);
		}
		for (auto& cur : m)
		{
			for (int i = 0; i < cur.second.size() - 1; i++)
			{
				for (int j = i + 1; j < cur.second.size(); j++)
				{
					std::string str1(code.begin() + pos1, code.begin() + pos2);
					if (str1.find(stringSplitPlus(*engineer, cur.second[i])[0]) > str1.find(stringSplitPlus(*engineer, cur.second[j])[0]))
					{
						std::string str = cur.second[i];
						cur.second[i] = cur.second[j];
						cur.second[j] = str;
					}
				}
			}
		}
		std::vector<std::vector<std::string>>needData;
		for (const auto& cur : m)
		{
			std::vector<std::string>v2;
			for (const auto& data : cur.second)
			{
				std::vector<std::string>v1 = stringSplitPlus(*engineer, data);
				v2.push_back(v1[1]);
			}
			needData.push_back(v2);
		}
		table->name = name;
		table->columns = selectVariable;
		table->data = needData;
		tabler->addTable(*table);
		tabler->showTableData(name);
		tabler->init();
	}
	catch (const std::out_of_range& e) 
	{
		std::cerr << "Caught an out_of_range exception: " << e.what() << std::endl;
		return;
	}

}

inline void Analyse::showTable()
{
	try
	{
		if (words.at(1) == "table")
		{
			tables = engineer->getTable();
			for (const auto& cur : tables)
			{
				int posdian = cur.find(".txt");
				std::string tableName(cur.begin(), cur.begin() + posdian);
				std::cout << tableName << std::endl;
			}
		}
		else if (words.at(1) == "database")
		{
			databases = engineer->getDatabase();
			for (const auto& cur : databases)
			{
				std::cout << cur << std::endl;
			}
		}
		else if (tableName.find(stringSplitPlus(*engineer,words.at(1))[0]) != tableName.end())
		{
			for (const auto& n : engineer->getColumn(stringSplitPlus(*engineer, words.at(1))[0]))
			{
				std::cout << n << std::endl;
				//columnName.insert(stringSplitPlus(*engineer, n)[0]);
			}
		}
		else
		{
			std::cout << "SQL Error ! " << "can't recognition " << words.at(1) << "Error code : " << code << std::endl;
		}
	}
	catch (const std::out_of_range& e)
	{
		std::cerr << "Caught an out_of_range exception: " << e.what() << std::endl;
		return;
	}
}



inline void Analyse::writeFile(std::string code)
{
	// 文件路径
	std::string filePath = "./log/log.txt";

	// 打开文件流，并设置为以追加模式打开
	std::ofstream fileStream(filePath, std::ios::app);

	if (!fileStream.is_open())
	{
		std::cerr << "无法打开文件：" << filePath << std::endl;
		return;
	}
	// 要追加的数据
	int pos = code.find("recovery");
	std::cout << "pos = " << pos << std::endl;
	if (!(pos >= 0 && pos < code.size()))
	{
		std::string dataToAppend = getTime() + " " + code + "\n";
		// 向文件中写入数据
		fileStream << dataToAppend;
	}


	// 关闭文件流
	fileStream.close();
}

inline std::vector<std::string> Analyse::readFile(std::string filePath)
{

	std::ifstream fileStream(filePath);

	if (!fileStream.is_open()) {
		std::cerr << "无法打开文件：" << filePath << std::endl;
	}

	std::vector<std::string> fileLines;
	std::string line;

	while (std::getline(fileStream, line))
	{
		fileLines.push_back(line);
	}
	// 关闭文件流
	fileStream.close();
	return fileLines;
}

std::string Analyse::getTime()
{
	// 获取当前时间点
	auto currentTimePoint = std::chrono::system_clock::now();

	// 将时间点转换为时间结构体
	std::time_t currentTime = std::chrono::system_clock::to_time_t(currentTimePoint);

	// 使用 localtime_s 函数将时间结构体转换为本地时间
	std::tm localTimeInfo;
	localtime_s(&localTimeInfo, &currentTime);

	// 输出格式化的时间
	char dateBuffer[11];  // 长度为 11 包括 null 终止符
	std::strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", &localTimeInfo);
	std::string currentDate(dateBuffer);
	return currentDate;
}

inline void Analyse::logRecord(std::string code)
{
	writeFile(code);
}

inline void Analyse::logRecovery(std::string date)
{
	//data = 2023-12-06
	std::string filePath = "./log/log.txt";
	std::vector<std::string>logdata = readFile(filePath);

	for (const auto& data : logdata)
	{
		std::string codeDate = stringSplitPlus(*engineer, data)[0];
		if (codeDate == date)
		{
			std::string code(data.begin() + stringSplitPlus(*engineer, data)[0].size() + 1, data.end());
			codes.push_back(code);
		}
		//std::cout << "code = " << code << std::endl;
	}
	for (const auto& code : codes)
	{
		grammarAnalyse(code);
	}
}

void Analyse::recoveryData()
{
	try
	{
		date = words.at(1);
		logRecovery(date);
	}
	catch (const std::out_of_range& e)
	{
		std::cerr << "Caught an out_of_range exception: " << e.what() << std::endl;
		return;
	}
}