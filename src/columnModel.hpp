// Author: Lin
// Date: 2025-12-29 12:47:30
// Description:

#ifndef COLUMN_MODEL_HPP
#define COLUMN_MODEL_HPP

#include <QAbstractListModel>

struct ColumnItem{
  QString name;
  int column;
  bool show;
};

class ColumnModel : public QAbstractListModel {
  Q_OBJECT

public:
  explicit ColumnModel(QObject *parent = nullptr);

  enum ColumnRoles {
    NameRole = Qt::UserRole + 1,
    ColumnRole,
    ShowRole
  };

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex &index,
               const QVariant &value,
               int role = Qt::EditRole) override;
  QHash<int, QByteArray> roleNames() const override;

signals:
  void sender_columns(const QList<int> visibleColumns);

public slots:
  void receive_headers(const QList<QString> headers);
  void receive_columns(const QList<int> visibleColumns);

private:
    QList<ColumnItem> m_items;
};

#endif // COLUMN_MODEL_HPP