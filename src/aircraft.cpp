#include "aircraft.hpp"

#include "GL/opengl_interface.hpp"

#include <cmath>

void Aircraft::turn_to_waypoint()
{
    if (!waypoints.empty())
    {
        Point3D target = waypoints[0];
        if (waypoints.size() > 1)
        {
            const float d = (waypoints[0] - pos).length();
            const Point3D W = (waypoints[0] - waypoints[1]).normalize(d / 2.0f);
            target += W;
        }

        turn(target - pos - speed);
    }
}

void Aircraft::turn(Point3D direction)
{
    (speed += direction.cap_length(type.max_accel)).cap_length(max_speed());
}

unsigned int Aircraft::get_speed_octant() const
{
    const float speed_len = speed.length();
    if (speed_len > 0)
    {
        const Point3D norm_speed{speed * (1.0f / speed_len)};
        const float angle =
            (norm_speed.y() > 0) ? 2.0f * 3.141592f - std::acos(norm_speed.x()) : std::acos(norm_speed.x());
        // partition into NUM_AIRCRAFT_TILES equal pieces
        return (static_cast<int>(std::round((angle * NUM_AIRCRAFT_TILES) / (2.0f * 3.141592f))) + 1) %
               NUM_AIRCRAFT_TILES;
    }
    else
    {
        return 0;
    }
}

// when we arrive at a terminal, signal the tower
void Aircraft::arrive_at_terminal()
{
    // we arrived at a terminal, so start servicing
    control.arrived_at_terminal(*this);
    is_at_terminal = true;
}

// deploy and retract landing gear depending on next waypoints
void Aircraft::operate_landing_gear()
{
    if (waypoints.size() > 1u)
    {
        const auto it = waypoints.begin();
        const bool ground_before = it->is_on_ground();
        const bool ground_after = std::next(it)->is_on_ground();
        // deploy/retract landing gear when landing/lifting-off
        if (ground_before && !ground_after)
        {
            std::cout << flight_number << " lift off" << std::endl;
        }
        else if (!ground_before && ground_after)
        {
            std::cout << flight_number << " is now landing..." << std::endl;
            landing_gear_deployed = true;
        }
        else if (!ground_before && !ground_after)
        {
            landing_gear_deployed = false;
        }
    }
}

void Aircraft::add_waypoint(const Waypoint &wp, const bool front)
{
    if (front)
    {
        waypoints.push_front(wp);
    }
    else
    {
        waypoints.push_back(wp);
    }
}

// TASK_0 - C.4)
bool Aircraft::move()
{
    // TASK_2 - D
    if (fuel <= 0)
    {
        // std::cout << get_flight_num() << " crashed because of fuel" << std::endl;
        // TASK_3 - 1.1)
        throw AircraftCrash(get_flight_num() + " crashed");
        // return false;
    }

    if (waypoints.empty())
    {
        // TASK 0 - C.5)
        if (!has_been_served)
        {
            waypoints = control.get_instructions(*this);
        }
        else
        {
            return false;
        }
    }
    if (!is_at_terminal)
    {
        turn_to_waypoint();
        // move in the direction of the current speed
        pos += speed;

        // if we are close to our next waypoint, stike if off the list
        if (!waypoints.empty() && distance_to(waypoints.front()) < DISTANCE_THRESHOLD)
        {
            if (waypoints.front().is_at_terminal())
            {
                arrive_at_terminal();
            }
            else
            {
                operate_landing_gear();
            }
            waypoints.pop_front();
        }

        // TASK_2 - E.4).2)
        if (is_circling())
        {

            auto waypoint_queue = control.reserve_terminal(*this); // On essaye de reserver un terminal
            if (!waypoint_queue.empty())
            {
                waypoints = waypoint_queue;
            }
        }

        if (is_on_ground())
        {
            if (!landing_gear_deployed)
            {
                using namespace std::string_literals;
                throw AircraftCrash{flight_number + " crashed into the ground"s};
            }
        }
        else
        {
            // TASK_2 - D
            fuel -= 1;

            // if we are in the air, but too slow, then we will sink!
            const float speed_len = speed.length();
            if (speed_len < SPEED_THRESHOLD)
            {
                pos.z() -= SINK_FACTOR * (SPEED_THRESHOLD - speed_len);
            }
        }

        // update the z-value of the displayable structure
        GL::Displayable::z = pos.x() + pos.y();
    }
    return true;
}

void Aircraft::display() const
{
    type.texture.draw(project_2D(pos), {PLANE_TEXTURE_DIM, PLANE_TEXTURE_DIM}, get_speed_octant());
}

// TASK_2 - E.1)
bool Aircraft::has_terminal() const
{
    return !waypoints.empty() && waypoints.back().is_at_terminal();
}

// TASK_2 - E.2)
bool Aircraft::is_circling() const
{
    return !waypoints.empty() && !waypoints.back().is_on_ground() && !landing_gear_deployed;
}

// TASK_2 - F
bool Aircraft::operator<(const Aircraft &aircraft) const
{
    return has_terminal() != aircraft.has_terminal() ? has_terminal() : fuel < aircraft.fuel;
}

// TASK_2 - G.1)
bool Aircraft::is_low_on_fuel() const
{
    return fuel < 200;
}

// TASK_2 - G.2)
int Aircraft::missing_fuel() const
{
    // std::cout << "ICI" << std::endl;
    return 3000 - fuel;
}

// TASK_2 - G.4)
void Aircraft::refill(int &fuel_stock)
{
    int difference = missing_fuel();
    if (fuel_stock != 0)
    {
        if (fuel_stock < difference)
        {
            fuel += fuel_stock;
            std::cout << get_flight_num() << " filled with " << fuel_stock << "L of fuel" << std::endl;
            fuel_stock = 0;
        }
        else
        {
            fuel += difference;
            std::cout << get_flight_num() << " filled with " << difference << "L of fuel" << std::endl;
            fuel_stock -= difference;
        }
    }
}
