#pragma once

#include <format>
#include <iostream>

struct IState
{
	virtual void InsertQuarter() = 0;
	virtual void EjectQuarter() = 0;
	virtual void TurnCrank() = 0;
	virtual void Dispense() = 0;
	virtual std::string ToString() const = 0;
	virtual ~IState() = default;
};

struct IGumballMachine
{
	virtual void ReleaseBall() = 0;
	virtual unsigned GetBallCount() const = 0;

	virtual void SetSoldOutState() = 0;
	virtual void SetNoQuarterState() = 0;
	virtual void SetSoldState() = 0;
	virtual void SetHasQuarterState() = 0;

	virtual ~IGumballMachine() = default;
};

class SoldState : public IState
{
public:
	SoldState(IGumballMachine& gumballMachine, std::ostream& out)
		: m_gumballMachine(gumballMachine)
		, out(out)
	{
	}
	void InsertQuarter() override
	{
		out << "Please wait, we're already giving you a gumball\n";
	}
	void EjectQuarter() override
	{
		out << "Sorry you already turned the crank\n";
	}
	void TurnCrank() override
	{
		out << "Turning twice doesn't get you another gumball\n";
	}
	void Dispense() override
	{
		m_gumballMachine.ReleaseBall();
		if (m_gumballMachine.GetBallCount() == 0)
		{
			out << "Oops, out of gumballs\n";
			m_gumballMachine.SetSoldOutState();
		}
		else
		{
			m_gumballMachine.SetNoQuarterState();
		}
	}
	std::string ToString() const override
	{
		return "delivering a gumball";
	}

private:
	IGumballMachine& m_gumballMachine;
	std::ostream& out;
};

class SoldOutState : public IState
{
public:
	SoldOutState(IGumballMachine& m_gumball_machine, std::ostream& out)
		: m_gumballMachine(m_gumball_machine)
		, out(out)
	{
	}

	void InsertQuarter() override
	{
		out << "You can't insert a quarter, the machine is sold out\n";
	}
	void EjectQuarter() override
	{
		out << "You can't eject, you haven't inserted a quarter yet\n";
	}
	void TurnCrank() override
	{
		out << "You turned but there's no gumballs\n";
	}
	void Dispense() override
	{
		out << "No gumball dispensed\n";
	}
	std::string ToString() const override
	{
		return "sold out";
	}

private:
	IGumballMachine& m_gumballMachine;
	std::ostream& out;
};

class HasQuarterState : public IState
{
public:
	explicit HasQuarterState(IGumballMachine& gumballMachine, std::ostream& out)
		: m_gumballMachine(gumballMachine)
		, out(out)
	{
	}

	void InsertQuarter() override
	{
		out << "You can't insert another quarter\n";
	}
	void EjectQuarter() override
	{
		out << "Quarter returned\n";
		m_gumballMachine.SetNoQuarterState();
	}
	void TurnCrank() override
	{
		out << "You turned...\n";
		m_gumballMachine.SetSoldState();
	}
	void Dispense() override
	{
		out << "No gumball dispensed\n";
	}
	std::string ToString() const override
	{
		return "waiting for turn of crank";
	}

private:
	IGumballMachine& m_gumballMachine;
	std::ostream& out;
};

class NoQuarterState : public IState
{
public:
	explicit NoQuarterState(IGumballMachine& gumballMachine, std::ostream& out)
		: m_gumballMachine(gumballMachine)
		, out(out)
	{
	}

	void InsertQuarter() override
	{
		out << "You inserted a quarter\n";
		m_gumballMachine.SetHasQuarterState();
	}
	void EjectQuarter() override
	{
		out << "You haven't inserted a quarter\n";
	}
	void TurnCrank() override
	{
		out << "You turned but there's no quarter\n";
	}
	void Dispense() override
	{
		out << "You need to pay first\n";
	}
	std::string ToString() const override
	{
		return "waiting for quarter";
	}

private:
	IGumballMachine& m_gumballMachine;
	std::ostream& out;
};

class GumballMachine : private IGumballMachine
{
public:
	explicit GumballMachine(unsigned numBalls, std::ostream& out)
		: m_count(numBalls)
		, m_soldState(*this, out)
		, m_soldOutState(*this, out)
		, m_noQuarterState(*this, out)
		, m_hasQuarterState(*this, out)
		, m_state(&m_soldOutState)
	{
		if (m_count > 0)
		{
			m_state = &m_noQuarterState;
		}
	}
	void EjectQuarter()
	{
		m_state->EjectQuarter();
	}
	void InsertQuarter()
	{
		m_state->InsertQuarter();
	}
	void TurnCrank()
	{
		m_state->TurnCrank();
		m_state->Dispense();
	}
	std::string ToString() const
	{
		return std::format("Mighty Gumball, Inc.\n"
						   "C++-enabled Standing Gumball Model #2016\n"
						   "Inventory: {} gumball{}\n"
						   "Machine is {}",
			m_count, m_count != 1 ? "s" : "", m_state->ToString());
	}

private:
	unsigned GetBallCount() const override
	{
		return m_count;
	}

	void ReleaseBall() override
	{
		if (m_count != 0)
		{
			std::cout << "A gumball comes rolling out the slot...\n";
			--m_count;
		}
	}

	void SetSoldOutState() override
	{
		m_state = &m_soldOutState;
	}

	void SetNoQuarterState() override
	{
		m_state = &m_noQuarterState;
	}

	void SetSoldState() override
	{
		m_state = &m_soldState;
	}

	void SetHasQuarterState() override
	{
		m_state = &m_hasQuarterState;
	}

	unsigned m_count = 0;
	SoldState m_soldState;
	SoldOutState m_soldOutState;
	NoQuarterState m_noQuarterState;
	HasQuarterState m_hasQuarterState;
	IState* m_state;
};
