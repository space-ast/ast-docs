# Reading SPK ephemerides

This example shows how to use `JplSpk` to read SPICE SPK binary ephemeris files (`.bsp`) and obtain the ephemeris time interval, the list of bodies in the file, and body position and velocity.

::: tip
This example depends on the CSPICE shared library. The library is loaded automatically on first use. If you see "SPICE library not loaded" at runtime, check whether the cspice library exists in the build output directory, or specify the library path through the `CSPICE_LIBS` configuration.
:::

## Include the headers

Use the `JplSpk` class to read SPK ephemeris data files.

```cpp title="load_ephemeris_spk.cpp"
#include "ast/JplSpk.hpp"
#include "ast/TimePoint.hpp"
#include "ast/TimeInterval.hpp"
#include "ast/Vector.hpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

AST_USING_NAMESPACE
```

## Create a JplSpk instance and load an ephemeris file

`open()` takes the SPK file path and returns an error code. Opening the same path again closes the old file first; `close()` can be called automatically by the destructor.

```cpp title="load_ephemeris_spk.cpp" indent

// Create a JplSpk instance and load an SPK ephemeris file (replace with the actual path)
JplSpk spk;
errc_t err = spk.open("./data/kernels/spk/de430.bsp");
if (err != eNoError) {
    std::cerr << "Failed to load the SPK ephemeris file" << std::endl;
    return -1;
}
```

## Get the ephemeris time interval

The first argument of `getInterval()` is the target NAIF ID, and it returns the time interval covered for that target in the file. The first call parses the file and caches the result.

```cpp title="load_ephemeris_spk.cpp" indent
// Get the ephemeris time interval for the Earth (NAIF ID 399)
TimeInterval interval;
err = spk.getInterval(399, interval);
if (err != eNoError) {
    std::cerr << "Failed to get the ephemeris time interval, error code: " << err << std::endl;
} else {
    std::cout << "ephemeris time span: " << std::endl;
    std::cout << "  start: " << interval.getStart().toString() << std::endl;
    std::cout << "  end:   " << interval.getStop().toString() << std::endl;
}
```

## List the bodies in the file

`getBodyIds()` and `getBodyNames()` walk the SPK segment descriptors and return the target bodies that appear in the file along with their names. Names are converted to readable form through CSPICE's `bodc2n`; unknown IDs are shown as `NAIF ID <id>`.

```cpp title="load_ephemeris_spk.cpp" indent
// List the bodies in the file
std::vector<std::string> names;
err = spk.getBodyNames(names);
if (err != eNoError) {
    std::cerr << "Failed to get the body list, error code: " << err << std::endl;
} else {
    std::cout << "\nbodies in the file:" << std::endl;
    for (const auto& name : names) {
        std::cout << "  " << name << std::endl;
    }
}
```

## Query body position and velocity

`getPosVelICRF()` returns the position and velocity of the target body relative to the reference body, in the ICRF frame centered on the reference body (`J2000` in CSPICE), with units of m and m/s.


```cpp title="load_ephemeris_spk.cpp" indent
// Query the body state at 2000-01-01 12:00:00 UTC
TimePoint time = TimePoint::FromUTC(2000, 1, 1, 12, 0, 0.0);

Vector3d pos, vel;

// Position and velocity of the Earth relative to the Earth-Moon barycenter (NAIF ID 3)
err = JplSpk::getPosVelICRF(time, 399, 3, pos, vel);
if (err != eNoError) {
    std::cerr << "Failed to get the Earth position and velocity, error code: " << err << std::endl;
} else {
    std::cout << "\nEarth state relative to the Earth-Moon barycenter (ICRF):" << std::endl;
    std::cout << "  position [m]:   " << std::setprecision(6)
              << pos.x() << "  " << pos.y() << "  " << pos.z() << std::endl;
    std::cout << "  velocity [m/s]: " << std::setprecision(6)
              << vel.x() << "  " << vel.y() << "  " << vel.z() << std::endl;
}
```

## Compute the Earth-Moon distance

In the same file, the reference body for both the Earth and the Moon is the Earth-Moon barycenter, so subtracting the Earth position from the Moon position gives the Moon position relative to the Earth.

```cpp title="load_ephemeris_spk.cpp" indent
// Position of the Moon relative to the Earth-Moon barycenter; subtract the Earth position to get the Moon position relative to the Earth
Vector3d earthPos, earthVel;
Vector3d moonPos, moonVel;

errc_t errEarth = JplSpk::getPosVelICRF(time, 399, 3, earthPos, earthVel);
errc_t errMoon = JplSpk::getPosVelICRF(time, 301, 3, moonPos, moonVel);
if (errEarth != eNoError || errMoon != eNoError) {
    std::cerr << "Failed to get the Earth and Moon position and velocity, error codes: " << errEarth << ", " << errMoon << std::endl;
} else {
    Vector3d moonRelEarth = moonPos - earthPos;
    std::cout << "\nMoon position relative to the Earth [m]:" << std::endl;
    std::cout << "  X: " << std::setprecision(6) << moonRelEarth.x() << std::endl;
    std::cout << "  Y: " << std::setprecision(6) << moonRelEarth.y() << std::endl;
    std::cout << "  Z: " << std::setprecision(6) << moonRelEarth.z() << std::endl;
    std::cout << "  Earth-Moon distance: " << std::setprecision(6) << moonRelEarth.norm() << " m" << std::endl;
}
```

## Close the ephemeris file

```cpp title="load_ephemeris_spk.cpp" indent
spk.close();
```

Full code: [load_ephemeris_spk.cpp](load_ephemeris_spk.cpp)
