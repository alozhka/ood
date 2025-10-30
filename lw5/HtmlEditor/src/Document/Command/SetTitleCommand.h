#pragma once
#include "ICommand.h"

#include <string>

class SetTitleCommand final : public ICommand
{
public:
	SetTitleCommand(std::string& title, const std::string& newTitle)
		: m_currentTitle(title)
		, m_newTitle(newTitle)
	{
	}

	void Execute() override
	{
		m_oldTitle = m_currentTitle;
		m_currentTitle = m_newTitle;
	}

	void Unexecute() override
	{
		m_currentTitle = m_oldTitle;
	}

	bool TryMerge(const ICommand* other) override
	{
		if (auto otherSetTitle = dynamic_cast<const SetTitleCommand*>(other))
		{
			m_newTitle = otherSetTitle->m_newTitle;
			return true;
		}
		return false;
	}

private:
	std::string& m_currentTitle;
	std::string m_oldTitle;
	std::string m_newTitle;
};