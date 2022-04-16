#include "aircraft_factory.hpp"

AircraftFactory::AircraftFactory()
{
    // TASK_1 - A
    // cf., inline void init_aircraft_types()
    aircraft_types.emplace_back(std::make_unique<AircraftType>(.02f, .05f, .02f, MediaPath{"l1011_48px.png"}));
    aircraft_types.emplace_back(std::make_unique<AircraftType>(.02f, .05f, .02f, MediaPath{"b707_jat.png"}));
    aircraft_types.emplace_back(std::make_unique<AircraftType>(.02f, .1f, .02f, MediaPath{"concorde_af.png"}));
}

// TASK_1 - A
std::string AircraftFactory::get_flight_number()
{
    std::string flight_number;
    do
    {
        flight_number = airlines[std::rand() % airlines.size()] + std::to_string(1000 + (rand() % 9000));
    } while (flight_numbers.find(flight_number) != flight_numbers.end());
    flight_numbers.emplace(flight_number);
    return flight_number;
}

std::unique_ptr<Aircraft> AircraftFactory::create_aircraft(Tower &tower)
{
    // TASK_1 - A
    // cf., TowerSimulation::create_aircraft(const AircraftType &type)
    const std::string flight_number = get_flight_number();
    const float angle = (std::rand() % 1000) * 2 * 3.141592f / 1000.f; // random angle between 0 and 2pi
    const Point3D start = Point3D{std::sin(angle), std::cos(angle), 0.f} * 3 + Point3D{0.f, 0.f, 2.f};
    const Point3D direction = (-start).normalize();
    const AircraftType &type = *aircraft_types[std::rand() % aircraft_types.size()];

    return std::make_unique<Aircraft>(type, flight_number, start, direction, tower);
}
