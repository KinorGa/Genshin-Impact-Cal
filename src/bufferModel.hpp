// Author: Lin
// Date: 2025-12-28 08:22:24
// Description:

#ifndef BUFFER_MODEL_HPP
#define BUFFER_MODEL_HPP

#include <QAbstractListModel>
#include <QList>
#include <QString>

struct BufferItem{
  QString tag;
  double value;
};

class BufferModel : public QAbstractListModel {
  Q_OBJECT
  Q_PROPERTY(double total READ total NOTIFY totalChanged)
  Q_PROPERTY(int tagKey READ tagKey WRITE setTagKey)
public:
  explicit BufferModel(QObject *parent = nullptr);

  enum BufferRoles {
    TagRole = Qt::UserRole + 1,
    ValueRole
  };

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex &index,
               const QVariant &value,
               int role = Qt::EditRole) override;
  QHash<int, QByteArray> roleNames() const override;
  
  double total() const;

  int tagKey() const { return m_tagKey; }
  void setTagKey(int key) { m_tagKey = key; }

  Q_INVOKABLE void sender_buffer_();

public slots:
  void addBuffer(const QString &tag, double value);
  void removeBuffer(int index);
  void clearBuffer();

signals:
    void totalChanged();
    void senderBuffer(int tagKey, QVector<QPair<QString, double>> data);
private:  
    void updateTotal();
    QList<BufferItem> m_items;
    double m_total = 0.0;
    int m_tagKey = -1;
};

#endif // BUFFER_MODEL_HPP