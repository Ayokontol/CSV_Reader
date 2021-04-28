#include "table.h"

std::vector<std::string> table::split_eq_line(std::string& eq_line) {
    size_t pos = 1;
    std::string name1 = "", name2 = "", num1 = "", num2 = "", op = "";
    while (pos < eq_line.size() && !(eq_line[pos] >= '0' && eq_line[pos] <= '9'))
        name1 += eq_line[pos], pos++;
    while (pos < eq_line.size() && (eq_line[pos] >= '0' && eq_line[pos] <= '9'))
        num1 += eq_line[pos], pos++;
    op = eq_line[pos];
    pos++;
    while (pos < eq_line.size() && !(eq_line[pos] >= '0' && eq_line[pos] <= '9'))
        name2 += eq_line[pos], pos++;
    while (pos < eq_line.size() && (eq_line[pos] >= '0' && eq_line[pos] <= '9'))
        num2 += eq_line[pos], pos++;
    if (ColumnName[name1] == 0 || ColumnName[name2] == 0 || LineNum[num1] == 0 || LineNum[num2] == 0)
        throw std::invalid_argument("Incorrect equality.\n");
    return { name1, num1, op, name2, num2 };
}

bool table::check_eq_line(std::string& eq_line) {
    size_t pos = 1;
    std::string name1 = "", name2 = "", num1 = "", num2 = "";

    while (pos < eq_line.size() && !(eq_line[pos] >= '0' && eq_line[pos] <= '9'))
        name1 += eq_line[pos], pos++;
    while (pos < eq_line.size() && (eq_line[pos] >= '0' && eq_line[pos] <= '9'))
        num1 += eq_line[pos], pos++;
    if (eq_line[pos] != '*' && eq_line[pos] != '/' && eq_line[pos] != '+' && eq_line[pos] != '-')
        return false;
    pos++;
    while (pos < eq_line.size() && !(eq_line[pos] >= '0' && eq_line[pos] <= '9'))
        name2 += eq_line[pos], pos++;
    while (pos < eq_line.size() && (eq_line[pos] >= '0' && eq_line[pos] <= '9'))
        num2 += eq_line[pos], pos++;
    if (ColumnName[name1] == 0 || ColumnName[name2] == 0)
        return false;
    return true;
}

bool table::check_other_line(std::vector<std::string>& line, size_t num) {
    if (line.size() != W)
        return false;

    Table_string.push_back(std::vector<std::string>(W));

    // add line name
    if (line[0].size() == 0)
        return false;
    for (auto c : line[0])
        if (!(c >= '0' && c <= '9'))
            return false;
    if (LineNum[line[0]] > 0)
        return false;
    LineNum[line[0]] = num;
    AllNums.push_back(line[0]);

    // add to table & check eq lines
    for (size_t i = 1; i < line.size(); ++i) {
        if (line[i][0] == '=') {
            if (!(check_eq_line(line[i])))
                return false;
            Table_string[num][i] = line[i];
        }
        else {
            if (line[i].size() == 0)
                return false;
            for (auto c : line[i]) {
                if (line[i][0] == '-' && line[i].size() > 1)
                    continue;
                else if (c >= '0' && c <= '9')
                    continue;
                else
                    return false;
            }
            Table_string[num][i] = line[i];
        }
    }
    return true;
}

bool table::check_first_line(std::vector<std::string>& line) {
    if (line.size() <= 1)
        return false;
    if (line[0].size() > 0)
        return false;
    for (size_t i = 1; i < line.size(); ++i) {
        // check name
        if (line[i].size() == 0)
            return false;
        for (auto c : line[i])
            if (!(c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z'))
                return false;
        // add line name
        if (ColumnName[line[i]] > 0)
            return false;
        ColumnName[line[i]] = i;
        AllNames.push_back(line[i]);
    }
    Table_string.push_back(std::vector<std::string>());
    return true;
}

bool table::check_symbol(char c) {
    if (c >= '0' && c <= '9' ||
        c >= 'A' && c <= 'Z' ||
        c >= 'a' && c <= 'z' ||
        c == ',' || c == ' ' || c == '=' ||
        c == '+' || c == '-' || c == '*' ||
        c == '/')
        return true;
    return false;
}

std::vector<std::string> table::split_by_comma(std::string& line) {
    std::vector<std::string> Split_line(1, "");

    for (auto c : line) {
        if (!check_symbol(c))
            throw  std::invalid_argument("Incorrect symbol.\n");
        if (c == ',' || c == ' ') {
            Split_line.push_back("");
            continue;
        }
        Split_line[Split_line.size() - 1] += c;
    }
    return Split_line;
}

table::table(std::string& file_name) {
    std::ifstream ifs(file_name);
    
    if (!ifs.is_open())
        throw std::invalid_argument("Incorrect file name.\n");
    
    //read first line
    std::string line;
    
    ifs >> line;
    std::vector<std::string> first_line;
    try {
        first_line = split_by_comma(line);
    }
    catch (...) {
        throw;
    }
    
    if (!check_first_line(first_line))
        throw std::invalid_argument("Incorrect first line.\n");
    W = first_line.size();
    
    // read other line
    size_t num = 1;
    while (ifs >> line) {
        std::vector<std::string> split_line;
        try {
            split_line = split_by_comma(line);
        }
        catch (...) {
            throw;
        }
    
        if (!check_other_line(split_line, num))
            throw std::invalid_argument("Incorrect lines.\n");
        num++;
    }
    if (num <= 1)
        throw std::invalid_argument("No lines.\n");
    H = num;
    Table.resize(H, std::vector<int>(W, INF));
}

bool table::try_calculate() {
    bool is_all = true;
    for (size_t i = 1; i < H; ++i)
        for (size_t j = 1; j < W; ++j) {
            if (Table_string[i][j][0] == '=' && Table[i][j] == INF) {
                std::vector<std::string> split_line;
                try {
                    split_line = split_eq_line(Table_string[i][j]);
                }
                catch (...) {
                    throw;
                }
                size_t col1 = ColumnName[split_line[0]];
                size_t num1 = LineNum[split_line[1]];
                char op = split_line[2][0];
                size_t col2 = ColumnName[split_line[3]];
                size_t num2 = LineNum[split_line[4]];

                if (Table[num1][col1] == INF || Table[num2][col2] == INF) {
                    is_all = false;
                    continue;
                }

                switch (op) {
                case '+':
                    Table[i][j] = Table[num1][col1] + Table[num2][col2];
                    break;
                case '-':
                    Table[i][j] = Table[num1][col1] - Table[num2][col2];
                    break;
                case '*':
                    Table[i][j] = Table[num1][col1] * Table[num2][col2];
                    break;
                case '/':
                    if (Table[num2][col2] == 0)
                        throw std::invalid_argument("Division by zero.\n");
                    Table[i][j] = Table[num1][col1] / Table[num2][col2];
                    break;
                }
            }
            else if (Table[i][j] == INF)
                Table[i][j] = std::stoi(Table_string[i][j]);
        }
    return is_all;
}

void table::calculate() {
    for (size_t i = 0; i < W * H; ++i) {
        bool is_all;
        try {
            is_all = try_calculate();
        }
        catch (...) {
            throw;
        }
        if (is_all)
            return;
    }
    throw std::invalid_argument("Have incorrect equality.\n");
}

void table::output() {
    for (auto name : AllNames)
        std::cout << "," << name;
    std::cout << "\n";
    for (size_t i = 1; i < H; ++i) {
        std::cout << AllNums[i - 1];
        for (size_t j = 1; j < W; ++j)
            std::cout << "," << Table[i][j];
        std::cout << "\n";
    }
}