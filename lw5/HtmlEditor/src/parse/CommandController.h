#pragma once
#include <istream>

class CommandController
{
public:
    explicit CommandController(std::istream& input) : m_input(input)
    {
    }

    void HandleCommand()
    {
        std::string command;
        m_input >> command;
    }

private:
    std::istream& m_input;
    std::unordered_map<std::string, Command> m_commands;
};
