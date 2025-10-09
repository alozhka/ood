# Диаграмма классов

```mermaid
classDiagram
    class IObserver~T~ {
        <<interface>>
        +Update(data: T) void*
    }
    class IObservable~T~ {
        <<interface>>
        +RegisterObserver(observer: IObserver~T~) void*
        +NotifyObservers() void*
        +RemoveObserver(observer: IObserver~T~) void*
    }
    class Observable~T~ {
        -m_observers: set~Observer~T~~
        +RegisterObserver(observer: IObserver~T~) void
        +NotifyObserver() void
        +RemoveObserver(observer: IObserver~T~) void
        #GetChangedData() T*
    }
    class StatsDisplay {
        -m_temperatureInfo: StatsInfo
        -m_humidityInfo: StatsInfo
        -m_pressureInfo: StatsInfo
        -Update(data: WeatherData) void
    }
    class Display {
        -Update(data: WeatherData) void
    }
    class StatsInfo {
        -m_name: string
        -m_minValue: double
        -m_maxValue: double
        -m_accValue: double
        -m_countAcc: uint
        +Update(value: double) void
        +Print(output: out) void
    }
    class WeatherInfo {
        +temperature: double
        +humidity: double
        +pressure: double
    }
    class WeatherData {
        -m_temperature: double
        -m_humidity: double
        -m_pressure: double
        +GetTemperature() double
        +GetHumidity() doble
        +GetPressure() double
        +MeasurementsChanged() void
        +SetMeasurements()
        #GetChangedData() WeatherInfo
    }

    IObservable o-- IObserver
    IObserver <|-- Display
    IObserver <|-- StatsDisplay
    IObservable <|-- Observable
    StatsDisplay *-- StatsInfo
    WeatherData *-- WeatherInfo
    StatsDisplay o-- WeatherData
    Observable <|-- WeatherData
```