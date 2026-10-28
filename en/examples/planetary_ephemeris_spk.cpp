#include "ast/SPKParser.hpp"
#include "ast/Vector.hpp"
#include "ast/JulianDate.hpp"
#include <iostream>
#include <iomanip>
#include <clocale>

AST_USING_NAMESPACE

int main()
{
    // Use UTF-8 encoding
    setlocale(LC_ALL, ".UTF-8");

    // Create an SPK parser object
    SPKParser spkParser;

    // Open an SPK ephemeris file (replace with the actual path)
    std::string spkFilePath = "./data/Test/kernels/spk/de430.bsp";

    errc_t err = spkParser.parse(spkFilePath);
    if (err != eNoError) {
        std::cerr << "Failed to parse the SPK ephemeris file: " << spkFilePath << std::endl;
        std::cerr << "error code: " << err << std::endl;
    }

    std::cout << "Opened the SPK ephemeris file" << std::endl;

    // Print the file comment
    std::cout << "\n=== SPK file comment ===" << std::endl;
    spkParser.printComment();

    // Get the SPK segment descriptors
    const auto& descriptors = spkParser.getDescriptors();
    std::cout << "\n=== SPK segment information ===" << std::endl;
    std::cout << "number of segments: " << descriptors.size() << std::endl;

    for (size_t i = 0; i < descriptors.size(); ++i) {
        const auto& desc = descriptors[i];
        std::cout << "\nsegment " << (i + 1) << ":" << std::endl;
        std::cout << "  target ID: " << desc.target << std::endl;
        std::cout << "  center ID: " << desc.center << std::endl;
        std::cout << "  frame ID:  " << desc.frame << std::endl;
        std::cout << "  data type: " << desc.type << std::endl;
        std::cout << "  time span: " << desc.start_time << " to " << desc.end_time << " (ET seconds)" << std::endl;
    }

    // Use the J2000.0 TDB epoch (ET = 0) as the sample time
    double et = 0.0;  // J2000.0 TDB, i.e. 2000-01-01 12:00:00 TDB

    // Define a few common NAIF body IDs
    const int SUN_ID = 10;
    const int EARTH_ID = 399;
    const int MOON_ID = 301;
    const int MARS_BARYCENTER_ID = 4; // Mars system barycenter ID

    // Get the Earth position and velocity (relative to the Earth-Moon barycenter)
    Vector3d pos, vel;
    err = spkParser.getPosVelNative(et, EARTH_ID, pos, vel);

    if (err == eNoError) {
        std::cout << "\n=== Earth state at J2000.0 (relative to the Earth-Moon barycenter) ===" << std::endl;
        std::cout << "position (m):" << std::endl;
        std::cout << "  X: " << std::setprecision(8) << pos.x() << std::endl;
        std::cout << "  Y: " << std::setprecision(8) << pos.y() << std::endl;
        std::cout << "  Z: " << std::setprecision(8) << pos.z() << std::endl;
        std::cout << "velocity (m/s):" << std::endl;
        std::cout << "  Vx: " << std::setprecision(6) << vel.x() << std::endl;
        std::cout << "  Vy: " << std::setprecision(6) << vel.y() << std::endl;
        std::cout << "  Vz: " << std::setprecision(6) << vel.z() << std::endl;

        // Compute the distance and speed
        double distance = pos.norm() / 1000.0;  // convert to km
        double speed = vel.norm();              // m/s
        std::cout << "distance to the Earth-Moon barycenter: " << std::setprecision(6) << distance << " km" << std::endl;
        std::cout << "speed: " << std::setprecision(4) << speed << " m/s" << std::endl;
    } else {
        std::cerr << "Failed to get the Earth state, error code: " << err << std::endl;
    }

    // Get the Mars system barycenter position (relative to the solar system barycenter)
    err = spkParser.getPosNative(et, MARS_BARYCENTER_ID, pos);
    if (err == eNoError) {
        std::cout << "\n=== Mars system barycenter position at J2000.0 TDB (relative to the solar system barycenter) ===" << std::endl;
        std::cout << "position (m):" << std::endl;
        std::cout << "  X: " << std::setprecision(8) << pos.x() << std::endl;
        std::cout << "  Y: " << std::setprecision(8) << pos.y() << std::endl;
        std::cout << "  Z: " << std::setprecision(8) << pos.z() << std::endl;
    } else {
        std::cerr << "Failed to get the Mars system barycenter position, error code: " << err << std::endl;
    }

    // Find the SPK segment descriptor for a given target and time
    const SPK_Descriptor* desc = spkParser.findSpkDescriptor(EARTH_ID, et);
    if (desc != nullptr) {
        std::cout << "\n=== SPK segment for Earth at ET=" << et << " ===" << std::endl;
        std::cout << "target ID: " << desc->target << std::endl;
        std::cout << "center ID: " << desc->center << std::endl;
        std::cout << "frame ID:  " << desc->frame << std::endl;
        std::cout << "data type: " << desc->type << std::endl;
    } else {
        std::cout << "\nNo SPK segment found for Earth at ET=" << et << std::endl;
    }

    std::cout << "\nExample finished" << std::endl;

}
