#pragma once

#include <ostream>
#include <vector>
#include <memory>
#include "aircraft.hpp"
#include "GL/dynamic_object.hpp"

class Aircraft;

// TASK_1 - C.3) : On souhaite que AircraftManager hérite de DynamicObject afin qu'il puisse être considéré comme tel.
class AircraftManager : public GL::DynamicObject
{
private:
    // TASK_1 - C.1)
    std::vector<std::unique_ptr<Aircraft>> aircrafts;

public:
    // TASK_1 - B.2)
    AircraftManager();
    ~AircraftManager() override = default;
    AircraftManager(const AircraftManager &) = delete;

    // AircraftManager &operator=(const AircraftManager &) = delete;
    bool move() override;

    // TASK_1 - C.3)
    // void add(std::unique_ptr<Aircraft>);
};
