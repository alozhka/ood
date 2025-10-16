#include "src/Beverages.h"
#include "src/Condiments.h"

#include <functional>
#include <iostream>

/*
Функциональный объект, создающий лимонную добавку
*/
struct MakeLemon
{
	explicit MakeLemon(unsigned quantity)
		: m_quantity(quantity)
	{
	}

	auto operator()(IBeveragePtr&& beverage) const
	{
		return make_unique<Lemon>(std::move(beverage), m_quantity);
	}

private:
	unsigned m_quantity;
};

/*
Функция, возвращающая функцию, создающую коричную добавку
*/
std::function<IBeveragePtr(IBeveragePtr&&)> MakeCinnamon()
{
	return [](IBeveragePtr&& b) {
		return make_unique<Cinnamon>(std::move(b));
	};
}

/*
Возвращает функцию, декорирующую напиток определенной добавкой

Параметры шаблона:
	Condiment - класс добавки, конструктор которого в качестве первого аргумента
				принимает IBeveragePtr&& оборачиваемого напитка
	Args - список типов прочих параметров конструктора (возможно, пустой)
*/
template <typename Condiment, typename... Args>
auto MakeCondiment(const Args&... args)
{
	// Возвращаем функцию, декорирующую напиток, переданный ей в качестве аргумента
	// Дополнительные аргументы декоратора, захваченные лямбда-функцией, передаются
	// конструктору декоратора через make_unique
	return [=](auto&& b) {
		// Функции make_unique передаем b вместе со списком аргументов внешней функции
		return make_unique<Condiment>(std::forward<decltype(b)>(b), args...);
	};
}

/*
Перегруженная версия оператора <<, которая предоставляет нам синтаксический сахар
для декорирования компонента

Позволяет создать цепочку оборачивающих напиток декораторов следующим образом:
auto beverage = make_unique<CConcreteBeverage>(a, b, c)
					<< MakeCondimentA(d, e, f)
					<< MakeCondimentB(g, h);

Функциональные объекты MakeCondiment* запоминают аргументы, необходимые для создания
дополнения, и возвращают фабричную функцию, принимающую оборачиваемый напиток, которая
при своем вызове создаст нужный объект Condiment, передав ему запомненные аргументы.
Использование:
	auto beverage =
		make_unique<CConcreteBeverage>(a, b, c)
		<< MakeCondimentA(d, e, f)
		<< MakeCondimentB(g, h);
или даже так:
	auto beverage =
		make_unique<CConcreteBeverage>
		<< MakeCondiment<CondimentA>(d, e, f)
		<< MakeCondiment<CondimentB>(g, h);
В последнем случае нет необходимости писать вручную реализации MakeCondimentA и MakeCondimentB, т.к.
необходимую реализацию сгенерирует компилятор

Классический способ оборачивания выглядел бы так:
	auto baseBeverage = make_unique<CConcretedBeverage>(a, b, c);
	auto wrappedWithCondimentA = make_unique<CCondimentA>(move(baseBeverage), d, e, f);
	auto beverage = make_unique<CCondimentB>(move(wrappedWithCondimentA), g, h);
либо так:
	auto beverage = make_unique<CCondimentB>(
						make_unique<CCondimentA>(
							make_unique<CConcreteBeverage>(a, b, c), // Напиток
							d, e, f	// доп. параметы CondimentA
						),
						g, h		// доп. параметры CondimentB
					);

unique_ptr<CLemon> operator << (IBeveragePtr && lhs, const MakeLemon & factory)
{
	return factory(move(lhs));
}
unique_ptr<CCinnamon> operator << (IBeveragePtr && lhs, const MakeCinnamon & factory)
{
	return factory(move(lhs));
}
*/
template <typename Component, typename Decorator>
auto operator<<(Component&& component, const Decorator& decorate)
{
	return decorate(std::forward<Component>(component));
}

void DialogWithUser()
{
	std::cout << "Type 1 for coffee or 2 for tea\n";
	int beverageChoice;
	std::cin >> beverageChoice;

	std::unique_ptr<IBeverage> beverage;

	if (beverageChoice == 1)
	{
		beverage = std::make_unique<Coffee>();
	}
	else if (beverageChoice == 2)
	{
		beverage = std::make_unique<Tea>(TeaType::Puer);
	}
	else
	{
		return;
	}

	int condimentChoice;
	for (;;)
	{
		std::cout << "1 - Lemon, 2 - Cinnamon, 0 - Checkout" << std::endl;
		std::cin >> condimentChoice;

		if (condimentChoice == 1)
		{
			// beverage = make_unique<CLemon>(move(beverage));
			beverage = std::move(beverage) << MakeCondiment<Lemon>(2);
		}
		else if (condimentChoice == 2)
		{
			// beverage = make_unique<CCinnamon>(move(beverage));
			beverage = std::move(beverage) << MakeCondiment<Cinnamon>();
		}
		else if (condimentChoice == 0)
		{
			break;
		}
		else
		{
			return;
		}
	}

	std::cout << beverage->GetDescription() << ", cost: " << beverage->GetCost() << std::endl;
}

