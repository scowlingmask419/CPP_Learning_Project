#pragma once

#include <ostream>
#include <vector>
#include <memory>
#include "aircraft.hpp"
#include "GL/dynamic_object.hpp"

class Aircraft;

class AircraftManager : public GL::DynamicObject
{
public:
    // TASK_1 - B.2)
    AircraftManager();
    ~AircraftManager() override = default;
    AircraftManager(const AircraftManager &) = delete;

    void add_aircraft(std::unique_ptr<Aircraft>);
    bool move() override;

private:
    std::vector<std::unique_ptr<Aircraft>> aircrafts;
};
