#include <iostream>
#include "lexical_analyzer.hpp"
#include "syntax_analyzer.hpp"
#include "message.hpp"

int main(int argc, char** argv)
{
    try {
        const char* outfile = "../data/out.s";
        if (argc < 2 || argc > 3)
            throw new message(std::string("неверные входные данные"));
        if (argc == 3 && argv[2] != nullptr)
            outfile = argv[2];
        syntax_analyzer sa(argv[1], outfile);
        sa.parse();
    } catch(message* msg) {
        new message("операция не завершена");
        return 1;
    }
    new message("операция успешно завершена");
    return 0;
}
