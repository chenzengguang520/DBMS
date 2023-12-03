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
public:
	Analyse();
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

	std::cout << "code = " << code << std::endl;
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


}