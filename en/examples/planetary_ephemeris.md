# Reading planetary ephemerides

## Overview

SpaceAST can read planetary ephemeris data in two formats:
- JPL DE ephemerides (DE440, DE441, etc.)
- SPICE SPK ephemeris files (`.bsp`)

## Reading JPL DE ephemerides

Use the `JplDe` class to read JPL DE ephemeris files.

### Features

- Supports DE-series ephemeris files
- Reads both position and velocity
- Reads nutation angles and angles related to lunar libration
- Thread-safe data cache, supporting parallel reads from multiple threads

### Example

@[code](planetary_ephemeris_de.cpp)

### Supported bodies

| Body | Enum value |
|------|------------|
| Mercury system barycenter | `JplDe::eMercury` |
| Venus system barycenter | `JplDe::eVenus` |
| Earth | `JplDe::eEarth` |
| Mars system barycenter | `JplDe::eMars` |
| Jupiter system barycenter | `JplDe::eJupiter` |
| Saturn system barycenter | `JplDe::eSaturn` |
| Uranus system barycenter | `JplDe::eUranus` |
| Neptune system barycenter | `JplDe::eNeptune` |
| Pluto system barycenter | `JplDe::ePluto` |
| Moon | `JplDe::eMoon` |
| Sun | `JplDe::eSun` |
| Solar system barycenter | `JplDe::eSSBarycenter` |
| Earth-Moon barycenter | `JplDe::eEMBarycenter` |

## Reading SPK ephemerides

Use the `SPKParser` class to read SPICE SPK ephemeris kernels.

### Features

- Supports the SPICE SPK format (`.bsp` files)
- Reads the binary file directly, with no dependency on CSPICE

### Example

@[code](planetary_ephemeris_spk.cpp)

### NAIF body IDs

SPK files identify bodies by NAIF ID:
- 0: Solar system barycenter
- 1: Mercury system barycenter
- 2: Venus system barycenter
- 3: Earth-Moon barycenter
- 4: Mars system barycenter
- 5: Jupiter system barycenter
- 6: Saturn system barycenter
- 7: Uranus system barycenter
- 8: Neptune system barycenter
- 9: Pluto system barycenter
- 10: Sun
- 199: Mercury
- 299: Venus
- 399: Earth
- 301: Moon
- 499: Mars
- 599: Jupiter
- 699: Saturn
- 799: Uranus
- 899: Neptune
- 999: Pluto

### Notes

- SPK files are available from [NAIF](https://naif.jpl.nasa.gov/pub/naif/generic_kernels/spk/).
- Only some SPK data types are supported for now.
- Returned SPK data is in its native coordinates with no frame conversion applied. Check the documentation of the ephemeris file you use to confirm its reference frame.
- For multithreaded use, create one `SPKParser` instance per thread.
