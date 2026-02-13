#include <QFile>
// #include <QGuiApplication>
#include "fstream" // IWYU pragma: keep
#include "src/states.hpp"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <yaml-cpp/yaml.h>
#include "src/tableModel.hpp"
#include "src/bufferModel.hpp"
#include <QElapsedTimer>
#include "src/columnModel.hpp"


// Custom message handler to write to file
void messageHandler(QtMsgType type, const QMessageLogContext &context,
                    const QString &msg) {
  QFile file("app.log");
  if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
    QTextStream stream(&file);
    QString timestamp =
        QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    QString levelStr;
    switch (type) {
    case QtDebugMsg:
      levelStr = "DEBUG";
      break;
    case QtInfoMsg:
      levelStr = "INFO";
      break;
    case QtWarningMsg:
      levelStr = "WARNING";
      break;
    case QtCriticalMsg:
      levelStr = "CRITICAL";
      break;
    case QtFatalMsg:
      levelStr = "FATAL";
      break;
    }

    stream << "[" << timestamp << "] [" << levelStr << "] " << msg << "\n";
    file.close();
  }

  // Also print to console
  fprintf(stderr, "%s\n", msg.toLocal8Bit().constData());
}

void test(){
  QElapsedTimer timer;
  QVector<std::array<double, 29>> data;
  timer.restart();
  data.reserve(7500000);
  // data.resize(7500000);
  for(int i=0; i<7500000; ++i){
    std::array<double, 29> row;
    // auto &row=data[i];
    row.fill(0);
    data.append(row);
  }
  qDebug() << "Test reallocate took" << timer.elapsed() << "ms";
}

void test2(){
  QElapsedTimer timer;
  QVector<std::array<float, 15>> data;
  timer.restart();
  data.reserve(7500000);
  for(int i=0; i<7500000; ++i){
    std::array<float, 15> row;
    row.fill(0);
    data.append(row);
  }
  qDebug() << "Test2 reallocate took" << timer.elapsed() << "ms";
}

int main(int argc, char *argv[]) {
  qInstallMessageHandler(messageHandler);

  QQuickStyle::setStyle("Basic");
  // test();
  // test2();

  QApplication app(argc, argv);

  QQmlApplicationEngine engine;

  States *states = new States(&app);
  engine.rootContext()->setContextProperty("States", states);
  GiTableModel *giTableModel=new GiTableModel();
  engine.rootContext()->setContextProperty("giTableModel", giTableModel);
  qmlRegisterType<BufferModel>("GiCal.Models", 1, 0, "BufferModel");
  QObject::connect(states, &States::sender, giTableModel, &GiTableModel::receive_state);

  ColumnModel *columnModel=new ColumnModel();
  engine.rootContext()->setContextProperty("columnModel", columnModel);
  QObject::connect(giTableModel, &GiTableModel::sender_headers, columnModel, &ColumnModel::receive_headers);
  QObject::connect(columnModel, &ColumnModel::sender_columns, giTableModel, &GiTableModel::receive_columns);
  QObject::connect(giTableModel, &GiTableModel::sender_columns, columnModel, &ColumnModel::receive_columns);

  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
  engine.loadFromModule("GiCal", "Main");

  return app.exec();
}
