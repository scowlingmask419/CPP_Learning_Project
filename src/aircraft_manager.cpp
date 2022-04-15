#include "aircraft_manager.hpp"

// TASK_1 - B.2)
AircraftManager::AircraftManager()
{
    GL::move_queue.emplace(this);
}