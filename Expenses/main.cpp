#include "ExpenseSheet.h"
#include "InputArguments.h"

#include <string>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <filesystem>

void Complain(bool repl, std::string msg)
{
    if (repl)
    {
        std::cout << 0 << std::endl;
    }
    else
    {
        std::cout << msg << std::endl;
    }
}

void Success(bool repl)
{
    if (repl)
    {
        std::cout << 1 << std::endl;
    }
}

int main(int argc, char** argv)
{
    ExpenseSheet expenses;

    // REPL Mode
    bool repl = false;

    // Check if file shall be opend
    for (int i = 1; i < argc; ++i)
    {
        if (argv[i][0] != '-')
        {
            std::filesystem::path path = argv[1];
            expenses.Open(path);
        }
        else
        {
            std::string sarg = &argv[i][1];
            if (sarg == "repl")
            {
                repl = true;
            }
        }
    }

    // Command loop
    std::string line;
    while (true)
    {
        // Getting user input
        if(!repl)
            std::cout << "> ";
        std::getline(std::cin, line);
        InputArguments inArgs(line);

        std::string cmd = inArgs[0];
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), [](char c) { return std::tolower(c); });
        auto args = inArgs.Subset(1);

        // Check the commd
        if (cmd == "add")
        {
            if (args.Count() == 2 || args.Count() == 3)
            {
                const auto& label = args[0];
                double value;
                if (args.Count() == 2)
                {
                    value = atof(args[1].c_str()) * -1.;
                }
                else
                {
                    value = atof(args[2].c_str());
                    if (args[1] != "+")
                    {
                        value *= -1.;
                    }
                }

                if (!expenses.Add(label, value))
                {
                    Complain(repl, "Failed to add!");
                }
                else
                {
                    Success(repl);
                }
            }
            else
            {
                Complain(repl, "Usage: add <label> (+/-) <value>");
            }
        }
        else if (cmd == "del")
        {
            if (args.Count() == 1)
            {
                if (!expenses.Del(args[0]))
                {
                    Complain(repl, "Failed to delete!");
                }
                else
                {
                    Success(repl);
                }
            }
            else
            {
                Complain(repl, "Usage: del <label>");
            }
        }
        else if (cmd == "list")
        {
            expenses.List(std::cout, repl);
        }
        else if (cmd == "eval")
        {
            auto value = expenses.Eval();
            if (repl)
                std::cout << value << std::endl;
            else
                std::cout << "TOTAL: " << value << std::endl;
        }
        else if (cmd == "exit")
        {
            Success(repl);
            break;
        }
        else if (cmd == "open")
        {
            if (args.Count() == 1)
            {
                std::filesystem::path p = args[0];
                if (!expenses.Open(p))
                {
                    Complain(repl, "Failed to open file!");
                }
                else
                {
                    Success(repl);
                }
            }
            else
            {
                Complain(repl, "Usage: open <path>");
            }
        }
        else if (cmd == "save")
        {
            if (args.Count() == 1)
            {
                std::filesystem::path p = args[0];
                if (!expenses.Save(p))
                {
                    Complain(repl, "Failed to save file!");
                }
                else
                {
                    Success(repl);
                }
            }
            else if (args.Count() == 0)
            {
                if (!expenses.Save())
                {
                    Complain(repl, "Failed to save file! Try: save <path>");
                }
                else
                {
                    Success(repl);
                }
            }
            else
            {
                Complain(repl, "Usage: save <path>");
            }
        }
        else if (cmd == "new")
        {
            expenses.New();
            Success(repl);
        }
        else if (cmd == "export")
        {
            if (args.Count() == 2)
            {
                if (args[0] == "csv")
                {
                    expenses.ExportCsv(args[1]);
                    Success(repl);
                }
                else if (args[0] == "html")
                {
                    expenses.ExportHtml(args[1]);
                    Success(repl);
                }
                else
                {
                    Complain(repl, "Usage: export <csv/html> <file>");
                }
            }
            else
            {
                Complain(repl, "Usage: export <csv/html> <file>");
            }
        }
        else
        {
            Complain(repl, "Command is unknown!");
        }
    }
}
