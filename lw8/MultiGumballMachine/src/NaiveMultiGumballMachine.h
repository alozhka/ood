#pragma once

#include <format>
#include <iostream>

class NaiveMultiGumballMachine
{
public:
	static constexpr unsigned MAX_QUARTERS = 5;

	explicit NaiveMultiGumballMachine(unsigned numBalls, std::ostream& out)
		: m_count(numBalls)
		, m_quarters(0)
		, m_output(out)
		, m_state(State::SOLD_OUT)
	{
		if (m_count > 0)
		{
			m_state = State::NO_QUARTER;
		}
	}

	void InsertQuarter()
	{
		switch (m_state)
		{
		case State::SOLD_OUT:
			m_output << "You can't insert a quarter, the machine is sold out\n";
			break;
		case State::NO_QUARTER:
			if (m_quarters < MAX_QUARTERS)
			{
				m_output << "You inserted a quarter\n";
				++m_quarters;
				m_state = State::HAS_QUARTER;
			}
			else
			{
				m_output << "You can't insert another quarter, the machine is full\n";
			}
			break;
		case State::HAS_QUARTER:
			if (m_quarters < MAX_QUARTERS)
			{
				m_output << "You inserted another quarter\n";
				++m_quarters;
			}
			else
			{
				m_output << "You can't insert another quarter, the machine is full\n";
			}
			break;
		case State::SOLD:
			m_output << "Please wait, we're already giving you a gumball\n";
			break;
		}
	}

	void EjectQuarter()
	{
		switch (m_state)
		{
		case State::SOLD_OUT:
			if (m_quarters > 0)
			{
				m_output << "Returning " << m_quarters << " quarter" << (m_quarters != 1 ? "s" : "") << "\n";
				m_quarters = 0;
			}
			else
			{
				m_output << "You can't eject, you haven't inserted a quarter yet\n";
			}
			break;
		case State::NO_QUARTER:
			m_output << "You haven't inserted a quarter\n";
			break;
		case State::HAS_QUARTER:
			m_output << "Returning " << m_quarters << " quarter" << (m_quarters != 1 ? "s" : "") << "\n";
			m_quarters = 0;
			m_state = State::NO_QUARTER;
			break;
		case State::SOLD:
			m_output << "Sorry you already turned the crank\n";
			break;
		}
	}

	void TurnCrank()
	{
		switch (m_state)
		{
		case State::SOLD_OUT:
			m_output << "You turned but there's no gumballs\n";
			break;
		case State::NO_QUARTER:
			m_output << "You turned but there's no quarter\n";
			break;
		case State::HAS_QUARTER:
			m_output << "You turned...\n";
			m_state = State::SOLD;
			break;
		case State::SOLD:
			m_output << "Turning twice doesn't get you another gumball\n";
			break;
		}
		Dispense();
	}

	std::string ToString() const
	{
		std::string stateString;
		switch (m_state)
		{
		case State::SOLD_OUT:
			stateString = "sold out";
			break;
		case State::NO_QUARTER:
			stateString = "waiting for quarter";
			break;
		case State::HAS_QUARTER:
			stateString = "waiting for turn of crank";
			break;
		case State::SOLD:
			stateString = "delivering a gumball";
			break;
		}

		return std::format("Mighty Gumball, Inc.\n"
						   "C++-enabled Standing Gumball Model #2016 (Multi-Quarter)\n"
						   "Inventory: {} gumball{}\n"
						   "Quarters inserted: {}/{}\n"
						   "Machine is {}",
			m_count,
			m_count != 1 ? "s" : "",
			m_quarters,
			MAX_QUARTERS,
			stateString);
	}

private:
	enum class State
	{
		SOLD_OUT,
		NO_QUARTER,
		HAS_QUARTER,
		SOLD
	};

	void Dispense()
	{
		switch (m_state)
		{
		case State::SOLD:
			if (m_count > 0)
			{
				m_output << "A gumball comes rolling out the slot...\n";
				--m_count;
				--m_quarters;

				if (m_count == 0)
				{
					m_output << "Oops, out of gumballs\n";
					m_state = State::SOLD_OUT;
				}
				else if (m_quarters == 0)
				{
					m_state = State::NO_QUARTER;
				}
				else
				{
					m_state = State::HAS_QUARTER;
				}
			}
			else
			{
				m_output << "No gumball dispensed\n";
				m_state = State::SOLD_OUT;
			}
			break;
		case State::SOLD_OUT:
			m_output << "No gumball dispensed\n";
			break;
		case State::NO_QUARTER:
			m_output << "You need to pay first\n";
			break;
		case State::HAS_QUARTER:
			m_output << "No gumball dispensed\n";
			break;
		}
	}

	unsigned m_count = 0;
	unsigned m_quarters = 0;
	State m_state;
	std::ostream& m_output;
};