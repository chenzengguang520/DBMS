#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <map>
#include <string>
namespace fs = std::filesystem;

class create
{
private:
	std::string dataBasePath;
	std::map<std::string, int> variableMap;
	std::map<std::string, std::string> tableVariable;
	std::map<std::pair<std::string, std::string>, int>whereMap; // 0: == ,1: > ,2 < ,3 <= , 4 >= ,5 != ;
	std::vector<std::vector<std::string>>data;
	std::map<std::string, std::string> variableCount;

private:
	void printVector(std::vector<std::string>& v);
	std::vector<std::string> stringSplit(std::string str);
public:
	create();
	~create();
	friend std::vector<std::string> stringSplitPlus(create& e,std::string str);

	std::vector<std::string> selectData(std::string, std::vector<std::string>&);//select column1,from table;

	std::vector<std::string> getWhereData(std::vector<std::string>& v);

	//update set
	void updateData(std::string, std::map<std::string, std::string>& m);
	 
	void setWhere(std::map<std::pair<std::string, std::string>, int>&);

	void checkAndDelete(std::list<std::string>&);

	void deleteData(std::string name);//delete table which name is _name

	void showData(std::string name);// get name table data

	void insert(std::string name,std::map<std::string, std::string>);


	//create database
	std::vector<std::string> getDatabase();
	//get database
	bool createDatabase(std::string name);

	//create table 
	bool createTable(std::string name,std::map<std::string,std::string> m);
	// use database
	void setDataBasePath(std::string path);
	// 
	std::vector<std::string> getTable();

	void writeFile(std::string,std::map<std::string, std::string>m);
	void writeFile(std::string filePath, std::vector<std::pair<std::string, std::string>> v,int count);

	void writeFile(std::string, std::list<std::string>& l);
	void writeFile(std::string, std::vector<std::string>& v);

	
};

bool create::createDatabase(std::string name)
{
	// 拼接当前路径和文件夹名
	name = "./dbms/" + name;
	fs::path folderPath = fs::current_path() / name;

	// 检查文件夹是否存在
	if (fs::exists(folderPath) && fs::is_directory(folderPath))
	{
		std::cout << "Error: Folder '" << name << "' already exists." << std::endl;
		return false;
	}
	else
	{
		// 创建文件夹
		if (fs::create_directory(folderPath))
		{
			std::cout << "Folder '" << name << "' created successfully." << std::endl;
			return true;
		}
		else
		{
			std::cerr << "Error: Unable to create folder '" << name << "'." << std::endl;
			return false;
		}
	}
}

inline bool create::createTable(std::string name,std::map<std::string,std::string>m)
{

	// 拼接文件路径
	std::string filePath =  dataBasePath + name + ".txt";

	// 尝试打开文件
	std::ifstream file(filePath);

	// 如果文件已存在，返回 false
	if (file.is_open()) {
		std::cout << "Error: Folder '" << name << "' already exists." << std::endl;
		file.close();
		return false;
	}

	// 文件不存在，创建文件并返回 true
	std::ofstream newFile(filePath);
	newFile.close();

	writeFile(filePath, m);
	//filePath = "./file/variableCount.txt";
	variableCount[filePath] = "0";
	writeFile(filePath, variableCount);

	return true;
}

inline void create::setDataBasePath(std::string path)
{
	dataBasePath = "./dbms/" + path + "/";

}

