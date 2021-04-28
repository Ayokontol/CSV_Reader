#include "table.h"

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << "Input file name.\n";
        return 0;
    }
    std::string file_name = argv[1];
    try {
        table Table(file_name);

        Table.calculate();

        Table.output();
    }
    catch (std::exception& excp) {
        std::cout << excp.what();
        return 0;
    }
}