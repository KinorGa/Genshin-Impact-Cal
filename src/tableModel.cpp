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
  for(int i=0; i<mCol; ++i){
    m_filteredColIndices.append(i);
    m_colOrders.append(0);
  }

  for(int i=0; i<mCol; ++i){
    m_headers.append(QString("Column %1").arg(i));
  }
}

int GiTableModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return m_filteredRowIndices.size();
}

int GiTableModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return m_filteredColIndices.size();
}

QVariant GiTableModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || role != Qt::DisplayRole) {
    qDebug() << "Invalid index or role " << index;
    return QVariant();
  }
  int row = index.row();
  int col = index.column();
  if (row < 0 || row >= m_filteredRowIndices.size() || col < 0 || col >= m_filteredColIndices.size()) {
    qDebug() << "Index out of bounds: row " << row << ", col " << col;
    return QVariant();
  }
  QVariant res=m_rawData[m_filteredRowIndices[row]][m_filteredColIndices[col]];

  if(res.isNull()||res.toString().isEmpty()){
    qDebug() << "Null or empty data at row " << row << ", col " << col;
    qDebug() << m_filteredColIndices;
  }
  if(m_filteredColIndices[col]>=nstat){ 
    return QString::number(res.toDouble(), 'f', 4);
  }
  return QString::number(res.toDouble(), 'f', 0);
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
    return QString::number(section + 1);
  } return QVariant{};
}

QHash<int, QByteArray> GiTableModel::roleNames() const {
  return {{Qt::DisplayRole, "display"}};
}

void GiTableModel::filter1() {
  // filterByInterval(4, 20., 20.);
  // display HP, HP%, EM, CR, CD, DMG DEALT, FINAL DMG
  beginResetModel();
  m_filteredColIndices.clear();
  m_filteredColIndices.append(nstat+1); // HP
  m_filteredColIndices.append(nstat+4); // HP%
  m_filteredColIndices.append(nstat+6); // EM
  m_filteredColIndices.append(nstat+8); // CR
  m_filteredColIndices.append(nstat+9); // CD
  m_filteredColIndices.append(nstat+10); // DMG DEALT
  m_filteredColIndices.append(nstat+11); // FINAL DMG
  endResetModel();
  emit sender_columns(m_filteredColIndices);
}

void GiTableModel::filter2(){
  // display ATK, ATK%, EM, CR, CD, DMG DEALT, FINAL DMG
  beginResetModel();
  m_filteredColIndices.clear();
  m_filteredColIndices.append(nstat); // ATK
  m_filteredColIndices.append(nstat+3); // ATK%
  m_filteredColIndices.append(nstat+6); // EM
  m_filteredColIndices.append(nstat+8); // CR
  m_filteredColIndices.append(nstat+9); // CD
  m_filteredColIndices.append(nstat+10); // DMG DEALT
  m_filteredColIndices.append(nstat+11); // FINAL DMG
  endResetModel();
  emit sender_columns(m_filteredColIndices);
}

void GiTableModel::resetData(){
  // reset col
  beginResetModel();
  m_filteredColIndices.clear();
  for(int i=0; i<m_headers.size(); ++i){
    m_filteredColIndices.append(i);
  }
  endResetModel();

  // reset row
  QFuture<void> res=QtConcurrent::run([this]()->void{
    QVector<int> newFilteredRowIndices;
    int n=m_rawData.size();
    newFilteredRowIndices.reserve(n);
    for(int i=0; i<n; ++i)newFilteredRowIndices.append(i);

    QMetaObject::invokeMethod(this, [this, tempRowIndices=std::move(newFilteredRowIndices)]()->void{
      beginResetModel();
      QVector<int> oldRowIndices=std::move(m_filteredRowIndices);

      m_filteredRowIndices=std::move(tempRowIndices);
      
      QFuture<void> released= QtConcurrent::run([death=std::move(oldRowIndices)]() mutable{});
      endResetModel();
    }, Qt::QueuedConnection);
  });

  // reset col orders
  for(int i=0; i<m_colOrders.size(); ++i){
    m_colOrders[i]=0;
  }

  emit sender_headers(m_headers);
}

