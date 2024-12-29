/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/*
 * File:   CrossEntropy.cpp
 * Author: ltsach
 *
 * Created on August 25, 2024, 2:47 PM
 */

#include "loss/CrossEntropy.h"
#include "ann/functions.h"

CrossEntropy::CrossEntropy(LossReduction reduction) : ILossLayer(reduction)
{
}

CrossEntropy::CrossEntropy(const CrossEntropy &orig) : ILossLayer(orig)
{
}

CrossEntropy::~CrossEntropy()
{
}

double CrossEntropy::forward(xt::xarray<double> X, xt::xarray<double> t)
{
    // YOUR CODE IS HERE
    bool mean_reduced = (m_eReduction == REDUCE_MEAN);
    m_aYtarget = t;
    m_aCached_Ypred = X;
    if (t.dimension() == 1 && X.dimension() > 1)
    {
        xt::xarray<unsigned long> tNew = xt::cast<unsigned long>(t);
        return cross_entropy(X, tNew, mean_reduced);
    }
    else
    {
        return cross_entropy(X, t, mean_reduced);
    }
}
xt::xarray<double> CrossEntropy::backward()
{
    double EPSILON = 1e-7;
    // YOUR CODE IS HERE
    bool mean_reduced = (m_eReduction == REDUCE_MEAN);
    double nNorm;
    if (mean_reduced)
    {
        nNorm = this->m_aCached_Ypred.shape()[0];
    }
    else
    {
        nNorm = 1.0;
    }
    xt::xarray<double> yGradient;
    if (m_aYtarget.dimension() == 1 && m_aCached_Ypred.dimension() > 1)
    {
        xt::xarray<unsigned long> tNew = xt::cast<unsigned long>(m_aYtarget);
        tNew = onehot_enc(tNew, m_aCached_Ypred.shape()[1]);
        yGradient = (-1.0 / nNorm) * (tNew / (m_aCached_Ypred + EPSILON));
    }
    else
    {
        yGradient = (-1.0 / nNorm) * (m_aYtarget / (m_aCached_Ypred + EPSILON));
    }
    return yGradient;
}