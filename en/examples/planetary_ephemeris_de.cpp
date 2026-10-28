#include "ast/JplDe.hpp"
#include "ast/TimePoint.hpp"
#include "ast/TimeInterval.hpp"
#include "ast/Vector.hpp"
#include "ast/JulianDate.hpp"
#include "ast/DateTime.hpp"
#include "ast/Euler.hpp"
#include <iostream>
#include <iomanip>
#include <clocale>

AST_USING_NAMESPACE

int main()
{
    // Use UTF-8 encoding
    setlocale(LC_ALL, ".UTF-8");

    // Create a JplDe object
    JplDe de;

    // Open a DE ephemeris file (replace with the actual path)
    errc_t err = de.open("./data/SolarSystem/plneph.430");
    if (err != eNoError) {
        std::cerr << "Failed to open the DE ephemeris file" << std::endl;
        std::cerr << "error code: " << err << std::endl;
        return -1;
    }

    std::cout << "Opened the DE ephemeris file" << std::endl;
    std::cout << "ephemeris version: DE" << de.getEphemVersion() << std::endl;

    // Get the ephemeris time span
    TimeInterval interval;
    de.getInterval(interval);
    std::cout << "ephemeris time span: " << std::endl;
    std::cout << "  start: " << interval.getStart().toString() << std::endl;
    std::cout << "  end:   " << interval.getStop().toString() << std::endl;

    // Create a time point (J2000.0 UTC)
    TimePoint time = TimePoint::FromUTC(2000, 1, 1, 12, 0, 0.0);

    // Get the Earth position and velocity relative to the solar system barycenter
    Vector3d pos, vel;
    err = de.getPosVelICRF(
        time,
        JplDe::eEarth,
        JplDe::eSSBarycenter,
        pos,
        vel
    );

    if (err == eNoError) {
        std::cout << "\nAt 2000-01-01 12:00:00 UTC:" << std::endl;
        std::cout << "Earth position relative to the solar system barycenter (m):" << std::endl;
        std::cout << "  X: " << std::setprecision(6) << pos.x() << std::endl;
        std::cout << "  Y: " << std::setprecision(6) << pos.y() << std::endl;
        std::cout << "  Z: " << std::setprecision(6) << pos.z() << std::endl;
        std::cout << "Earth velocity relative to the solar system barycenter (m/s):" << std::endl;
        std::cout << "  Vx: " << std::setprecision(6) << vel.x() << std::endl;
        std::cout << "  Vy: " << std::setprecision(6) << vel.y() << std::endl;
        std::cout << "  Vz: " << std::setprecision(6) << vel.z() << std::endl;
    } else {
        std::cerr << "Failed to get position and velocity, error code: " << err << std::endl;
    }

    // Get the Moon position relative to the Earth
    err = de.getPosVelICRF(
        time,
        JplDe::eMoon,
        JplDe::eEarth,
        pos,
        vel
    );

    if (err == eNoError) {
        std::cout << "\nMoon position relative to the Earth (m):" << std::endl;
        std::cout << "  X: " << std::setprecision(6) << pos.x() << std::endl;
        std::cout << "  Y: " << std::setprecision(6) << pos.y() << std::endl;
        std::cout << "  Z: " << std::setprecision(6) << pos.z() << std::endl;

        // Compute the Earth-Moon distance
        double distance = pos.norm();
        std::cout << "Earth-Moon distance: " << std::setprecision(6) << distance << " m" << std::endl;
    } else {
        std::cerr << "Failed to get the Moon position, error code: " << err << std::endl;
    }

    // Get the nutation angles
    double nutLong, nutObl;
    err = de.getNutation(time, nutLong, nutObl);
    if (err == eNoError) {
        std::cout << "\nNutation angles (rad):" << std::endl;
        std::cout << "  nutation in longitude: " << std::setprecision(10) << nutLong << std::endl;
        std::cout << "  nutation in obliquity: " << std::setprecision(10) << nutObl << std::endl;
    } else {
        std::cerr << "Failed to get the nutation angles, error code: " << err << std::endl;
    }

    // Get the angles related to lunar libration
    Euler libration;
    err = de.getLibration(time, libration);
    if (err == eNoError) {
        std::cout << "\nlunar libration angles (rad):" << std::endl;
        std::cout << "  precession angle (omega): " << std::setprecision(10) << libration.angle1() << std::endl;
        std::cout << "  nutation angle (i): " << std::setprecision(10) << libration.angle2() << std::endl;
        std::cout << "  rotation angle (u): " << std::setprecision(10) << libration.angle3() << std::endl;
    } else {
        std::cerr << "Failed to get the libration data, error code: " << err << std::endl;
    }

    // Close the ephemeris file
    de.close();

    std::cout << "\nExample finished" << std::endl;

    return 0;
}
