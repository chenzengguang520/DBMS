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
	std::vector<std::string> stringSplit(std::string str);
	void printVector(std::vector<std::string>& v);
public:
	create();
	~create();

	 
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

	
};

bool create::createDatabase(std::string name)
{
	// ƴ�ӵ�ǰ·�����ļ�����
	name = "./dbms/" + name;
	fs::path folderPath = fs::current_path() / name;

	// ����ļ����Ƿ����
	if (fs::exists(folderPath) && fs::is_directory(folderPath))
	{
		std::cout << "Error: Folder '" << name << "' already exists." << std::endl;
		return false;
	}
	else
	{
		// �����ļ���
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

	// ƴ���ļ�·��
	std::string filePath =  dataBasePath + name + ".txt";

	// ���Դ��ļ�
	std::ifstream file(filePath);

	// ����ļ��Ѵ��ڣ����� false
	if (file.is_open()) {
		std::cout << "Error: Folder '" << name << "' already exists." << std::endl;
		file.close();
		return false;
	}

	// �ļ������ڣ������ļ������� true
	std::ofstream newFile(filePath);
	newFile.close();

	writeFile(filePath, m);

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
	//std::string filePath = dataBasePath + name + ".txt"; // �滻Ϊ��ϣ��д�����ݵ��ļ�·��
//	std::cout << filePath << std::endl;
	// ���ļ���
	std::ofstream outputFile(filePath);

	// ����ļ��Ƿ�ɹ���
	if (outputFile.is_open()) {
		for (const auto& cur : m)
		{
			outputFile << cur.first << " " << cur.second << "\n";
		}

		// �ر��ļ���
		outputFile.close();

		std::cout << "�����ѳɹ�д�뵽�ļ���" << filePath << std::endl;
	}
	else {
		std::cerr << "�޷����ļ���" << filePath << std::endl;
	}
}

inline void create::writeFile(std::string filePath, std::vector<std::pair<std::string, std::string>> v,int count)
{
	std::ofstream outputFile(filePath, std::ios::app);

	// ����ļ��Ƿ�ɹ���
	if (outputFile.is_open()) {
		for (const auto& cur : v)
		{
			outputFile << cur.first << " " << cur.second <<" " << count << "\n";
		}

		// �ر��ļ���
		outputFile.close();

		std::cout << "�����ѳɹ�д�뵽�ļ���" << filePath << std::endl;
	}
	else {
		std::cerr << "�޷����ļ���" << filePath << std::endl;
	}

}

inline void create::writeFile(std::string filePath, std::list<std::string>& l)
{
	std::ofstream outputFile(filePath);

	// ����ļ��Ƿ�ɹ���
	if (outputFile.is_open()) {
		for (const auto& cur : l)
		{
			outputFile << cur << "\n";
		}

		// �ر��ļ���
		outputFile.close();

		std::cout << "�����ѳɹ�д�뵽�ļ���" << filePath << std::endl;
	}
	else 
	{
		std::cerr << "�޷����ļ���" << filePath << std::endl;
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

	std::string filePath = "./file/variable.txt"; // �滻Ϊ��ϣ����ȡ���ݵ��ļ�·��

	// ���ļ���
	std::ifstream inputFile(filePath);

	// ����ļ��Ƿ�ɹ���
	if (inputFile.is_open()) 
	{
		// ���ж�ȡ�ļ�����
		std::string line;
		int l = 0;
		while (std::getline(inputFile, line)) 
		{
			variableMap[line] = l++;
			//std::cout << line << " " << l << std::endl;
		}

		// �ر��ļ���
		inputFile.close();
	}
	else 
	{
		std::cerr << "�޷����ļ���" << filePath << std::endl;
	}



	//get variableCount
	filePath = "./file/variableCount.txt"; // �滻Ϊ��ϣ����ȡ���ݵ��ļ�·��

	// ���ļ���
	std::ifstream inputFile1(filePath);

	// ����ļ��Ƿ�ɹ���
	if (inputFile1.is_open())
	{
		// ���ж�ȡ�ļ�����
		std::string line;
		while (std::getline(inputFile1, line))
		{
		//	std::cout << line  << std::endl;
			std::vector<std::string>v = stringSplit(line);
			//std::cout << v[0] << " " << v[1] << std::endl;
			variableCount.insert({ v[0],v[1]});
		}
		// �ر��ļ���
		inputFile1.close();
	}
	else
	{
		std::cerr << "�޷����ļ���" << filePath << std::endl;
	}


	//std::cout << " ---------------- " << std::endl;
}

//[id] = 2
inline void create::insert(std::string name, std::map<std::string, std::string> data)
{
	std::string path1 = dataBasePath + name + ".txt";


	// ���ļ���
	std::ifstream inputFile(path1);

	// ����ļ��Ƿ�ɹ���
	if (inputFile.is_open()) 
	{
		// ���ж�ȡ�ļ�����
		std::string line;
		tableVariable.clear();
		while (std::getline(inputFile, line)) 
		{
			std::cout << line << std::endl;
			std::vector<std::string> words = stringSplit(line);
			tableVariable[words[0]] = words[1];
		}
		// �ر��ļ���
		int count = std::stoi(variableCount[path1]);
		count++;
		variableCount[path1] = std::to_string(count);
		inputFile.close();
	}
	else 
	{
		std::cerr << "�޷����ļ���" << path1 << std::endl;
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
	// ָ��Ŀ¼��·��
	std::string directoryPath = "./dbms";
	std::vector<std::string> v;
	try 
	{
		// ʹ��directory_iterator����Ŀ¼
		for (const auto& entry : fs::directory_iterator(directoryPath)) 
		{
			// ����Ƿ����ļ���
			if (fs::is_directory(entry.path())) 
			{
				// ����ļ�������
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

inline void create::setWhere(std::map<std::pair<std::string, std::string>, int>& m)
{
	this->whereMap = m;
}

inline void create::checkAndDelete(std::list<std::string>& l)
{
	std::list<std::string>deleteList;
	for (const auto& data : l)
	{
		//word[0] Ӧ������������word[1] Ӧ��������ֵ, word[2]Ӧ���ǵڼ�������
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

	// ���ļ���
	std::ifstream inputFile(path1);

	// ����ļ��Ƿ�ɹ���
	if (inputFile.is_open())
	{
		// ���ж�ȡ�ļ�����
		std::string line;
		tableVariable.clear();
		while (std::getline(inputFile, line))
		{
			std::cout << line << std::endl;
			v.push_back(line);
		}
		//std::cout << "******************" << std::endl;
		inputFile.close();
	}
	else
	{
		std::cerr << "�޷����ļ���" << path1 << std::endl;
	}

	checkAndDelete(v);

	writeFile(path1, v);

	variableCount[dataBasePath + name + ".txt"] = std::to_string(v.size());
	std::cout << v.size() << std::endl;

}

inline void create::showData(std::string name)
{
	std::string path1 = "./data/" + name + ".txt";

	std::vector<std::string> v;

	// ���ļ���
	std::ifstream inputFile(path1);

	// ����ļ��Ƿ�ɹ���
	if (inputFile.is_open())
	{
		// ���ж�ȡ�ļ�����
		std::string line;
		tableVariable.clear();
		while (std::getline(inputFile, line))
		{
			std::cout << line << std::endl;
			v.push_back(line);
		}
		//std::cout << "******************" << std::endl;
		inputFile.close();
	}
	else
	{
		std::cerr << "�޷����ļ���" << path1 << std::endl;
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
