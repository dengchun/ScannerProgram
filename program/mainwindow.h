#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDataVisualization>
#include <QAbstract3DInputHandler>
#include <QVector3D>
#include <QDebug>
#include <QPointF>
#include <QSplitter>
#include <QSet>
#include <QList>
#include <QChartView>
#include <QComboBox>
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

    Q3DScatter *m_3Dgraph;
    QScatter3DSeries *m_3Dseries;
    QWidget *container;
    QChart *m_chart;
    QScatterSeries *m_scatterSeries; //散点类型
    QChartView *containerChart;
    QString f = "C:\\Users\\dengchun\\Desktop\\partN_0000_210913134725_TPscan.txt";
    QLineEdit *lineedit_visualpath = new QLineEdit();
    QComboBox *select_z = new QComboBox();
    QColor color;

    //郑博：：
    void pdfDrawForm(QPainter* paint,int y,int horzBorder,int row,int column,int unitHeight,QFont &font,QStringList& list);//新加
    void export_report();//新加

    QTableWidget *table_1 = new QTableWidget(this);
    QTableWidget *table_2 = new QTableWidget(this);
    void inittable();
    void getinformation();
    class information{    //存储叶片数据
        public:
            QString LE_RADIUS;
            QString TE_RADIUS;
            QString LE_THICK;
            QString TE_THICK;
            QString MAC_THICK;
            QString CHORD_WID;
            QString STACK_T;
            QString STACK_X;
            QString STACK_Y;
            QString TWIST_ANG;
            QString CV_CONT_MAX;
            QString CV_CONT_MIN;
            QString CC_CONT_MAX;
            QString CC_CONT_MIN;
            QString LE_CONT_MAX;
            QString LE_CONT_MIN;
            QString TE_CONT_MAX;
            QString TE_CONT_MIN;
            QString BOW_AMOUNT;
        public:
            QString element;   //零件名称
            QString file_name; //检测文件名称
            QString section; //所选截面
            QString test_time; //检测时间
            QString SERIAL_NUMBER;
            QString CLOCK_NUMBER;
            QString PARTCOUNT;
        public:
            QString CVX_WEIGHT; //加权
            QString CVX_DEVIATION_MAX; //最大偏差
            QString CVX_DEVIATION_MIN; //最小偏差
            QString CVX_SHAPE; //形状
            QString CVX_MEAN; //均差
            QString CVX_STANDARD; //标准差
            QString CCV_WEIGHT;
            QString CCV_DEVIATION_MAX;
            QString CCV_DEVIATION_MIN;
            QString CCV_SHAPE;
            QString CCV_MEAN;
            QString CCV_STANDARD;
            QString LE_WEIGHT;
            QString LE_DEVIATION_MAX;
            QString LE_DEVIATION_MIN;
            QString LE_SHAPE;
            QString LE_MEAN;
            QString LE_STANDARD;
            QString TE_WEIGHT;
            QString TE_DEVIATION_MAX;
            QString TE_DEVIATION_MIN;
            QString TE_SHAPE;
            QString TE_MEAN;
            QString TE_STANDARD;
    }inf;
    ~MainWindow();
public Q_SLOTS:
    void openfile();
    void visualize();
    void initMap();
    void initCharts();
    void rePrintChart();

    //郑博：：
    void settable();


private:
    Ui::MainWindow *ui;


};
#endif // MAINWINDOW_H
