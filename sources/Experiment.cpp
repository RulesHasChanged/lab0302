#include "HardwareData.h"
#include "Experiment.h"
#include "Cli.h"
#include <cmath>
#include <chrono>
#include <iostream>

#define BURN_CACHE uint8_t value = 0; \
    for (size_t i = 0; i < bufferSize; i++) { \
       value = buffer[i]; \
    }

#define WALK_FORWARD \
    for (size_t i = 0; i < bufferSize; i++) { \
        value = buffer[i]; \
    }

#define WALK_BACKWARD \
    for (size_t i = 0; i < bufferSize; i++) { \
        value = buffer[bufferSize - i - 1]; \
    }

#define WALK_RANDOM \
    for (size_t i = 0; i < bufferSize; i++) { \
        value = buffer[random.generateSize(bufferSize)]; \
    }

#define VALUE_USEFUL \
buffer[0] = value;

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
    std::cout << "Experiment. Size: " << bufferSize << std::endl;

    // Инициализация
    class Random random;
    std::chrono::system_clock::time_point startTime, stopTime;
    auto buffer = createFilledBuffer(bufferSize);

    // Выполнение
    if (direction == Investigation::Forward) {
        BURN_CACHE
        startTime = std::chrono::high_resolution_clock::now();

        for (size_t k = 0; k < iterationAmount; k++) {
            WALK_FORWARD
        }

        stopTime = std::chrono::high_resolution_clock::now();
        VALUE_USEFUL
    } else if (direction == Investigation::Backward) {
        BURN_CACHE
        startTime = std::chrono::high_resolution_clock::now();

        for (size_t k = 0; k < iterationAmount; k++) {
            WALK_BACKWARD
        }

        stopTime = std::chrono::high_resolution_clock::now();
        VALUE_USEFUL
    } else {
        BURN_CACHE
        startTime = std::chrono::high_resolution_clock::now();

        for (size_t k = 0; k < iterationAmount; k++) {
            WALK_RANDOM
        }

        stopTime = std::chrono::high_resolution_clock::now();
        VALUE_USEFUL
    }

    // Возврат результата
    return {
        bufferSize,
        static_cast<size_t>(
            std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime).count()
        ),
    };
}

size_t Experiment::iterationAmount = 1000;

Experiment::BufferPtr Experiment::createFilledBuffer(size_t size)
{
    auto buffer = BufferPtr(new uint8_t[size]);
    Random random;

    for (size_t i = 0; i < size; i++) {
        buffer[i] = random.generateByte();
    }

    return buffer;
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
