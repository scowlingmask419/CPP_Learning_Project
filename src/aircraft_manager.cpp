#include "aircraft_manager.hpp"

// TASK_1 - B.2)
AircraftManager::AircraftManager()
{
    GL::move_queue.emplace(this);
}

// TASK_1 - C.3)
bool AircraftManager::move()
{
    for (auto it = aircrafts.begin(); it != aircrafts.end();)
    {
        auto &aircraft = **it;

        if (aircraft.move())
        {
            ++it;
        }
        else
        {
            it = aircrafts.erase(it);
        }
    }
    return true;
}

// TASK_1 - A
void AircraftManager::add_aircraft(std::unique_ptr<Aircraft> aircraft)
{
    assert(aircraft != nullptr);
    aircrafts.emplace_back(std::move(aircraft));
}