inline std::vector<std::string> create::getTable()
{
	std::vector<std::string> v;
	//std::cout << dataBasePath << std::endl;
	try {
		for (const auto& entry : fs::directory_iterator(dataBasePath)) {
			std::cout << entry.path().filename() << std::endl;
			v.push_back(entry.path().filename().string());
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
	return v;
}

inline void create::writeFile(std::string filePath,std::map<std::string, std::string> m)
{
	//std::string filePath = dataBasePath + name + ".txt"; // 替换为你希望写入内容的文件路径
//	std::cout << filePath << std::endl;
	// 打开文件流
	std::ofstream outputFile(filePath);

	// 检查文件是否成功打开
	if (outputFile.is_open()) {
		for (const auto& cur : m)
		{
			outputFile << cur.first << " " << cur.second << "\n";
		}

		// 关闭文件流
		outputFile.close();

		std::cout << "内容已成功写入到文件：" << filePath << std::endl;
	}
	else {
		std::cerr << "无法打开文件：" << filePath << std::endl;
	}
}

inline void create::writeFile(std::string filePath, std::vector<std::pair<std::string, std::string>> v,int count)
{
	std::ofstream outputFile(filePath, std::ios::app);

	// 检查文件是否成功打开
	if (outputFile.is_open()) {
		for (const auto& cur : v)
		{
			outputFile << cur.first << " " << cur.second <<" " << count << "\n";
		}

		// 关闭文件流
		outputFile.close();

		std::cout << "内容已成功写入到文件：" << filePath << std::endl;
	}
	else {
		std::cerr << "无法打开文件：" << filePath << std::endl;
	}

}

inline void create::writeFile(std::string filePath, std::list<std::string>& l)
{
	std::ofstream outputFile(filePath);

	// 检查文件是否成功打开
	if (outputFile.is_open()) {
		for (const auto& cur : l)
		{
			outputFile << cur << "\n";
		}

		// 关闭文件流
		outputFile.close();

		std::cout << "内容已成功写入到文件：" << filePath << std::endl;
	}
	else 
	{
		std::cerr << "无法打开文件：" << filePath << std::endl;
	}
}

inline void create::writeFile(std::string filePath, std::vector<std::string>& l)
{

	std::ofstream outputFile(filePath);

	// 检查文件是否成功打开
	if (outputFile.is_open()) {
		for (const auto& cur : l)
		{
			outputFile << cur << "\n";
		}

		// 关闭文件流
		outputFile.close();

		std::cout << "内容已成功写入到文件：" << filePath << std::endl;
	}
	else
	{
		std::cerr << "无法打开文件：" << filePath << std::endl;
	}

}


inline std::vector<std::string> create::stringSplit(std::string str)
{
	std::vector<std::string> v;
	int i = 0;
	int j = 0;
	while (j < str.size())
	{
		if (str[j] == ' ')
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

inline create::create()
{

	std::string filePath = "./file/variable.txt"; // 替换为你希望读取内容的文件路径

	// 打开文件流
	std::ifstream inputFile(filePath);

	// 检查文件是否成功打开
	if (inputFile.is_open()) 
	{
		// 逐行读取文件内容
		std::string line;
		int l = 0;
		while (std::getline(inputFile, line)) 
		{
			variableMap[line] = l++;
			//std::cout << line << " " << l << std::endl;
		}

		// 关闭文件流
		inputFile.close();
	}
	else 
	{
		std::cerr << "无法打开文件：" << filePath << std::endl;
	}



	//get variableCount
	filePath = "./file/variableCount.txt"; // 替换为你希望读取内容的文件路径

	// 打开文件流
	std::ifstream inputFile1(filePath);

	// 检查文件是否成功打开
	if (inputFile1.is_open())
	{
		// 逐行读取文件内容
		std::string line;
		while (std::getline(inputFile1, line))
		{
			std::vector<std::string>v = stringSplit(line);
			variableCount.insert({ v[0],v[1]});
		}
		// 关闭文件流
		inputFile1.close();
	}
	else
	{
		std::cerr << "无法打开文件：" << filePath << std::endl;
	}
}

//[id] = 2
inline void create::insert(std::string name, std::map<std::string, std::string> data)
{
	std::string path1 = dataBasePath + name + ".txt";


	// 打开文件流
	std::ifstream inputFile(path1);

	// 检查文件是否成功打开
	if (inputFile.is_open()) 
	{
		// 逐行读取文件内容
		std::string line;
		tableVariable.clear();
		while (std::getline(inputFile, line)) 
		{
			std::vector<std::string> words = stringSplit(line);
			tableVariable[words[0]] = words[1];
			//std::cout << "words[1] = " << words[1] << std::endl;
		}
		//std::cout << "-----------------" << std::endl;
		// 关闭文件流
		int count = std::stoi(variableCount[path1]);
		count++;
		variableCount[path1] = std::to_string(count);
		inputFile.close();
	}
	else 
	{
		std::cerr << "无法打开文件：" << path1 << std::endl;
	}
	std::vector<std::pair<std::string,std::string>>v;
	for (const auto cur : data)
	{
		std::string variable = cur.second;
		v.push_back(std::pair(cur.first, variable));
	}
	std::string path2 = "./data/" + name + ".txt";
	writeFile(path2, v,std::stoi(variableCount[path1]));

}

inline void create::printVector(std::vector<std::string>& v)
{
	for (const auto& cur : v)
	{
		std::cout << cur << std::endl;
	}

}

std::vector<std::string> create::getDatabase()
{
	// 指定目录的路径
	std::string directoryPath = "./dbms";
	std::vector<std::string> v;
	try 
	{
		// 使用directory_iterator遍历目录
		for (const auto& entry : fs::directory_iterator(directoryPath)) 
		{
			// 检查是否是文件夹
			if (fs::is_directory(entry.path())) 
			{
				// 输出文件夹名称
				std::cout << entry.path().filename().string() << std::endl;
				v.push_back(entry.path().filename().string());
			}
		}
	}
	catch (const std::filesystem::filesystem_error& ex) 
	{
		std::cerr << "Error accessing directory: " << ex.what() << std::endl;
	}
	return v;
}

inline create::~create()
{

	std::string filePath = "./file/variableCount.txt";
	writeFile(filePath, variableCount);

}

inline std::vector<std::string> create::selectData(std::string name, std::vector<std::string>& variableName)
{
	std::string path1 = "./data/" + name + ".txt";

	std::vector<std::string> v;

	// 打开文件流
	std::ifstream inputFile(path1);

	// 检查文件是否成功打开
	if (inputFile.is_open())
	{
		// 逐行读取文件内容
		std::string line;
		tableVariable.clear();
		while (std::getline(inputFile, line))
		{
			v.push_back(line);
		}
		//std::cout << "******************" << std::endl;
		inputFile.close();
	}
	else
	{
		std::cerr << "无法打开文件：" << path1 << std::endl;
	}

	std::vector<std::string> need = getWhereData(v);
	std::vector<std::string>needData;
	for (const auto& cur : need)
	{
		std::vector<std::string> words1 = stringSplit(cur);
		for (const auto& data : v)
		{
			std::vector<std::string>words2 = stringSplit(data);
			if (words1[2] == words2[2])
			{
				for (const auto& variablename : variableName)
				{
					if (words2[0] == variablename)
					{
						needData.push_back(data);
					}
				}
			}
		}
	}
	return needData;
}

inline std::vector<std::string> create::getWhereData(std::vector<std::string>& v)
{

	std::vector<std::string>deleteList;
	for (const auto& data : v)
	{
		//word[0] 应该是数据名，word[1] 应该是数据值, word[2]应该是第几个数据
		std::vector<std::string>word = stringSplit(data);
		int count = 0;
		int destion = whereMap.size();
		for (const auto& cur : whereMap)
		{
			std::string variableName = cur.first.first;
			std::string variableValue = cur.first.second;
			int condition = cur.second;// 0: == , 1 : > , 2 < , 3 <= , 4 >= , 5 !=
			if (word[0] == variableName)
			{
				switch (condition)
				{
				case 0:
					if (word[1] == variableValue)
					{
						count++;
					}
					break;
				case 1:
					if (word[1] > variableValue)
					{
						count++;
					}
					break;
				case 2:
					if (word[1] < variableValue)
					{
						count++;
					}
					break;
				case 3:
					if (word[1] <= variableValue)
					{
						count++;
					}
					break;
				case 4:
					if (word[1] >= variableValue)
					{
						count++;
					}
					break;
				case 5:
					if (word[1] != variableValue)
					{
						count++;
					}
					break;
				default:
					break;
				}
			}
		}
		if (count == destion)
			deleteList.push_back(data);
	}

	if (deleteList.empty())
		return v;

	return deleteList;
}

inline void create::updateData(std::string name, std::map<std::string, std::string>& m)
{
	std::string path1 = "./data/" + name + ".txt";

	std::vector<std::string> v;

	// 打开文件流
	std::ifstream inputFile(path1);

	// 检查文件是否成功打开
	if (inputFile.is_open())
	{
		// 逐行读取文件内容
		std::string line;
		tableVariable.clear();
		while (std::getline(inputFile, line))
		{
			v.push_back(line);
		}
		inputFile.close();
	}
	else
	{
		std::cerr << "无法打开文件：" << path1 << std::endl;
	}

	std::vector<std::string> needUpdate = getWhereData(v);

	std::vector<std::string> allNeedUpdate;

	for (const auto& cur : needUpdate)
	{
		std::vector<std::string> words1 = stringSplit(cur);
		for (auto& data : v)
		{
			std::vector<std::string> words2 = stringSplit(data);
			if (words2[2] == words1[2])
			{
				if (m.find(words2[0]) != m.end())
				{
					words2[1] = m[words2[0]];
					data = words2[0] + " " + words2[1] + " " + words2[2];
				}
			}
		}
	}
	writeFile(path1, v);
}

inline void create::setWhere(std::map<std::pair<std::string, std::string>, int>& m)
{
	this->whereMap = m;
}

inline void create::checkAndDelete(std::list<std::string>& l)
{
	std::list<std::string>deleteList;
	for (const auto& data : l)
	{
		//word[0] 应该是数据名，word[1] 应该是数据值, word[2]应该是第几个数据
		std::vector<std::string>word = stringSplit(data);
		int count = 0;
		int destion = whereMap.size();
		for (const auto& cur : whereMap)
		{
			std::string variableName = cur.first.first;
			std::string variableValue = cur.first.second;
			int condition = cur.second;// 0: == , 1 : > , 2 < , 3 <= , 4 >= , 5 !=
			if (word[0] == variableName)
			{
				switch (condition)
				{
				case 0:
					if (word[1] == variableValue)
					{
						count++;
					}
					break;
				case 1:
					if (word[1] > variableValue)
					{
						count++;
					}
					break;
				case 2:
					if (word[1] < variableValue)
					{
						count++;
					}
					break;
				case 3:
					if (word[1] <= variableValue)
					{
						count++;
					}
					break;
				case 4:
					if (word[1] >= variableValue)
					{
						count++;
					}
					break;
				case 5:
					if (word[1] != variableValue)
					{
						count++;
					}
					break;
				default:
					break;
				}
			}
		}
		if (count == destion)
			deleteList.push_back(data);
	}
	std::list<std::string>allDeleteList;
	if (deleteList.empty())
		l.clear();
	else
	{
		for (const auto& data : deleteList)
		{
			/*l.remove(data);*/
			std::vector<std::string>words1 = stringSplit(data);
			for (const auto& d : l)
			{
				std::vector<std::string>words2 = stringSplit(d);
				if (words2[2] == words1[2])
				{
					allDeleteList.push_back(d);
				}
			}
		}
		for (const auto& data : allDeleteList)
			l.remove(data);

	}
}

inline void create::deleteData(std::string name)
{
	std::string path1 = "./data/" + name + ".txt";

	std::list<std::string> v;

	// 打开文件流
	std::ifstream inputFile(path1);

	// 检查文件是否成功打开
	if (inputFile.is_open())
	{
		// 逐行读取文件内容
		std::string line;
		tableVariable.clear();
		while (std::getline(inputFile, line))
		{
			v.push_back(line);
		}
		//std::cout << "******************" << std::endl;
		inputFile.close();
	}
	else
	{
		std::cerr << "无法打开文件：" << path1 << std::endl;
	}

	checkAndDelete(v);

	writeFile(path1, v);

	std::cout << "dataBasePath + name + \".txt\" = " << dataBasePath + name + ".txt" << std::endl;
	variableCount[dataBasePath + name + ".txt"] = std::to_string(v.size());
	std::cout << v.size() << std::endl;

}

inline void create::showData(std::string name)
{
	std::string path1 = "./data/" + name + ".txt";

	std::vector<std::string> v;

	// 打开文件流
	std::ifstream inputFile(path1);

	// 检查文件是否成功打开
	if (inputFile.is_open())
	{
		// 逐行读取文件内容
		std::string line;
		tableVariable.clear();
		while (std::getline(inputFile, line))
		{
			v.push_back(line);
		}
		//std::cout << "******************" << std::endl;
		inputFile.close();
	}
	else
	{
		std::cerr << "无法打开文件：" << path1 << std::endl;
	}
	//data.resize(100);
	std::vector<std::string> variable1;
	
	for (int i = 0; i < std::stoi(variableCount[dataBasePath + name + ".txt"]);i++)
	{
		for (const auto& var : v)
		{
			//std::cout << "var = " << var << std::endl;
			std::vector<std::string> words = stringSplit(var);
			std::string variableName = words[0];
			std::string variableValue = words[1];
			int lineCount = std::stoi(words[2]);
			if (lineCount == i + 1)
			{
				variable1.push_back(variableValue);
			}
		}
		data.push_back(variable1);
		variable1.clear();
	}
	//std::cout << "*************" << std::endl;
	for (const auto& cur : data)
	{
		for (const auto& var : cur)
		{
			std::cout << var << " ";
		}
		std::cout << "\n";
	}

}


std::vector<std::string> stringSplitPlus(create& e,std::string str)
{
	return e.stringSplit(str);
}