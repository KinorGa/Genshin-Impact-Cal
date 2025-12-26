// Author: Lin
// Date: 2025-12-26 09:18:55
// Description: the target is to sort and filter data in inplace.

#include "src/tableModel.hpp"

GiTableModel::GiTableModel(QObject *parent): QAbstractTableModel(parent)
{
  m_rawData.resize(100000);
  for (int i = 0; i < 100000; ++i) {
    auto& row = m_rawData[i];
    for (int j = 0; j < MAX_COLUMN; ++j) {
        row[j] = i * MAX_COLUMN + j; 
    }
  }
  for(int i=0; i<nCol; ++i){
    m_filteredColIndices.append(i);
  }
}

int GiTableModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return m_rawData.size();
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
  return m_rawData[row][m_filteredColIndices[col]];
}

QVariant GiTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }
  if (orientation == Qt::Horizontal) {
    return QString("Column %1").arg(m_filteredColIndices[section]);
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
  m_filteredColIndices.append(5);
  m_filteredColIndices.append(2);
  m_filteredColIndices.append(9);
  m_filteredColIndices.append(15);
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