int main()
{
	DialogWithUser();
	std::cout << std::endl;
	{
		// Наливаем чашечку латте
		auto latte = std::make_unique<Latte>();
		// добавляем корицы
		auto cinnamon = std::make_unique<Cinnamon>(std::move(latte));
		// добавляем пару долек лимона
		auto lemon = std::make_unique<Lemon>(std::move(cinnamon), 2);
		// добавляем пару кубиков льда
		auto iceCubes = std::make_unique<IceCubes>(std::move(lemon), 2, IceCubeType::Dry);
		// добавляем 2 грамма шоколадной крошки
		auto beverage = std::make_unique<ChocolateCrumbs>(std::move(iceCubes), 2);

		// Выписываем счет покупателю
		std::cout << beverage->GetDescription() << " costs " << beverage->GetCost() << std::endl;
	}

	{
		auto beverage = std::make_unique<ChocolateCrumbs>( // Внешний слой: шоколадная крошка
			std::make_unique<IceCubes>( // | под нею - кубики льда
				std::make_unique<Lemon>( // | | еще ниже лимон
					std::make_unique<Cinnamon>( // | | | слоем ниже - корица
						std::make_unique<Latte>()), // | | | в самом сердце - Латте
					2), // | | 2 дольки лимона
				2, IceCubeType::Dry), // | 2 кубика сухого льда
			2); // 2 грамма шоколадной крошки

		// Выписываем счет покупателю
		std::cout << beverage->GetDescription() << " costs " << beverage->GetCost() << std::endl;
	}

	// Подробнее рассмотрим работу MakeCondiment и оператора <<
	{
		// lemon - функция, добавляющая "2 дольки лимона" к любому напитку
		auto lemon2 = MakeCondiment<Lemon>(2);
		// iceCubes - функция, добавляющая "3 кусочка льда" к любому напитку
		auto iceCubes3 = MakeCondiment<IceCubes>(3, IceCubeType::Water);

		auto tea = std::make_unique<Tea>(TeaType::Black);

		// декорируем чай двумя дольками лимона и тремя кусочками льда
		auto lemonIceTea = iceCubes3(lemon2(std::move(tea)));
		/* Предыдущая строка выполняет те же действия, что и следующий код:
		auto lemonIceTea =
			make_unique<CIceCubes>(
				make_unique<CLemon>(
					move(tea),
					2),
				2, IceCubeType::Water);
		*/

		auto oneMoreLemonIceTea = std::make_unique<Tea>(TeaType::Green) // Берем чай
			<< MakeCondiment<Lemon>(2) // добавляем пару долек лимона
			<< MakeCondiment<IceCubes>(3, IceCubeType::Water); // и 3 кубика льда
		/*
		Предыдущая конструкция делает то же самое, что и следующая:
		auto oneMoreLemonIceTea =
			MakeCondiment<CIceCubes>(3, IceCubeType::Water)(
				MakeCondiment<CLemon>(2)(make_unique<CTea>())
				);
		*/
	}

	// Аналог предыдущего решения с добавкой синтаксического сахара
	// обеспечиваемого операторами << и функцией MakeCondiment
	{
		auto beverage = std::make_unique<Latte>() // Наливаем чашечку латте,
			<< MakeCondiment<Cinnamon>() // оборачиваем корицей,
			<< MakeCondiment<Lemon>(2) // добавляем пару долек лимона
			<< MakeCondiment<IceCubes>(2, IceCubeType::Dry) // брасаем пару кубиков сухого льда
			<< MakeCondiment<ChocolateCrumbs>(2); // посыпаем шоколадной крошкой

		// Выписываем счет покупателю
		std::cout << beverage->GetDescription() << " costs " << beverage->GetCost() << std::endl;
	}

	{
		auto beverage = std::make_unique<Milkshake>(MilkshakeSize::Medium) // Наливаем молочный коктейль
			<< MakeCondiment<Syrup>(SyrupType::Maple) // заливаем кленовым сиропом
			<< MakeCondiment<CoconutFlakes>(8); // посыпаем кокосовой стружкой

		// Выписываем счет покупателю
		std::cout << beverage->GetDescription() << " costs " << beverage->GetCost() << std::endl;
	}

	{
		auto beverage = std::make_unique<Tea>(TeaType::Puer)
			<< MakeCondiment<Liquor>(LiquorType::Chocolate)
			<< MakeCondiment<ChocolatePieces>(4)
			<< MakeCondiment<Cream>();

		std::cout << beverage->GetDescription() << " costs " << beverage->GetCost() << std::endl;
	}
}
