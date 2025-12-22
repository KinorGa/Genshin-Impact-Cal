// Author: Lin
// Date: 2025-12-21 15:03:54
// Description:

/*
  flower, plume, sands, goblet, circlet Stats
  for one item, Main intersetion with Sub is 0, 
  the alternate bits are (Main | Sub) & MainMask for Main,
  the alternate bits are (Main | Sub) & SubMask for Sub.
  So I restore the whole Main and Sub states with array of unsigned long long by id.
*/

#ifndef STATES_H
#define  STATES_H

#include <QObject>
#include <QVector>

using uT = unsigned long long;
class States: public QObject{
  Q_OBJECT
  // Q_PROPERTY(uT relicState READ relicState WRITE setRelicState NOTIFY relicStateChanged)

public:
  explicit States(QObject *parent = nullptr);

  Q_INVOKABLE uT relicState(int i) const;
  Q_INVOKABLE void setRelicState(int i, uT v);
  Q_INVOKABLE uT getState(int i);
  Q_INVOKABLE void restState();

signals:
  // void relicStateChanged();

private:
  QVector<uT> MainStates;
  QVector<uT> SubStates;
  QVector<uT> MainMasks;
  QVector<uT> SubMasks;
  QVector<uT> states;
};

#endif