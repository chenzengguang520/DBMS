#pragma once
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;


class create
{
public:

	std::vector<std::string> getDatabase();

	bool createDatabase(std::string name);
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


std::vector<std::string> create::getDatabase()
{
	// 指定目录的路径
	std::string directoryPath = "./dbms";
	std::vector<std::string> v;
	try {
		// 使用directory_iterator遍历目录
		for (const auto& entry : fs::directory_iterator(directoryPath)) {
			// 检查是否是文件夹
			if (fs::is_directory(entry.path())) {
				// 输出文件夹名称
				std::cout << entry.path().filename().string() << std::endl;
				v.push_back(entry.path().filename().string());
			}
		}
	}
	catch (const std::filesystem::filesystem_error& ex) {
		std::cerr << "Error accessing directory: " << ex.what() << std::endl;
	}
	return v;

}