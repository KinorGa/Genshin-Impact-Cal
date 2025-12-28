// Author: Lin
// Date: 2025-12-26 09:18:55
// Description: the target is to sort and filter data in inplace.

#include "src/tableModel.hpp"
#include "src/tools.hpp"
#include <QtConcurrent>
#include <QElapsedTimer>
#include <execution>

GiTableModel::GiTableModel(QObject *parent): QAbstractTableModel(parent)
{
  m_rawData.resize(100000);
  for (int i = 0; i < 100000; ++i) {
    auto& row = m_rawData[i];
    for (int j = 0; j < MAX_COLUMN; ++j) {
        row[j] = i * MAX_COLUMN + j; 
    }m_filteredRowIndices.append(i);
  }
  for(int i=0; i<nCol; ++i){
    m_filteredColIndices.append(i);
    m_colOrders.append(0);
  }
  mRow=m_rawData.size();

  for(int i=0; i<nCol; ++i){
    m_headers.append(QString("Column %1").arg(i));
  }
}

int GiTableModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return mRow;
}

int GiTableModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return nCol;
}

QVariant GiTableModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || role != Qt::DisplayRole) {
    return QVariant();
  }
  int row = index.row();
  int col = index.column();
  if (row < 0 || row >= m_rawData.size() || col < 0 || col >= nCol) {
    return QVariant();
  }
  return m_rawData[m_filteredRowIndices[row]][m_filteredColIndices[col]];
}

QVariant GiTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }
  if (orientation == Qt::Horizontal) {
    switch (m_colOrders[section]%3) {
      case 2:
        return m_headers[m_filteredColIndices[section]] + " ↑";
      case 1:
        return m_headers[m_filteredColIndices[section]] + " ↓";
      case 0:
        return m_headers[m_filteredColIndices[section]];
    };
  } else {
    return QString::number(section + 1).rightJustified(8, ' ');
  }
}

QHash<int, QByteArray> GiTableModel::roleNames() const {
  return {{Qt::DisplayRole, "display"}};
}

void GiTableModel::filter1() {
  beginResetModel();
  m_filteredColIndices.clear();
  m_filteredColIndices.append(0);
  m_filteredColIndices.append(1);
  m_filteredColIndices.append(2);
  m_filteredColIndices.append(3);
  m_filteredColIndices.append(4);
  m_filteredColIndices.append(5);
  m_filteredColIndices.append(6);
  m_filteredColIndices.append(7);
  nCol = m_filteredColIndices.size();
  endResetModel();
}

void GiTableModel::filter2(){
  beginResetModel();
  m_filteredColIndices.clear();
  m_filteredColIndices.append(0);
  m_filteredColIndices.append(1);
  m_filteredColIndices.append(3);
  m_filteredColIndices.append(4);
  m_filteredColIndices.append(6);
  nCol = m_filteredColIndices.size();
  endResetModel();
}

void GiTableModel::resetData(){
  beginResetModel();
  m_filteredColIndices.clear();
  for(int i=0; i<29; ++i){
    m_filteredColIndices.append(i);
  }
  nCol = m_filteredColIndices.size();
  endResetModel();
}

void GiTableModel::receive_state(QVector<uT> states, QVector<double> buffers){
  // process states to filter data
  qDebug() << "Received states " << states;

  // Run in background to avoid blocking UI
  QtConcurrent::run(
    [this, states, buffers]() {
    QVector<std::array<double, MAX_COLUMN>> tempData;
    QElapsedTimer timer;
    timer.start();
    tempData = calculate(states, buffers, 25);
    
    qDebug() << "Data calculation took" << timer.elapsed() << "ms";
    timer.restart();
    //  if original data size is not equal to tempData size, clear and reset filtered indices
    if(m_rawData.size() != tempData.size()){
      m_filteredRowIndices.clear();
      m_filteredRowIndices.reserve(tempData.size());
      for(int i=0; i<tempData.size(); ++i){
        m_filteredRowIndices.append(i);
      }
    }
    qDebug() << "update rowIndices took" << timer.elapsed() << "ms";
    
    QMetaObject::invokeMethod(this, [this, tempData=std::move(tempData)]() {
      QElapsedTimer timer;
      beginResetModel();
      timer.start();
      
      // 后台处理析构旧数据
      QVector<std::array<double, MAX_COLUMN>> oldRawData = std::move(m_rawData);
                
      m_rawData = std::move(tempData);
      mRow = m_rawData.size();

      QtConcurrent::run([oldData = std::move(oldRawData)]() mutable {});
      qDebug() << "Data assignment took" << timer.elapsed() << "ms";
    
      endResetModel();
      qDebug() << "Data updated:" << mRow << "rows";
    }, Qt::QueuedConnection);
  });
}

void GiTableModel::sortByColumn(int column){
  if(column <0 || column >= nCol) return;
  int colIndex = m_filteredColIndices[column];
  int order=m_colOrders[colIndex];
  qDebug() << "start sort column " << colIndex;
  qDebug() << "m_filter row size " << m_filteredRowIndices.size();
  qDebug() << "m_rawData size " << m_rawData.size();
  qDebug() << "mRow " << mRow;
  qDebug() << "order " << order;

  // beginResetModel();
  // applySortFilter(colIndex, order);
  // endResetModel();

  QtConcurrent::run(
    [this, colIndex, order]()->void{
    QElapsedTimer timer;
    timer.restart();
    QVector<int> newFilteredRowIndices = this->m_filteredRowIndices;
    if(order%3==2){
      std::sort(std::execution::par, newFilteredRowIndices.begin(), newFilteredRowIndices.end());
    }
    else{
      std::sort(std::execution::par, newFilteredRowIndices.begin(), newFilteredRowIndices.end(), [this, colIndex, order](int a, int b) -> bool {
        bool cmp = m_rawData[a][colIndex] < m_rawData[b][colIndex];
        return order % 2 == 0 ? cmp : !cmp;
      });
    }
    qDebug() << "Sorting took" << timer.elapsed() << "ms";

    QMetaObject::invokeMethod(this, [this, newFilteredRowIndices=std::move(newFilteredRowIndices), colIndex]() {
      beginResetModel();
      m_filteredRowIndices = std::move(newFilteredRowIndices);
      m_colOrders[colIndex] += 1;
      endResetModel();
      qDebug() << "Sort applied on UI thread.";
    }, Qt::QueuedConnection);
  });
}

void GiTableModel::applySortFilter(int colIndex, int order){
  QElapsedTimer timer;
  timer.restart();
  if(order%3==2){
    std::sort(std::execution::par, m_filteredRowIndices.begin(), m_filteredRowIndices.end());
  }else{
    std::sort(std::execution::par, m_filteredRowIndices.begin(), m_filteredRowIndices.end(),
    [this, colIndex, order=m_colOrders[colIndex]](int a, int b)->bool {
      bool cmp=m_rawData[a][colIndex] < m_rawData[b][colIndex];
      return order % 2 == 0 ? cmp : !cmp;
    });
  }
  m_colOrders[colIndex] += 1;
  qDebug() << "Sorting took" << timer.elapsed() << "ms";
}