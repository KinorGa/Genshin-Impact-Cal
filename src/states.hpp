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
#include <QVariant>
#include <QPair>
#include <yaml-cpp/yaml.h>
#include "fstream" // IWYU pragma: keep

using uT = unsigned long long;
class States: public QObject{
  Q_OBJECT
  // Q_PROPERTY(uT relicState READ relicState WRITE setRelicState NOTIFY relicStateChanged)

public:
  static const QMap<QString, int> names2i;
  static const QVector<QString> names;

  explicit States(QObject *parent = nullptr);

  Q_INVOKABLE uT relicState(int i) const;
  Q_INVOKABLE void setRelicState(int i, uT v);
  Q_INVOKABLE uT getState(int i);
  Q_INVOKABLE void restState();

  // Buffer Section
  Q_INVOKABLE uT getBufferSize(int i) const;
  Q_INVOKABLE QVariant getBufferData(int i, int j);
  Q_INVOKABLE QVariant getBuffer(int i);

  // config
  Q_INVOKABLE void saveYaml(QString path);
  Q_INVOKABLE void loadYaml(QString path);

  Q_INVOKABLE void clearBufferData(int i);
  Q_INVOKABLE void updateBufferData(int i, QString tag, double value);

  Q_INVOKABLE void test_generate();

signals:
  // void relicStateChanged();
  void loadBufferConfig();
  void loadRelicConfig();
  void updateBuffer();

  void sender(QVector<uT> states, QVector<double> buffers);

public slots:
  void receiveBuffer(int tagKey, QVector<QPair<QString, double>>data);

private:
  QVector<uT> MainStates;
  QVector<uT> SubStates;
  QVector<uT> MainMasks;
  QVector<uT> SubMasks;
  QVector<uT> states;
  QVector<QVector<QPair<QString, double>>> buffDatas;
};

#endif