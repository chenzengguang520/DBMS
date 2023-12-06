#pragma once
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
class User
{
private:
	std::map<std::string, std::string> userMap;
	std::vector<std::string> userVector;
private:
	std::vector<std::string> readFile(std::string);
	std::vector<std::string> stringSplit(std::string str);
public:
	User();
	bool verification(std::string account,std::string password);
};

User::User()
{
	std::string filePath = "./account/account.txt";
	userVector = readFile(filePath);
	for (const auto& cur : userVector)
	{
		std::vector<std::string>words = stringSplit(cur);
		userMap[words[0]] = words[1];
	}
}
inline bool User::verification(std::string account, std::string password)
{
	if (userMap.find(account) == userMap.end())
	{
		return false;
	}
	if (userMap[account] != password)
		return false;
	return true;
}
inline std::vector<std::string> User::readFile(std::string filePath)
{

	std::ifstream fileStream(filePath);

	if (!fileStream.is_open()) {
		std::cout << "无法打开文件：" << filePath << std::endl;
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

inline std::vector<std::string> User::stringSplit(std::string str)
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
