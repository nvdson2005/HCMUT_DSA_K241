/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/*
 * File:   Softmax.cpp
 * Author: ltsach
 *
 * Created on August 25, 2024, 2:46 PM
 */

#include "layer/Softmax.h"
#include "ann/functions.h"
#include "sformat/fmt_lib.h"
#include <filesystem> //require C++17
namespace fs = std::filesystem;

Softmax::Softmax(int axis, string name) : m_nAxis(axis)
{
    if (trim(name).size() != 0)
        m_sName = name;
    else
        m_sName = "Softmax_" + to_string(++m_unLayer_idx);
}

Softmax::Softmax(const Softmax &orig)
{
}

Softmax::~Softmax()
{
}

xt::xarray<double> Softmax::forward(xt::xarray<double> X)
{
    // YOUR CODE IS HERE
    m_aCached_Y = softmax(X, m_nAxis);
    return m_aCached_Y;
}
xt::xarray<double> Softmax::backward(xt::xarray<double> DY)
{
    // YOUR CODE IS HERE
    if (DY.dimension() == 1)
    {
        xt::xarray<double> diagY = xt::diag(m_aCached_Y);
        xt::xarray<double> transposeY = xt::transpose(m_aCached_Y);
        xt::xarray<double> outerProductYtY = xt::linalg::outer(transposeY, m_aCached_Y);
        xt::xarray<double> jacobian = diagY - outerProductYtY;
        xt::xarray<double> gradientZ = xt::linalg::dot(jacobian, DY);
        return gradientZ;
    }
    else
    {
        xt::xarray<double> DZ = xt::zeros_like(DY);
        for (std::size_t i = 0; i < DY.shape()[0]; i++)
        {
            xt::xarray<double> y = xt::row(m_aCached_Y, i);
            xt::xarray<double> diagY = xt::diag(y);
            xt::xarray<double> outerProductY = xt::linalg::outer(y, xt::transpose(y));
            xt::xarray<double> jacobian = diagY - outerProductY;
            xt::xarray<double> dy = xt::row(DY, i);
            xt::xarray<double> dz = xt::linalg::dot(jacobian, dy);
            xt::view(DZ, i) = dz;
        }
        return DZ;
    }
}

string Softmax::get_desc()
{
    string desc = fmt::format("{:<10s}, {:<15s}: {:4d}",
                              "Softmax", this->getname(), m_nAxis);
    return desc;
}
