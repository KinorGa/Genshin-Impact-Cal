// Author: Lin
// Date: 2025-12-27 09:48:46
// Description:

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <Qvector>

namespace GiCal {
  constexpr int MAX_COLUMN=30;

  /*
      0   1   2   3   4   5   6  7  8  9   10    11    12  13    14     15     16
    ATK ATK% HP HP% DEF DEF% CR CD EM ER Anemo Pyro Hydro Geo Electro Dendro Cryo
  */
  const QVector<int> relic2buffer={
    1, 6, 3, 7, 5, 8, 9, 10, 12, 11, 
    18, 19, 20, 21, 22, 23, 24,
  };
  const QVector<double> statvalues={
    19.45, 5.83, 298.75, 5.83, 23.15, 7.29, 3.89, 7.77, 23.31, 6.48
  };
  const QVector<double> fullvalues={
    311., 46.6, 4780., 46.6, 0, 58.3, 31.1, 62.2, 186.5, 51.8,
    46.6, 46.6, 46.6, 46.6, 46.6, 46.6, 46.6
  };
  const QVector<QString> statnames={
    "ATK", "ATK%", "HP", "HP%", "DEF", "DEF%", "CR", "CD", "EM", "ER", "DMG DEALT",
  }; 

  const QVector<QString> showheaders={
    "ATK", "HP", "DEF", "ATK%", "HP%", "DEF%", "EM", "ER", "CR", "CD", "DMG DEALT", "FINAL DMG"
  };
}

#endif