#pragma once
#include "Command/ICommand.h"

#include <deque>

class History
{
public:
	using ICommandPtr = std::unique_ptr<ICommand>;

private:
	std::deque<ICommandPtr> m_commands;
	size_t m_currentCommandIndex = 0;
};