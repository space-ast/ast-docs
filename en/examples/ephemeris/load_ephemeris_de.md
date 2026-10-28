# Reading JPL DE ephemerides

This example shows how to use `JplDe` to read JPL DE binary ephemerides (DE440, DE441, etc.) and obtain the ephemeris time interval and body position and velocity.


## Include the headers

Use the `JplDe` class to read JPL DE ephemeris data files.

```cpp title="load_ephemeris_de.cpp"
#include "ast/JplDe.hpp"
#include "ast/TimePoint.hpp"
#include "ast/TimeInterval.hpp"
#include "ast/Vector.hpp"
#include <iostream>
#include <iomanip>
#include <clocale>

AST_USING_NAMESPACE
```

## Create a JplDe instance and load an ephemeris file

`open()` takes the ephemeris file path and returns an error code

```cpp title="load_ephemeris_de.cpp" indent

// Create a JplDe instance and load a JPL DE ephemeris file (replace with the actual path)
JplDe de;
errc_t err = de.open("./data/SolarSystem/plneph.430");
if (err != eNoError) {
    std::cerr << "Failed to load the DE ephemeris file" << std::endl;
    return -1;
}
```

## Get the ephemeris time interval

```cpp title="load_ephemeris_de.cpp" indent
// Get the ephemeris time interval
TimeInterval interval;
de.getInterval(interval);
std::cout << "ephemeris time span: " << std::endl;
std::cout << "  start: " << interval.getStart().toString() << std::endl;
std::cout << "  end:   " << interval.getStop().toString() << std::endl;
```

## Query body position and velocity

`getPosVelICRF()` returns the position and velocity of the target body relative to the reference body, in the ICRF frame centered on the reference body, with units of m and m/s.

```cpp title="load_ephemeris_de.cpp" indent
// Query the body state at 2000-01-01 12:00:00 UTC
TimePoint time = TimePoint::FromUTC(2000, 1, 1, 12, 0, 0.0);

Vector3d pos, vel;

// Position and velocity of the Earth relative to the solar system barycenter
err = de.getPosVelICRF(time, JplDe::eEarth, JplDe::eSSBarycenter, pos, vel);
if (err != eNoError) {
    std::cerr << "Failed to get the Earth position and velocity, error code: " << err << std::endl;
} else {
    std::cout << "\nEarth state relative to the solar system barycenter (ICRF):" << std::endl;
    std::cout << "  position [m]:   " << std::setprecision(6)
              << pos.x() << "  " << pos.y() << "  " << pos.z() << std::endl;
    std::cout << "  velocity [m/s]: " << std::setprecision(6)
              << vel.x() << "  " << vel.y() << "  " << vel.z() << std::endl;
}
```

## Compute the Earth-Moon distance

```cpp title="load_ephemeris_de.cpp" indent
// Position of the Moon relative to the Earth, used to compute the Earth-Moon distance
err = de.getPosVelICRF(time, JplDe::eMoon, JplDe::eEarth, pos, vel);
if (err != eNoError) {
    std::cerr << "Failed to get the Moon position and velocity, error code: " << err << std::endl;
} else {
    std::cout << "\nMoon position relative to the Earth [m]:" << std::endl;
    std::cout << "  X: " << std::setprecision(6) << pos.x() << std::endl;
    std::cout << "  Y: " << std::setprecision(6) << pos.y() << std::endl;
    std::cout << "  Z: " << std::setprecision(6) << pos.z() << std::endl;
    std::cout << "  Earth-Moon distance: " << std::setprecision(6) << pos.norm() << " m" << std::endl;
}
```

## Close the ephemeris file

```cpp title="load_ephemeris_de.cpp" indent
de.close();
```

Full code: [load_ephemeris_de.cpp](load_ephemeris_de.cpp)
