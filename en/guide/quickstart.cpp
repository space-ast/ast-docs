// hello_ast.cpp
#include "ast/DateTime.hpp"
#include "ast/JulianDate.hpp"
#include <iostream>

int main() {
    AST_USING_NAMESPACE

    // Build a Julian date from a calendar date
    JulianDate jd = JulianDate::FromDateTime(2025, 1, 1, 0, 0, 0.0);
    std::cout << "JD: " << jd.impreciseDay() << std::endl;

    // Convert the Julian date back to a date-time
    DateTime dt = DateTime::FromJD(jd);
    std::cout << "DateTime: " << dt.toString() << std::endl;

    return 0;
}
