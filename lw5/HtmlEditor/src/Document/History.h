#pragma once
#include "Command/ICommand.h"

#include <deque>
#include <memory>

class History
{
public:
	using ICommandPtr = std::unique_ptr<ICommand>;

	void AddAndExecute(ICommandPtr command)
	{
		// Удаляем все команды после текущей позиции (если была отмена)
		if (m_currentCommandIndex < m_commands.size())
		{
			m_commands.erase(m_commands.begin() + m_currentCommandIndex, m_commands.end());
		}

		// Выполняем команду
		command->Execute();

		// Добавляем команду в историю
		m_commands.push_back(std::move(command));
		++m_currentCommandIndex;

		// Ограничиваем размер истории до 10 команд
		constexpr size_t MAX_HISTORY_SIZE = 10;
		if (m_commands.size() > MAX_HISTORY_SIZE)
		{
			m_commands.pop_front();
			--m_currentCommandIndex;
		}
	}

	bool CanUndo() const
	{
		return m_currentCommandIndex > 0;
	}

	void Undo()
	{
		if (!CanUndo())
		{
			throw std::logic_error("Cannot undo");
		}

		--m_currentCommandIndex;
		m_commands[m_currentCommandIndex]->Unexecute();
	}

	bool CanRedo() const
	{
		return m_currentCommandIndex < m_commands.size();
	}

	void Redo()
	{
		if (!CanRedo())
		{
			throw std::logic_error("Cannot redo");
		}

		m_commands[m_currentCommandIndex]->Execute();
		++m_currentCommandIndex;
	}

private:
	std::deque<ICommandPtr> m_commands;
	size_t m_currentCommandIndex = 0;
};