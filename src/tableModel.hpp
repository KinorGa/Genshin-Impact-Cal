// Author: Lin
// Date: 2025-12-26 09:18:40
// Description:

#include <QAbstractTableModel>
#include <QVector>

const int MAX_COLUMN = 30;
using uT = unsigned long long;

class GiTableModel: public QAbstractTableModel {
  Q_OBJECT
public:
  explicit GiTableModel(QObject *parent = nullptr);

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;
  Q_INVOKABLE void filter1();
  Q_INVOKABLE void filter2();
  Q_INVOKABLE void resetData();

  // sort
  Q_INVOKABLE void sortByColumn(int column);
  void applySortFilter(int colIndex, int order);

public slots:
  void receive_state(QVector<uT> states, QVector<double> buffers);

private:
  QVector<std::array<double, MAX_COLUMN>> m_rawData;
  QVector<int> m_filteredRowIndices;
  QVector<int> m_filteredColIndices;
  QVector<QString> m_headers;

  QVector<int> m_colOrders;
  int nCol=29;
  int mRow=0;
};