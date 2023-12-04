#pragma once
#include<string>
#include<vector>
#include<iostream>
struct Table 
{
	std::string name;
	std::vector<std::string> columns;
	std::vector<std::vector<std::string>> data;
};
class Tabler {
private:
	std::vector<Table> tables;

public:
	// 添加表
	void addTable(const Table& table);

	// 显示表格数据
	void showTableData(const std::string& tableName);
	void init();


};


void Tabler::addTable(const Table& table)
{
	tables.push_back(table);
}

void Tabler::showTableData(const std::string& tableName)
{
	std::cout << "size = " << tables.size() << std::endl;
	for (const auto& table : tables) {
		if (table.name == tableName) {
			// 打印表头
			for (const auto& column : table.columns) {
				std::cout << column << "\t";
			}
			std::cout << std::endl;

			// 打印数据
			for (const auto& row : table.data) {
				for (const auto& cell : row) {
					std::cout << cell << "\t";
				}
				std::cout << std::endl;
			}
			return;
		}
	}

	std::cout << "Table not found: " << tableName << std::endl;
}

inline void Tabler::init()
{
	tables.clear();

}
