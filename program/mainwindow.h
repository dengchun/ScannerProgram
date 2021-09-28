#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDataVisualization>
#include <QAbstract3DInputHandler>
#include <QVector3D>
#include <QDebug>
#include <QPointF>
#include <QSplitter>

#include <QChartView>
#include <QtCharts>
#include <QtCharts/QScatterSeries>


typedef QVector<QVector3D> MY_BUF3D;

using namespace QtDataVisualization;
QT_CHARTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void initMap();
    void initCharts();
    Q3DScatter *m_3Dgraph;
    QScatter3DSeries *m_3Dseries;
    QWidget *container;
    QChart *m_chart;
    QScatterSeries *m_scatterSeries; //散点类型
    QChartView *containerChart;
    QString f = "C:\\Users\\dengchun\\Desktop\\data0909_1.txt";
    ~MainWindow();

public Q_SLOTS:
    void openfile();

    void visualize();



private:
    Ui::MainWindow *ui;


};
#endif // MAINWINDOW_H
