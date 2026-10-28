# Time system conversion

## Overview

Aerospace missions commonly use six time systems: UTC, TAI, TT, TDB, UT1 and GPS. Their relations are determined by constant offsets, accumulated leap seconds and periodic terms:

```
TAI = UTC + ΔAT     (accumulated leap seconds, 37 s in 2024)
TT  = TAI + 32.184s (constant offset)
TDB = TT  + P(t)    (periodic term, amplitude about 1.6 ms)
GPS = TAI - 19s     (constant offset)
UT1 = UTC + DUT1    (Earth rotation correction, |DUT1| < 0.9 s)
```

This example converts between the three time representations TimePoint, JulianDate and DateTime, and between the six time systems.

## Example

@[code](time-system-convert.cpp)

## Notes

- TimePoint stores time relative to TAI internally; conversions to other systems are applied as offsets.
- UTC ↔ TAI conversion depends on the leap second table; the library ships leap second data through the end of 2016.
- The periodic term in TDB ↔ TT is computed from an analytical formula and has an amplitude of about 1.6 ms.
- JulianDate uses (int day, double second) to preserve precision and avoid the rounding error of a single double.
- For long time spans, prefer TimePoint over DateTime for better precision.
