/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/*
 * File:   Sigmoid.cpp
 * Author: ltsach
 *
 * Created on August 25, 2024, 2:45 PM
 */

#include "layer/Sigmoid.h"
#include "sformat/fmt_lib.h"
#include "ann/functions.h"

Sigmoid::Sigmoid(string name)
{
    if (trim(name).size() != 0)
        m_sName = name;
    else
        m_sName = "Sigmoid_" + to_string(++m_unLayer_idx);
}

Sigmoid::Sigmoid(const Sigmoid &orig)
{
    m_sName = "Sigmoid_" + to_string(++m_unLayer_idx);
}

Sigmoid::~Sigmoid()
{
}
xt::xarray<double> Sigmoid::forward(xt::xarray<double> X)
{
    // YOUR CODE IS HERE
    X = -X;
    auto exponential = xt::exp(X);
    xt::xarray<double> tempXarr = exponential;
    auto res = 1.0 / (1.0 + tempXarr);
    xt::xarray<double> Y = res;
    m_aCached_Y = Y;
    //Print Test
    //cout << "Sigmoid Forward: " << Y << endl;
    return Y;
}
xt::xarray<double> Sigmoid::backward(xt::xarray<double> DY)
{
    // YOUR CODE IS HERE
    auto res = DY * this->m_aCached_Y * (1.0 - this->m_aCached_Y);
    xt::xarray<double> returnXarr = res;
    //Print Test
    //cout << "Sigmoid Backward: " << returnXarr << endl;
    return returnXarr;
}

string Sigmoid::get_desc()
{
    string desc = fmt::format("{:<10s}, {:<15s}:",
                              "Sigmoid", this->getname());
    return desc;
}