#pragma once
#include "../src/GumballMachine.h"

class TestGumballMachine : private IGumballMachine
{
public:
	TestGumballMachine(unsigned numBalls, std::ostream& out)
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

	IState* GetSoldState()
	{
		return &m_soldState;
	}

	IState* GetHasQuarterState()
	{
		return &m_hasQuarterState;
	}

	void SetState(IState* state)
	{
		m_state = state;
	}

	// Общий интерфейс
	void InsertQuarter()
	{
		m_state->InsertQuarter();
	}
	void EjectQuarter()
	{
		m_state->EjectQuarter();
	}
	void TurnCrank()
	{
		m_state->TurnCrank();
	}
	void Dispense()
	{
		m_state->Dispense();
	}

	std::string ToString() const
	{
		return m_state->ToString();
	}

private:
	void ReleaseBall() override
	{
		if (m_count != 0)
		{
			m_output << "A gumball comes rolling out the slot...\n";
			--m_count;
		}
	}

	unsigned GetBallCount() const override
	{
		return m_count;
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
	std::ostream& m_output;
};
