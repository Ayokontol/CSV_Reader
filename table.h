#pragma once

#include <vector>
#include <map>
#include <stdexcept>
#include <fstream>
#include <string>
#include <iostream>

class table {
    std::vector<std::vector<int>> Table;
    std::vector<std::vector<std::string>> Table_string;
    std::map<std::string, int> LineNum, ColumnName;
    std::vector<std::string> AllNames, AllNums;
    size_t W, H;

    const int INF = 10000000;

    std::vector<std::string> split_eq_line(std::string& eq_line);

    bool check_eq_line(std::string& eq_line);

    bool check_other_line(std::vector<std::string>& line, size_t num);

    bool check_first_line(std::vector<std::string>& line);

    bool check_symbol(char c);

    std::vector<std::string> split_by_comma(std::string& line);

public:
    table(std::string& file_name);
private:
    bool try_calculate();
public:
    void calculate();

    void output();
};
