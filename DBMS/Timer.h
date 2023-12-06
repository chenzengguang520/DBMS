#pragma once
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include "grammarAnalyse.h"
class Timer
{
private:
	Analyse* analyse = nullptr;
	std::vector<std::string>codes;
private:
	void writeFile(std::string code);
	std::vector<std::string>readFile(std::string filePath);
public:
	Timer();
	std::string getTime();
	void logRecord(std::string code);
	void logRecovery(std::string date);

};

inline void Timer::writeFile(std::string code)
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
	if (!(pos >= 0 && pos < code.size()))
	{
		std::string dataToAppend = getTime() + " " + code + "\n";
		// 向文件中写入数据
		fileStream << dataToAppend;
	}

	// 关闭文件流
	fileStream.close();
}

inline std::vector<std::string> Timer::readFile(std::string filePath)
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

inline Timer::Timer()
{
	analyse = new Analyse();
}

std::string Timer::getTime()
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

inline void Timer::logRecord(std::string code)
{
	writeFile(code);
}

inline void Timer::logRecovery(std::string date)
{
	//data = 2023-12-06
	std::string filePath = "./log/log.txt";
	std::vector<std::string>logdata = readFile(filePath);
	
	for (const auto& data : logdata)
	{
		std::string codeDate = stringSplitBlank(analyse, data)[0];
		if (codeDate == date)
		{
			std::string code(data.begin() + stringSplitBlank(analyse, data)[0].size() + 1, data.end());
			codes.push_back(code);
		}
		//std::cout << "code = " << code << std::endl;
	}
	for (const auto& code : codes)
	{
		analyse->grammarAnalyse(code);
	}
}
