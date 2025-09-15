#include "lib/Duck/DecoyDuck.h"
#include "lib/Duck/MallardDuck.h"
#include "lib/Duck/ModelDuck.h"
#include "lib/Duck/RedheadDuck.h"
#include "lib/Duck/RubberDuck.h"
#include "lib/DuckFunctions.h"
#include <cstdlib>

using Strategy = std::function<int(int x, int y)>;
class Context
{
public:
	void SetStrategy(const Strategy& s)
	{
		m_strategy = s;
	}

	[[nodiscard]] int Perform(int x, int y) const
	{
		if (m_strategy)
		{
			return m_strategy(x, y);
		}
		return 0;
	}

private:
	Strategy m_strategy;
};

int AddStrategy(int x, int y)
{
	return x < y;
}
int main()
{
	MallardDuck mallardDuck;
	PlayWithDuck(mallardDuck);
	mallardDuck.Fly();
	mallardDuck.Fly();
	mallardDuck.Fly();

	RedheadDuck redheadDuck;
	PlayWithDuck(redheadDuck);

	RubberDuck rubberDuck;
	PlayWithDuck(rubberDuck);

	DecoyDuck decoyDuck;
	PlayWithDuck(decoyDuck);

	ModelDuck modelDuck;
	PlayWithDuck(modelDuck);

	modelDuck.SetFlyBehavior(FlyWithWings);
	PlayWithDuck(modelDuck);

	return EXIT_SUCCESS;
}