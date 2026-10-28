// 本文件为 zh/examples/ephemeris/load_ephemeris_de.md 中的完整代码

#include "ast/JplDe.hpp"
#include "ast/TimePoint.hpp"
#include "ast/TimeInterval.hpp"
#include "ast/Vector.hpp"
#include <iostream>     // for std::cout, std::cerr, std::endl
#include <clocale>      // for std::setlocale

const char* locale = std::setlocale(LC_ALL, ".UTF-8");

AST_USING_NAMESPACE     // 引入 SpaceAST 的命名空间

int main()
{
    // 创建 JplDe 实例并加载 JPL DE 星历文件（请替换为实际文件路径）
    JplDe de;
    errc_t err = de.open("./data/SolarSystem/plneph.430");
    if (err != eNoError) {
        std::cerr << "无法加载DE星历文件" << std::endl;
        return -1;
    }

    // 获取星历时间区间
    TimeInterval interval;
    de.getInterval(interval);
    std::cout << "星历时间区间: " << interval.toString() << std::endl;

    Vector3d pos, vel;

    // 时刻: 2000-01-01 12:00:00 UTC 
    TimePoint time = TimePoint::FromUTC(2000, 1, 1, 12, 0, 0.0);
    // 计算地球相对太阳系质心的位置和速度
    err = de.getPosVelICRF(time, JplDe::eEarth, JplDe::eSSBarycenter, pos, vel);
    if (err != eNoError) {
        std::cerr << "获取地球位置速度失败" << std::endl;
    } else {
        std::cout << "\n地球相对太阳系质心的状态 (ICRF):" << std::endl;
        std::cout << "  位置 [m]:   " << toString(pos) << std::endl;
        std::cout << "  速度 [m/s]: " << toString(vel) << std::endl;
    }

    // 计算月球相对地球的位置，并计算地月距离
    err = de.getPosVelICRF(time, JplDe::eMoon, JplDe::eEarth, pos, vel);
    if (err != eNoError) {
        std::cerr << "获取月球位置速度失败，错误码: " << err << std::endl;
    } else {
        std::cout << "\n月球相对地球的位置 [m]:" << toString(pos) << std::endl;
        std::cout << "  地月距离: " << pos.norm() << " m" << std::endl;
    }

    de.close();

    return 0;
}
