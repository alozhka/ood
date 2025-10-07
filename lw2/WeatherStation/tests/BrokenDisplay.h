#pragma once
#include "../src/Observer.h"
#include "../src/WeatherData.h"

class BrokenDisplay final : public IObserver<WeatherData>
{

public:
	void Update(const WeatherData& data) override;
};