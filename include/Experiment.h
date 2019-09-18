#pragma once

#include <vector>
#include <memory>
#include <random>
#include <functional>
#include <chrono>

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
    using AtomicType = uint64_t;
    using BufferPtr = std::shared_ptr<AtomicType[]>;
    using WalkFunctionType = std::function<AtomicType(BufferPtr, size_t)>;

    size_t bufferSize;
    size_t duration;

    static Experiment doExperiment(size_t bufferSize, Investigation::Direction direction);

    static void run(const BufferPtr &buffer, const std::vector<size_t> &indexes);

    static size_t kibToSize(size_t kib);
    static size_t sizeToKib(size_t size);

    static const size_t iterationAmount;
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


