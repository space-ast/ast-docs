// hello_ast.cpp
#include "ast/DateTime.hpp"
#include "ast/JulianDate.hpp"
#include <iostream>

int main() {
    AST_USING_NAMESPACE

    // 根据日期创建儒略日
    JulianDate jd = JulianDate::FromDateTime(2025, 1, 1, 0, 0, 0.0);
    std::cout << "JD: " << jd.impreciseDay() << std::endl;

    // 将儒略日转回日期时间
    DateTime dt = DateTime::FromJD(jd);
    std::cout << "DateTime: " << dt.toString() << std::endl;

    return 0;
}