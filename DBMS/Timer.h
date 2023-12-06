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
	// �ļ�·��
	std::string filePath = "./log/log.txt";

	// ���ļ�����������Ϊ��׷��ģʽ��
	std::ofstream fileStream(filePath, std::ios::app);

	if (!fileStream.is_open()) 
	{
		std::cerr << "�޷����ļ���" << filePath << std::endl;
		return;
	}
	// Ҫ׷�ӵ�����
	int pos = code.find("recovery");
	if (!(pos >= 0 && pos < code.size()))
	{
		std::string dataToAppend = getTime() + " " + code + "\n";
		// ���ļ���д������
		fileStream << dataToAppend;
	}

	// �ر��ļ���
	fileStream.close();
}

inline std::vector<std::string> Timer::readFile(std::string filePath)
{

	std::ifstream fileStream(filePath);

	if (!fileStream.is_open()) {
		std::cerr << "�޷����ļ���" << filePath << std::endl;
	}

	std::vector<std::string> fileLines;
	std::string line;

	while (std::getline(fileStream, line))
	{
		fileLines.push_back(line);
	}
	// �ر��ļ���
	fileStream.close();
	return fileLines;
}

inline Timer::Timer()
{
	analyse = new Analyse();
}

std::string Timer::getTime()
{
	// ��ȡ��ǰʱ���
	auto currentTimePoint = std::chrono::system_clock::now();

	// ��ʱ���ת��Ϊʱ��ṹ��
	std::time_t currentTime = std::chrono::system_clock::to_time_t(currentTimePoint);

	// ʹ�� localtime_s ������ʱ��ṹ��ת��Ϊ����ʱ��
	std::tm localTimeInfo;
	localtime_s(&localTimeInfo, &currentTime);

	// �����ʽ����ʱ��
	char dateBuffer[11];  // ����Ϊ 11 ���� null ��ֹ��
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
