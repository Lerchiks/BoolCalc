#include "boolexpr.h"
#include <iostream>
#include <fstream>

void HELP();

int main(int argc, char*argv[]) {
    
    if (argc != 4 &&  argc != 2) {
        std::cout <<"Wrong number of arguments";
       return 0;
    }
    
    std::string work = argv[1];
    if (argc == 2 && (work=="-h"||work=="?")) {
        HELP();
        return 0;
    }

    std::string input = argv[2];
    std::string output = argv[3];

    std::ifstream f_input(input);
    std::ofstream f_output(output);

    if (!f_input||!f_output) {
        std::cout << "Can't open files";
        return 0;
    }

    std::string now_f;

    if (work == "-cnf") {
        while (std::getline(f_input, now_f)) {
            try {
                f_output << BooleanExpression(now_f.c_str()).cnf() << '\n';
            } catch (const std::runtime_error&) {
                f_output << "error";
                return 0;
            }
        }
    }

    if (work == "-dnf") {
        while (std::getline(f_input, now_f)) {
            try {
                f_output << BooleanExpression(now_f.c_str()).dnf() << '\n';
            } catch (const std::runtime_error&) {
                f_output << "error";
                return 0;
            }
        }
    }

    if (work == "-zh") {
        while (std::getline(f_input, now_f)) {
            try {
                f_output << BooleanExpression(now_f.c_str()).zhegalkin() << '\n';

            } catch (const std::runtime_error&) {
                f_output << "error";
                return 0;
            }
        }
    }

    if (work == "-table") {
        while (std::getline(f_input, now_f)) {
            try {
                f_output << BooleanExpression(now_f.c_str()).make_table() << '\n';

            }
            catch (const std::runtime_error&) {
                f_output << "error";
                return 0;
            }
        }
    }
    int i = 0;

    if (work == "-isfull") {
        std::vector<BooleanExpression> system;


        while (std::getline(f_input, now_f)) {
            try {
                system.emplace_back(now_f.c_str());
               
            } catch (const std::runtime_error&) {
                f_output << "error";
                return 0;
            }
        }
        try {
          
            f_output << ( IsFullSystem(system) ? "yes" : "no" );
        } catch (const std::runtime_error&) {
            f_output << "error";
            return 0;
        }

    }
    return 0;
}

void HELP() {
    std::cout << "\tDescription of the Boolean expression calculator:\n";
    std::cout << "--------------------------------------------------------------------------------\n";
    std::cout << "-cnf: \n";
    std::cout << "\tConstructs a conjunctive normal form for expressions written in the input file and writes the results to the output file\n";
    std::cout << "--------------------------------------------------------------------------------\n";
    std::cout << "-dnf: \n";
    std::cout << "\tBuilds a disjunctive normal form for expressions written in the input file and writes the results to the output file\n";
    std::cout << "--------------------------------------------------------------------------------\n";
    std::cout << "-zh: \n";
    std::cout << "\tbuilds the Zhegalkin polynomial for expressions written in the input file and writes the results to the output file\n";
    std::cout << "--------------------------------------------------------------------------------\n";
    std::cout << "-isfull: \n";
    std::cout << "\tChecks the system of functions for completeness.\n";
    std::cout << "--------------------------------------------------------------------------------\n";

}
