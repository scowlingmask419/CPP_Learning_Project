#pragma once

#include "geometry.hpp"

#include <deque>

enum WaypointType
{
    wp_air,
    wp_ground,
    wp_terminal
};

class Waypoint : public Point3D
{
public:
    // const WaypointType type;
    //  TASK_2 - B.4)
    WaypointType type;

    Waypoint(const Point3D &position, const WaypointType type_ = wp_air) : Point3D{position}, type{type_}
    {
    }

    // TASK_2 - B.4)
    Waypoint(const Waypoint &) = default;
    ~Waypoint() = default;

    bool is_on_ground() const { return type != wp_air; }
    bool is_at_terminal() const { return type == wp_terminal; }

    // TASK_2 - B.4)
    Waypoint &operator=(const Waypoint &other)
    {
        if (this != &other)
        {
            values = other.values;
            type = other.type;
        }
        return *this;
    }
};

using WaypointQueue = std::deque<Waypoint>;