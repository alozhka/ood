#include "../lib/Duck/DecoyDuck.h"
#include "../lib/Duck/Duck.h"
#include "../lib/Duck/MallardDuck.h"
#include "../lib/Duck/ModelDuck.h"
#include "../lib/Duck/RubberDuck.h"

#include <gtest/gtest.h>
#include <iostream>
#include <sstream>

class DuckBehaviorTest : public testing::Test
{
protected:
	void SetUp() override
	{
		// Перенаправляем cout для тестирования вывода
		original_cout_buffer = std::cout.rdbuf();
		std::cout.rdbuf(test_output.rdbuf());
	}

	void TearDown() override
	{
		// Восстанавливаем cout
		std::cout.rdbuf(original_cout_buffer);
	}

	std::string GetOutput()
	{
		std::string result = test_output.str();
		test_output.str("");
		test_output.clear();
		return result;
	}

private:
	std::stringstream test_output;
	std::streambuf* original_cout_buffer = nullptr;
};

TEST_F(DuckBehaviorTest, MallardDuckFlightCounterWorks)
{
	MallardDuck mallard;

	// Первый полёт
	mallard.Fly();
	std::string output1 = GetOutput();
	EXPECT_NE(output1.find("Flight #1"), std::string::npos);

	// Второй полёт - должен крякнуть
	mallard.Fly();
	std::string output2 = GetOutput();
	EXPECT_NE(output2.find("Flight #2"), std::string::npos);
	EXPECT_NE(output2.find("Quack"), std::string::npos);

	// Третий полёт
	mallard.Fly();
	std::string output3 = GetOutput();
	EXPECT_NE(output3.find("Flight #3"), std::string::npos);
}

TEST_F(DuckBehaviorTest, ModelDuckInitiallyCannotFly)
{
	ModelDuck model;

	// Первый полёт - не может летать
	model.Fly();
	std::string output1 = GetOutput();
	EXPECT_NE(output1, "I'm a model duck and I can't fly\n");

	// Второй полёт - теперь может летать
	model.Fly();
	std::string output2 = GetOutput();
	EXPECT_NE(output2.find("Flight #1"), std::string::npos);
}

TEST_F(DuckBehaviorTest, RubberDuckDoesNotFly)
{
	RubberDuck rubber;

	rubber.Fly();
	std::string output = GetOutput();
	// Резиновая утка не должна выводить сообщения о полёте
	EXPECT_EQ(output.find("flying"), std::string::npos);
	EXPECT_EQ(output.find("Flight"), std::string::npos);
}

TEST_F(DuckBehaviorTest, DanceBehaviorWorks)
{
	MallardDuck mallard;
	mallard.Dance();
	EXPECT_EQ(GetOutput(), "");

	RubberDuck rubber;
	rubber.Dance();
	std::string output2 = GetOutput();
	// Резиновая утка не танцует (пустой вывод)
	EXPECT_TRUE(output2.empty());
}

TEST_F(DuckBehaviorTest, QuackBehaviorWorks)
{
	MallardDuck mallard;
	mallard.Quack();
	std::string output1 = GetOutput();
	EXPECT_NE(output1.find("Quack"), std::string::npos);

	RubberDuck rubber;
	rubber.Quack();
	std::string output2 = GetOutput();
	EXPECT_NE(output2.find("Squeak"), std::string::npos);

	DecoyDuck decoy;
	decoy.Quack();
	std::string output3 = GetOutput();
	EXPECT_NE(output3.find("<< Silence >>"), std::string::npos);
}

TEST_F(DuckBehaviorTest, SetFlyBehaviorWorks)
{
	ModelDuck model;

	// Изначально не может летать
	model.Fly();
	GetOutput(); // Очищаем вывод

	// Устанавливаем новое поведение полёта
	model.SetFlyBehavior(CreateFlyWithWings());

	// Теперь может летать
	model.Fly();
	std::string output = GetOutput();
	EXPECT_NE(output.find("I'm flying with wings! Flight #1"), std::string::npos);
}

