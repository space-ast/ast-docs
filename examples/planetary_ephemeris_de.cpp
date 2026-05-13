/// @file      planetary_ephemeris_de.cpp
/// @brief     JPL DE星历读取示例
/// @author    ast团队
/// @date      2026-05-13

#include "AstCore/JplDe.hpp"
#include "AstCore/TimePoint.hpp"
#include "AstCore/TimeInterval.hpp"
#include "AstCore/Vector.hpp"
#include "AstCore/JulianDate.hpp"
#include "AstCore/DateTime.hpp"
#include <iostream>
#include <iomanip>
#include <clocale>

AST_USING_NAMESPACE

int main()
{
    // 设置UTF-8编码，确保中文正确显示
    setlocale(LC_ALL, ".UTF-8");
    
    // 创建JplDe对象
    JplDe de;

    // 打开DE星历文件（请替换为实际文件路径）
    errc_t err = de.open("./data/SolarSystem/plneph.430");
    if (err != eNoError) {
        std::cerr << "无法打开DE星历文件" << std::endl;
        std::cerr << "错误码: " << err << std::endl;
        return -1;
    }

    std::cout << "成功打开DE星历文件" << std::endl;
    std::cout << "星历版本: DE" << de.getEphemVersion() << std::endl;

    // 获取星历时间区间
    TimeInterval interval;
    de.getInterval(interval);
    std::cout << "星历时间范围: " << std::endl;
    std::cout << "  开始时间: " << interval.getStart().toString() << std::endl;
    std::cout << "  结束时间: " << interval.getStop().toString() << std::endl;

    // 创建一个时间点（J2000.0 UTC时刻）
    TimePoint time = TimePoint::FromUTC(2000, 1, 1, 12, 0, 0.0);
    
    // 获取地球相对于太阳系质心的位置和速度
    Vector3d pos, vel;
    err = de.getPosVelICRF(
        time, 
        JplDe::eEarth, 
        JplDe::eSSBarycenter, 
        pos, 
        vel
    );

    if (err == eNoError) {
        std::cout << "\n2000年1月1日 12:00:00 UTC 时刻:" << std::endl;
        std::cout << "地球相对太阳系质心的位置 (m):" << std::endl;
        std::cout << "  X: " << std::setprecision(6) << pos.x() << std::endl;
        std::cout << "  Y: " << std::setprecision(6) << pos.y() << std::endl;
        std::cout << "  Z: " << std::setprecision(6) << pos.z() << std::endl;
        std::cout << "地球相对太阳系质心的速度 (m/s):" << std::endl;
        std::cout << "  Vx: " << std::setprecision(6) << vel.x() << std::endl;
        std::cout << "  Vy: " << std::setprecision(6) << vel.y() << std::endl;
        std::cout << "  Vz: " << std::setprecision(6) << vel.z() << std::endl;
    } else {
        std::cerr << "获取位置速度失败，错误码: " << err << std::endl;
    }

    // 获取月球相对于地球的位置
    err = de.getPosVelICRF(
        time, 
        JplDe::eMoon, 
        JplDe::eEarth, 
        pos, 
        vel
    );

    if (err == eNoError) {
        std::cout << "\n月球相对地球的位置 (m):" << std::endl;
        std::cout << "  X: " << std::setprecision(6) << pos.x() << std::endl;
        std::cout << "  Y: " << std::setprecision(6) << pos.y() << std::endl;
        std::cout << "  Z: " << std::setprecision(6) << pos.z() << std::endl;
        
        // 计算地月距离
        double distance = pos.norm();
        std::cout << "地月距离: " << std::setprecision(6) << distance << " m" << std::endl;
    }

    // 获取章动角
    double nutLong, nutObl;
    err = de.getNutation(time, nutLong, nutObl);
    if (err == eNoError) {
        std::cout << "\n章动角 (rad):" << std::endl;
        std::cout << "  黄经章动: " << std::setprecision(10) << nutLong << std::endl;
        std::cout << "  倾角章动: " << std::setprecision(10) << nutObl << std::endl;
    }
    
    // 获取月球天平动相关角度
    Vector3d libration;
    err = de.getLibration(time, libration);
    if (err == eNoError) {
        std::cout << "\n月球天平动相关角度 (rad):" << std::endl;
        std::cout << "  进动角 (omega): " << std::setprecision(10) << libration.x() << std::endl;
        std::cout << "  章动角 (i): " << std::setprecision(10) << libration.y() << std::endl;
        std::cout << "  自转角 (u): " << std::setprecision(10) << libration.z() << std::endl;
    }

    // 关闭星历文件
    de.close();

    std::cout << "\n示例程序执行完毕" << std::endl;

    return 0;
}