#pragma once

#include "aircraft_manager.hpp"

class Airport;
struct AircraftType;

class TowerSimulation
{
private:
    bool help = false;
    Airport *airport = nullptr;
    // TASK_1 - C.2)
    std::unique_ptr<AircraftManager> aircraft_manager;

    TowerSimulation(const TowerSimulation &) = delete;
    TowerSimulation &operator=(const TowerSimulation &) = delete;

    void create_aircraft(const AircraftType &type) const;
    void create_random_aircraft() const;

    // CONST A RETIRER
    void create_keystrokes();
    void display_help() const;

    void init_airport();

public:
    TowerSimulation(int argc, char **argv);
    ~TowerSimulation();

    void launch();
};
