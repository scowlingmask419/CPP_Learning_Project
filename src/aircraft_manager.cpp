#include "aircraft_manager.hpp"

// TASK_1 - B.2)
AircraftManager::AircraftManager()
{
    GL::move_queue.emplace(this);
}

// TASK_1 - C.3)
bool AircraftManager::move()
{
    /*
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
    }*/

    // TASK_2 - B.4)
    std::sort(aircrafts.begin(), aircrafts.end(),
              [](const std::unique_ptr<Aircraft> &a, const std::unique_ptr<Aircraft> &b)
              { return *a < *b; });

    // TASK_2 - B.1)
    aircrafts.erase(std::remove_if(aircrafts.begin(), aircrafts.end(),
                                   [this](const std::unique_ptr<Aircraft> &aircraft)
                                   { return !aircraft->move(); }),
                    aircrafts.end());
    return true;
}

// TASK_1 - A
void AircraftManager::add_aircraft(std::unique_ptr<Aircraft> aircraft)
{
    assert(aircraft != nullptr);
    aircrafts.emplace_back(std::move(aircraft));
}

// TASK_2 - B.2)
int AircraftManager::count(const std::string_view &line)
{
    return std::count_if(aircrafts.begin(), aircrafts.end(),
                         [line](const std::unique_ptr<Aircraft> &a)
                         { return (a->get_flight_num().rfind(line, 0) == 0); });
}

// TASK_2 - C.2)
int AircraftManager::get_required_fuel()
{
    return std::accumulate(aircrafts.begin(), aircrafts.end(), 0,
                           [](int acc, const std::unique_ptr<Aircraft> &aircraft)
                           {
                               return aircraft->is_low_on_fuel() && aircraft->is_circling() ? acc + aircraft->missing_fuel() : acc;
                           });
}