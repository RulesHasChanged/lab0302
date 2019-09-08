#pragma once

#include <vector>
#include <memory>
#include <random>
#include <functional>

class Random
{
public:
    Random();

    [[nodiscard]] uint8_t generateByte() const;
    [[nodiscard]] size_t generateSize(size_t end) const;

private:
    static std::mt19937 generator;
};

struct Experiment
{
    using BufferPtr = std::shared_ptr<uint8_t[]>;

    size_t bufferSize;
    size_t duration;

    static Experiment doExperiment(size_t bufferSize, const std::function<void(BufferPtr, size_t)> &walkFunction);

    static size_t iterationAmount;

private:
    static BufferPtr createFilledBuffer(size_t size);
};

struct Investigation
{
    enum Direction
    {
        Forward,
        Backward,
        Random,
    };

    Direction direction;

    std::vector<Experiment> experiments;

    static Investigation doInvestigation(Direction direction, const std::vector<size_t>& bufferSizes);

    static void walkForward(const Experiment::BufferPtr &buffer, size_t size);
    static void walkBackward(const Experiment::BufferPtr & buffer, size_t size);
    static void walkRandom(const Experiment::BufferPtr & buffer, size_t size);

    static std::function<void(Experiment::BufferPtr, size_t)> getDirectionFunction(Direction direction);
};

struct ExperimentData
{
    std::vector<Investigation> investigations;
};

struct ExperimentInitData
{
    static ExperimentInitData getExperimentData(const class HardwareData &hardware);

    std::vector<size_t> bufferSizes;
};


