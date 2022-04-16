#pragma once

#include "aircraft_types.hpp"
#include "aircraft.hpp"
#include "tower.hpp"

class Aircraft;
class Tower;

// TASK_1 - A
inline std::vector<std::string> airlines{"AF", "LH", "EY", "DL", "KL", "BA", "AY", "EY"};

class AircraftFactory
{
private:
    // TASK_1 - A
    std::vector<std::unique_ptr<AircraftType>> aircraft_types;
    // TASK_1 - B
    std::unordered_set<std::string> flight_numbers;

public:
    AircraftFactory(const AircraftFactory &) = delete;
    AircraftFactory &operator=(const AircraftFactory &) = delete;
    AircraftFactory();
    ~AircraftFactory() = default;

    // TASK_1 - A
    std::string get_flight_number();
    // TASK_1 - A
    std::unique_ptr<Aircraft> create_aircraft(Tower &tower);
};