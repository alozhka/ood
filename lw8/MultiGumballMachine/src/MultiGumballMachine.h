#pragma once

#include <format>
#include <iostream>

struct IMultiGumballMachine
{
	virtual void ReleaseBall() = 0;
	virtual unsigned GetBallCount() const = 0;
	virtual unsigned GetQuarterCount() const = 0;
	virtual void AddQuarter() = 0;
	virtual void ReturnQuarters() = 0;
	virtual unsigned RemoveQuarter() = 0;
	virtual bool HasQuarters() const = 0;
	virtual bool IsFullOfQuarters() const = 0;

	virtual void SetSoldOutState() = 0;
	virtual void SetNoQuarterState() = 0;
	virtual void SetSoldState() = 0;
	virtual void SetHasQuarterState() = 0;

	virtual ~IMultiGumballMachine() = default;
};

struct IState
{
	virtual void InsertQuarter() = 0;
	virtual void EjectQuarter() = 0;
	virtual void TurnCrank() = 0;
	virtual void Dispense() = 0;
	virtual std::string ToString() const = 0;
	virtual ~IState() = default;
};

class SoldState : public IState
{
public:
	SoldState(IMultiGumballMachine& gumballMachine, std::ostream& out)
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
		m_gumballMachine.RemoveQuarter();

		if (m_gumballMachine.GetBallCount() == 0)
		{
			out << "Oops, out of gumballs\n";
			m_gumballMachine.SetSoldOutState();
		}
		else if (m_gumballMachine.HasQuarters())
		{
			m_gumballMachine.SetHasQuarterState();
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
	IMultiGumballMachine& m_gumballMachine;
	std::ostream& out;
};

class SoldOutState : public IState
{
public:
	SoldOutState(IMultiGumballMachine& gumballMachine, std::ostream& out)
		: m_gumballMachine(gumballMachine)
		, out(out)
	{
	}

	void InsertQuarter() override
	{
		out << "You can't insert a quarter, the machine is sold out\n";
	}

	void EjectQuarter() override
	{
		if (m_gumballMachine.HasQuarters())
		{
			m_gumballMachine.ReturnQuarters();
		}
		else
		{
			out << "You can't eject, you haven't inserted a quarter yet\n";
		}
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
	IMultiGumballMachine& m_gumballMachine;
	std::ostream& out;
};

class HasQuarterState : public IState
{
public:
	explicit HasQuarterState(IMultiGumballMachine& gumballMachine, std::ostream& out)
		: m_gumballMachine(gumballMachine)
		, out(out)
	{
	}

	void InsertQuarter() override
	{
		if (m_gumballMachine.IsFullOfQuarters())
		{
			out << "You can't insert another quarter, the machine is full\n";
		}
		else
		{
			out << "You inserted another quarter\n";
			m_gumballMachine.AddQuarter();
		}
	}

	void EjectQuarter() override
	{
		m_gumballMachine.ReturnQuarters();
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
	IMultiGumballMachine& m_gumballMachine;
	std::ostream& out;
};

class NoQuarterState : public IState
{
public:
	explicit NoQuarterState(IMultiGumballMachine& gumballMachine, std::ostream& out)
		: m_gumballMachine(gumballMachine)
		, out(out)
	{
	}

	void InsertQuarter() override
	{
		out << "You inserted a quarter\n";
		m_gumballMachine.AddQuarter();
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
	IMultiGumballMachine& m_gumballMachine;
	std::ostream& out;
};

class MultiGumballMachine : private IMultiGumballMachine
{
public:
	static constexpr unsigned MAX_QUARTERS = 5;

	explicit MultiGumballMachine(unsigned numBalls, std::ostream& out)
		: m_count(numBalls)
		, m_soldState(*this, out)
		, m_soldOutState(*this, out)
		, m_noQuarterState(*this, out)
		, m_hasQuarterState(*this, out)
		, m_state(&m_soldOutState)
		, m_output(out)
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
						   "C++-enabled Standing Gumball Model #2016 (Multi-Quarter)\n"
						   "Inventory: {} gumball{}\n"
						   "Quarters inserted: {}/{}\n"
						   "Machine is {}",
			m_count,
			m_count != 1 ? "s" : "",
			m_quarters,
			MAX_QUARTERS,
			m_state->ToString());
	}

private:
	unsigned GetBallCount() const override
	{
		return m_count;
	}

	unsigned GetQuarterCount() const override
	{
		return m_quarters;
	}

	void AddQuarter() override
	{
		if (m_quarters < MAX_QUARTERS)
		{
			++m_quarters;
		}
	}

	void ReturnQuarters() override
	{
		if (m_quarters > 0)
		{
			m_output << "Returning " << m_quarters << " quarter" << (m_quarters != 1 ? "s" : "") << "\n";
			m_quarters = 0;
		}
	}

	unsigned RemoveQuarter() override
	{
		if (m_quarters > 0)
		{
			return --m_quarters;
		}
		return 0;
	}

	bool HasQuarters() const override
	{
		return m_quarters > 0;
	}

	bool IsFullOfQuarters() const override
	{
		return m_quarters >= MAX_QUARTERS;
	}

	void ReleaseBall() override
	{
		if (m_count != 0)
		{
			m_output << "A gumball comes rolling out the slot...\n";
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
	unsigned m_quarters = 0;
	SoldState m_soldState;
	SoldOutState m_soldOutState;
	NoQuarterState m_noQuarterState;
	HasQuarterState m_hasQuarterState;
	IState* m_state;
	std::ostream& m_output;
};