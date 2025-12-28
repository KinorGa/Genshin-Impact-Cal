// Author: Lin
// Date: 2025-12-28 08:22:28
// Description:
#include "src/bufferModel.hpp"

BufferModel::BufferModel(QObject *parent)
    : QAbstractListModel(parent) {}

int BufferModel::rowCount(const QModelIndex &parent) const {
  if(parent.isValid())
    return 0;
  return m_items.count();
}

QVariant BufferModel::data(const QModelIndex &index, int role) const {
  if(!index.isValid() || index.row() < 0 || index.row() >= m_items.count())
    return QVariant();

  const BufferItem &item = m_items.at(index.row());
  if(role == TagRole)
    return item.tag;
  else if(role == ValueRole)
    return QString::number(item.value);

  return QVariant();
}

bool BufferModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  if(!index.isValid() || index.row() < 0 || index.row() >= m_items.count())
    return false;

  BufferItem &item = m_items[index.row()];
  if(role == TagRole)
    item.tag = value.toString();
  else if(role == ValueRole)
    item.value = value.toDouble();
  else
    return false;

  emit dataChanged(index, index, {role});
  updateTotal();
  return true;
}

QHash<int, QByteArray> BufferModel::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[TagRole] = "tag";
  roles[ValueRole] = "value";
  return roles;
}

double BufferModel::total() const {
  return m_total;
}

void BufferModel::addBuffer(const QString &tag, double value) {
  beginInsertRows(QModelIndex(), m_items.count(), m_items.count());
  bool ok;
  double val = QString::number(value).toDouble(&ok);
  m_items.append({tag, ok?value:0.0});
  endInsertRows();
  updateTotal();
}

void BufferModel::removeBuffer(int index) {
  if(index < 0 || index >= m_items.count())
    return;
  beginRemoveRows(QModelIndex(), index, index);
  m_items.removeAt(index);
  endRemoveRows();
  updateTotal();
}

void BufferModel::clearBuffer() {
  beginResetModel();
  m_items.clear();
  endResetModel();
  updateTotal();
}

void BufferModel::updateTotal() {
  double newTotal = 0.0;
  for(const BufferItem &item : std::as_const(m_items)) {
    newTotal += item.value;
  }
  if(!qFuzzyCompare(m_total, newTotal)) {
    m_total = newTotal;
    emit totalChanged();
  }
}