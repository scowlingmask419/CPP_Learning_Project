#include "tower_sim.hpp"

#include "GL/opengl_interface.hpp"
#include "aircraft.hpp"
#include "airport.hpp"
#include "config.hpp"
#include "img/image.hpp"
#include "img/media_path.hpp"

#include <cassert>
#include <cstdlib>
#include <ctime>

using namespace std::string_literals;

// TASK_1 - D
// const std::string airlines[8] = {"AF", "LH", "EY", "DL", "KL", "BA", "AY", "EY"};

TowerSimulation::TowerSimulation(int argc, char **argv) : help{(argc > 1) && (std::string{argv[1]} == "--help"s || std::string{argv[1]} == "-h"s)}
{
    MediaPath::initialize(argv[0]);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    GL::init_gl(argc, argv, "Airport Tower Simulation");
    // TASK_1 -
    aircraft_manager = std::make_unique<AircraftManager>();

    create_keystrokes();

    // TASK_1 - C.3)
    // GL::move_queue.emplace(&aircraft_manager);
}

TowerSimulation::~TowerSimulation()
{
    delete airport;
}
/*
 void TowerSimulation::create_aircraft(const AircraftType &type) const
 {
     assert(airport); // make sure the airport is initialized before creating aircraft

     const std::string flight_number = airlines[std::rand() % 8] + std::to_string(1000 + (rand() % 9000));
     const float angle = (rand() % 1000) * 2 * 3.141592f / 1000.f; // random angle between 0 and 2pi
     const Point3D start = Point3D{std::sin(angle), std::cos(angle), 0} * 3 + Point3D{0, 0, 2};
     const Point3D direction = (-start).normalize();

     Aircraft *aircraft = new Aircraft{type, flight_number, start, direction, airport->get_tower()};
     // TASK_0 - C.4)
     // GL::display_queue.emplace_back(aircraft);
     GL::move_queue.emplace(aircraft);
     std::cout << "CREATE_AIRCRAFT" << std::endl;
 }*/

void TowerSimulation::create_random_aircraft() const
{
    // create_aircraft(*(aircraft_types[rand() % 3]));
    // TASK_1 - A
    aircraft_manager->add_aircraft(aircraft_factory->create_aircraft(airport->get_tower()));
}

void TowerSimulation::show_airline(unsigned i)
{
    assert(i < airlines.size());
    auto airline = airlines[i];
    int count = aircraft_manager->count(airline);
    std::cout << count << " aircrafts for " << airline << std::endl;
}

// CONST A RETIRER
void TowerSimulation::create_keystrokes()
{
    GL::keystrokes.emplace('x', []()
                           { GL::exit_loop(); });
    GL::keystrokes.emplace('q', []()
                           { GL::exit_loop(); });
    // TASK_0 - C.3)
    /* GL::keystrokes.emplace('c', [this]()
                           { create_random_aircraft(); });*/
    // TASK_1 - C.4)
    GL::keystrokes.emplace('c', [this]()
                           { create_random_aircraft(); });

    GL::keystrokes.emplace('+', []()
                           { GL::change_zoom(0.95f); });
    GL::keystrokes.emplace('-', []()
                           { GL::change_zoom(1.05f); });
    GL::keystrokes.emplace('f', []()
                           { GL::toggle_fullscreen(); });
    // TASK_0 - C.2)
    GL::keystrokes.emplace('a', []()
                           { GL::ticks_per_sec += 1u; });
    GL::keystrokes.emplace('d', []()
                           { GL::ticks_per_sec =
                                 std::max(GL::ticks_per_sec - 1u, 1u); });
    GL::keystrokes.emplace('p', []()
                           { GL::is_paused = !GL::is_paused; });

    // TASK_2 - B.2)
    for (auto i = 0; i < (int)airlines.size(); i++)
    {
        GL::keystrokes.emplace('0' + i, [this, i]()
                               { show_airline(i); });
    }
    // TASK_3 - 2)
    GL::keystrokes.emplace('m', [this]()
                           { aircraft_manager->show_crash_amount(); });
}

void TowerSimulation::display_help() const
{
    std::cout << "This is an airport tower simulator" << std::endl
              << "the following keysstrokes have meaning:" << std::endl;
    /*
    for (const auto &ks_pair : GL::keystrokes)
    {
        std::cout << ks_pair.first << ' ';
    }*/
    // TASK_2 - A
    for (const auto &[key, function] : GL::keystrokes)
    {
        std::cout << key << ' ';
    }
    std::cout << std::endl;
}

void TowerSimulation::init_airport()
{
    airport = new Airport{
        one_lane_airport,
        Point3D{0, 0, 0},
        new img::Image{one_lane_airport_sprite_path.get_full_path()},
        *aircraft_manager};

    // TASK_0 - C.4)
    // GL::display_queue.emplace_back(airport);
    GL::move_queue.emplace(airport);
}

void TowerSimulation::launch()
{
    if (help)
    {
        display_help();
        return;
    }

    init_airport();

    // TASK_1 - D
    // init_aircraft_types();
    aircraft_factory = std::make_unique<AircraftFactory>();

    GL::loop();
}
