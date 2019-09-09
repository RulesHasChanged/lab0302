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

struct Experiment;

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
};

struct Experiment
{
    using BufferPtr = std::shared_ptr<uint8_t[]>;
    using WalkFunctionType = std::function<uint8_t(BufferPtr, size_t)>;

    size_t bufferSize;
    size_t duration;

    static Experiment doExperiment(size_t bufferSize, Investigation::Direction direction);

    static size_t iterationAmount;

private:
    static BufferPtr createFilledBuffer(size_t size);
};

struct ExperimentData
{
    std::vector<Investigation> investigations;
};

struct ExperimentInitData
{
    static ExperimentInitData getExperimentData(const struct HardwareData &hardware);

    std::vector<size_t> bufferSizes;
};