void GiTableModel::receive_state(QVector<uT> states, QVector<double> buffers){
  // process states to filter data
  qDebug() << "Received states " << states;

  // Run in background to avoid blocking UI
  QFuture<void> res=QtConcurrent::run(
    [this, states, buffers]() {
    QVector<std::array<double, MAX_COLUMN>> tempData;
    QElapsedTimer timer;
    timer.start();
    tempData = calculate(states, buffers, 25);
    
    qDebug() << "Data calculation took" << timer.elapsed() << "ms";
    timer.restart();
    //  if original data size is not equal to tempData size, clear and reset filtered indices
    // if(m_rawData.size() != tempData.size()){
    //   m_filteredRowIndices.clear();
    //   m_filteredRowIndices.reserve(tempData.size());
    //   for(int i=0; i<tempData.size(); ++i){
    //     m_filteredRowIndices.append(i);
    //   }
    // }
    QVector<int> newFilteredRowIndices;
    for(int i=0; i<tempData.size(); ++i){
      newFilteredRowIndices.append(i);
    }
    qDebug() << "update rowIndices took" << timer.elapsed() << "ms";
    
    QMetaObject::invokeMethod(this, [this, tempData=std::move(tempData), newFilteredRowIndices=std::move(newFilteredRowIndices)]() {
      QElapsedTimer timer;
      beginResetModel();
      timer.start();
      
      QVector<std::array<double, MAX_COLUMN>> oldRawData = std::move(m_rawData);
      QVector<int> oldRowIndices = std::move(m_filteredRowIndices);
          
      m_rawData = std::move(tempData);
      m_filteredRowIndices = std::move(newFilteredRowIndices);
      QFuture<void> released=QtConcurrent::run([oldData = std::move(oldRawData), oldRawI=std::move(oldRowIndices)]() mutable {});
      qDebug() << "Data assignment took" << timer.elapsed() << "ms";
    
      endResetModel();
    }, Qt::QueuedConnection);
  });

  updateHeader(states);
}

void GiTableModel::sortByColumn(int column){
  if(column <0 || column >= m_filteredColIndices.size()) return;
  int colIndex = m_filteredColIndices[column];
  int order=m_colOrders[colIndex];
  qDebug() << "start sort column " << colIndex;
  qDebug() << "m_filter row size " << m_filteredRowIndices.size();
  qDebug() << "m_rawData size " << m_rawData.size();
  qDebug() << "order " << order;

  // beginResetModel();
  // applySortFilter(colIndex, order);
  // endResetModel();

  QFuture<void> sortRes=QtConcurrent::run(
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

void GiTableModel::filterByInterval(int column, double minValue, double maxValue){
  if(column<0 || column>=m_filteredColIndices.size())return ;

  // real column index
  int colIndex=m_filteredColIndices[column];

  QFuture<void> filterRes=QtConcurrent::run([this, colIndex, minValue, maxValue]()->void{
    QElapsedTimer timer;
    timer.restart();
    // copy old row indices
    QVector<int> newFilteredRowIndices;
    for(int rowIdx: m_filteredRowIndices){
      double value=m_rawData[rowIdx][colIndex];
      if(value>=minValue && value<=maxValue){
        newFilteredRowIndices.append(rowIdx);
      }
    }
    qDebug() << "Filtering took" << timer.elapsed() << "ms";
    qDebug() << "Filter size " << newFilteredRowIndices.size();;

    QMetaObject::invokeMethod(this, [this, newFilteredRowIndices=std::move(newFilteredRowIndices)]()->void{
      beginResetModel();
      QVector<int> oldRowIndices=std::move(m_filteredRowIndices);
      m_filteredRowIndices=std::move(newFilteredRowIndices);
      QFuture<void> released= QtConcurrent::run([death=std::move(oldRowIndices)]() mutable{});
      endResetModel();
      qDebug() << "Filter applied on UI thread.";
    }, Qt::QueuedConnection);
  });
}

void GiTableModel::updateHeader(QVector<uT> states){
  QFuture<void> headerRes=QtConcurrent::run([this, states]()->void{
    // get new headers
    QVector<QString> newHeaders;
    vT<int> fm(GiCal::MAX_COLUMN, 0);
    for(int i=0; i<25; ++i){
      if(i%5!=0){
        fm[states[i]]++;
      }
    }
    for(int i=0; i<fm.size(); ++i){
      if(fm[i]>0){
        newHeaders.append(GiCal::statnames[i] + " Stat");
      }
    }
    nstat=newHeaders.size();
    // add compute header
    for(auto h: GiCal::showheaders){
      newHeaders.append(h);
    }

    // update m_filteredColIndices
    QVector<int> newColIndices;
    for(int i=0; i<newHeaders.size(); ++i)newColIndices.append(i);

    QMetaObject::invokeMethod(this, [this, newHeaders=std::move(newHeaders), newColIndices=std::move(newColIndices)]()->void{
      beginResetModel();
      auto oldHeaders=std::move(m_headers);
      m_headers = std::move(newHeaders);

      QFuture<void> released= QtConcurrent::run([death=std::move(oldHeaders)]() mutable{});

      auto oldColIndices=std::move(m_filteredColIndices);
      m_filteredColIndices=std::move(newColIndices);
      QFuture<void> released2= QtConcurrent::run([death=std::move(oldColIndices)]() mutable{});
      emit sender_headers(m_headers);
      endResetModel();
      qDebug() << "Headers updated on UI thread.";
    }, Qt::QueuedConnection);
  });
  
}

void GiTableModel::receive_columns(QVector<int> visibleColumns){
  beginResetModel();
  m_filteredColIndices=visibleColumns;
  endResetModel();
}
