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


std::vector<std::string> create::getDatabase()
{
	// ָ��Ŀ¼��·��
	std::string directoryPath = "./dbms";
	std::vector<std::string> v;
	try {
		// ʹ��directory_iterator����Ŀ¼
		for (const auto& entry : fs::directory_iterator(directoryPath)) {
			// ����Ƿ����ļ���
			if (fs::is_directory(entry.path())) {
				// ����ļ�������
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