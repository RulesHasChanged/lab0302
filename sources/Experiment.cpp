#include "HardwareData.h"
#include "Experiment.h"
#include "Cli.h"
#include <cmath>
#include <chrono>
#include <iostream>
#include <random>

uint8_t Random::generateByte() const
{
    std::uniform_int_distribution<> converter(0, 128);
    return converter(generator);
}

size_t Random::generateSize(size_t end) const
{
    std::uniform_int_distribution<> converter(0, end);
    return converter(generator);
}

std::mt19937 Random::generator{std::random_device{}()};

Random::Random()
{
    Random::generator = std::mt19937{std::random_device{}()};
}

Experiment Experiment::doExperiment(size_t bufferSize, Investigation::Direction direction)
{
    std::cout << "Experiment. Size (KiB): " << bufferSize
              << ". Size (array size): " << kibToSize(bufferSize) << std::endl;
    bufferSize = kibToSize(bufferSize);

    // Инициализация
    class Random random;
    std::chrono::system_clock::time_point startTime, stopTime;
    auto buffer = createFilledBuffer(bufferSize);

    std::vector<size_t> indexes(bufferSize);
    std::iota(indexes.begin(), indexes.end(), 0);

    static const std::unordered_map<
        Investigation::Direction,
        std::function<void()>
    > runners = {
        {
            Investigation::Forward,
            [&buffer, &indexes]()
            {
                return run(buffer, indexes);
            }
        },
        {
            Investigation::Backward,
            [&buffer, &indexes]()
            {
                std::reverse(indexes.begin(), indexes.end());

                return run(buffer, indexes);
            }
        },
        {
            Investigation::Random,
            [&buffer, &indexes]()
            {
                std::shuffle(indexes.begin(), indexes.end(), std::mt19937{std::random_device{}()});

                return run(buffer, indexes);
            }
        },
    };

    // Выполнение
    startTime = std::chrono::high_resolution_clock::now();
    for (size_t k = 0; k < iterationAmount; k++) {
        runners.at(direction)();
    }
    stopTime = std::chrono::high_resolution_clock::now();

    // Возврат результата
    return {
        sizeToKib(bufferSize),
        static_cast<size_t>(
            std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - startTime).count()
        ),
    };
}

void Experiment::run(const BufferPtr &buffer, const std::vector<size_t> &indexes)
{
    AtomicType value = 0;

    for (size_t index : indexes) {
        value = buffer[index];
    }
    buffer[0] = value;          // Чтобы компилятор не ругался
}

const size_t Experiment::iterationAmount = 1000;

Experiment::BufferPtr Experiment::createFilledBuffer(size_t size)
{
    auto buffer = BufferPtr(new AtomicType[size]);
    Random random;

    for (size_t i = 0; i < size; i++) {
        buffer[i] = random.generateByte();
    }

    return buffer;
}

size_t Experiment::kibToSize(size_t kib)
{
    return kib * 1024 / sizeof(AtomicType);
}

size_t Experiment::sizeToKib(size_t size)
{
    return size * sizeof(AtomicType) / 1024;
}

Investigation Investigation::doInvestigation(Direction direction, const std::vector<size_t> &bufferSizes)
{
    std::cout << "Investigation. Direction: " << Cli::directionToString(direction) << std::endl;

    std::vector<Experiment> experiments;

    for (size_t size: bufferSizes) {
        experiments.emplace_back(
            Experiment::doExperiment(size, direction)
        );
    }

    return {
        direction,
        std::move(experiments),
    };
}

ExperimentInitData ExperimentInitData::getExperimentData(const HardwareData &hardware)
{
    size_t startValue = hardware.cacheMemoryKB.front() / 2;
    size_t stopValue = hardware.cacheMemoryKB.back() * 3 / 2;

    size_t startPower = ceil(log2(startValue));
    size_t stopPower = floor(log2(stopValue));

    std::vector<size_t> resultBufferSizes;
    resultBufferSizes.reserve(2 + (stopPower - startPower + 1));                // borders + powers

    resultBufferSizes.push_back(startValue);
    for (size_t power = startPower; power <= stopPower; power++) {
        resultBufferSizes.push_back(1u << power);
    }
    resultBufferSizes.push_back(stopValue);

    return {
        std::move(resultBufferSizes),
    };
}
