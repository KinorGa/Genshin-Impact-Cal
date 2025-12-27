// Author: Lin
// Date: 2025-12-27 09:33:10
// Description:

#ifndef TOOLS_HPP
#define TOOLS_HPP

#include "src/constants.hpp"
#include <QVector>
#include <array>

template<class T>
using vT = QVector<T>;
using uT = unsigned long long;

vT<int> getSubStates(const vT<uT> &states);
vT<int> getSubIndexs(const vT<uT> &states);
QVector<std::array<double, GiCal::MAX_COLUMN>> generate(const vT<uT> &states, int m=25);
QVector<std::array<double, GiCal::MAX_COLUMN>> calculate(const vT<uT> &ori_states, const vT<double> &ori_buffers, int m);

#endif