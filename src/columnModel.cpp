// Author: Lin
// Date: 2025-12-29 12:47:41
// Description:

#include "src/columnModel.hpp"
ColumnModel::ColumnModel(QObject *parent)
    : QAbstractListModel(parent) {}

int ColumnModel::rowCount(const QModelIndex &parent) const {
  if(parent.isValid())
    return 0;
  return m_items.count();
}

QVariant ColumnModel::data(const QModelIndex &index, int role) const {
  if(!index.isValid() || index.row() < 0 || index.row() >= m_items.count())
    return QVariant();

  const ColumnItem &item = m_items.at(index.row());
  if(role == NameRole)
    return item.name;
  else if(role == ColumnRole)
    return item.column;
  else if(role == ShowRole)
    return item.show;

  return QVariant();
}

bool ColumnModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  if(!index.isValid() || index.row() < 0 || index.row() >= m_items.count())
    return false;

  ColumnItem &item = m_items[index.row()];
  if(role == NameRole)
    item.name = value.toString();
  else if(role == ColumnRole)
    item.column = value.toInt();
  else if (role == ShowRole)
    item.show = value.toBool();
  else
    return false;{
  }

  emit dataChanged(index, index, {role});
  QList<int> visibleColumns;
  for(auto &item: m_items){
    if(item.show){
      visibleColumns.append(item.column);
    }
  }

  qDebug() << "Emitting visible columns:" << visibleColumns;
  emit sender_columns(visibleColumns);
  return true;
}

QHash<int, QByteArray> ColumnModel::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[NameRole] = "name";
  roles[ColumnRole] = "column";
  roles[ShowRole] = "show";
  return roles;
}

void ColumnModel::receive_headers(const QList<QString> headers) {
  beginResetModel();
  m_items.clear();
  for(int i=0; i<headers.size(); ++i){
    ColumnItem item;
    item.name = headers[i];
    item.column = i;
    item.show = true;
    m_items.append(item);
  }
  endResetModel();
}

void ColumnModel::receive_columns(const QList<int> visibleColumns) {
  beginResetModel();
  for(auto &item: m_items){
    item.show = visibleColumns.contains(item.column);
  }
  qDebug() << "Received visible columns:" << visibleColumns;
  for(auto &item: m_items){
    qDebug() << "name " << item.name << " Column" << item.column << "show:" << item.show;
  }
  endResetModel();
}