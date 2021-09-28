#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <iostream>
#include <QFile>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtGui/QScreen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qRegisterMetaType<MY_BUF3D>("MY_BUF3D");

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::openfile()
{
    QString filepath = QFileDialog::getOpenFileName(this,"open file","D:\\");
    std::cout<<filepath.toStdString().data()<<std::endl;
    f = filepath;
}

void MainWindow::visualize()
{
    QtDataVisualization::QScatterDataArray *data = new QScatterDataArray();
    QFile file(f);
    qDebug()<<f<<endl;
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QList<QPointF> listPoint;
    while(!file.atEnd()){

        QPointF pointf;
        double zuobiao[3];
        QString line = file.readLine();
        QStringList str;
        QString s;
        str=line.split("\t");
        pointf.setX(str.at(0).toFloat());
        pointf.setY(str.at(1).toFloat());
        listPoint << pointf;

        zuobiao[0] = str.at(0).toFloat();
        zuobiao[1] = str.at(1).toFloat();
        zuobiao[2] = str.at(2).toFloat();
        *data << QVector3D(zuobiao[0], zuobiao[1], zuobiao[2]);
    }
    m_3Dseries->dataProxy()->resetArray(data);
    m_3Dseries->setBaseColor(QColor(255,255,255));//设置点的颜色
    m_scatterSeries->replace(listPoint);
}

void MainWindow::initMap()
{
    m_3Dgraph = new Q3DScatter();
    container = QWidget::createWindowContainer(m_3Dgraph);
    QScatterDataProxy *proxy = new QScatterDataProxy();
    m_3Dseries = new QScatter3DSeries(proxy);
    m_3Dseries->setMeshSmooth(true);
    m_3Dgraph->addSeries(m_3Dseries);
    //创建坐标轴
    m_3Dgraph->axisX()->setTitle("axis X");
    m_3Dgraph->axisX()->setTitleVisible(true);
    m_3Dgraph->axisX()->setRange(-30,30);
    m_3Dgraph->axisY()->setTitle("axis Y");
    m_3Dgraph->axisY()->setTitleVisible(true);
    m_3Dgraph->axisY()->setRange(-30,30);
    m_3Dgraph->axisZ()->setTitle("axis Z");
    m_3Dgraph->axisZ()->setTitleVisible(true);
    m_3Dgraph->axisZ()->setRange(50,60);
    m_3Dgraph->activeTheme()->setLabelBackgroundEnabled(false);
    m_3Dgraph->activeTheme()->setBackgroundColor(QColor(90,90,90));//设置背景色
    m_3Dgraph->setShadowQuality(QAbstract3DGraph::ShadowQualityNone);
    QtDataVisualization::QScatterDataArray data;
    QFile file(f);
    qDebug()<<f<<endl;
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    while(!file.atEnd()){
        QString line = file.readLine();
        QStringList str;
        QString s;
        float value;
        str=line.split("\t");
        double zuobiao[3];
        for(int i = 0;i<3;i++)
           {
               s = str.at(i);         //获取字符串集合的元素
               value = s.toFloat();    //转为float类型

               zuobiao[i]=value;
           }
        data << QVector3D(zuobiao[0], zuobiao[1], zuobiao[2]);
    }
    m_3Dseries->setMesh(QAbstract3DSeries::MeshSphere);//数据点为圆球
    m_3Dseries->setSingleHighlightColor(QColor(0,0,255));//设置点选中时的高亮颜色
    m_3Dseries->setBaseColor(QColor(0,255,255));//设置点的颜色
    m_3Dseries->setItemSize(0.01);//设置点的大小
    m_3Dseries->dataProxy()->addItems(data);
}

void MainWindow::initCharts()
{
    containerChart = new QChartView(this);
    m_chart = new QChart();
    m_scatterSeries = new QScatterSeries(m_chart);
    m_scatterSeries->setUseOpenGL(true);            //启用OpenGL，否则可能会很卡顿
    m_scatterSeries->setMarkerShape(QScatterSeries::MarkerShapeRectangle);//设置散点样式
    m_scatterSeries->setMarkerSize(1);              //设置散点大小
    m_scatterSeries->setColor(QColor(0,0,255));     //设置散点颜色
    m_chart->addSeries(m_scatterSeries);

    QList<QPointF> listPoint;
    m_scatterSeries->clear();
    QFile file(f);
    qDebug()<<f<<endl;
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    while(!file.atEnd()){
        QPointF pointf;
        QString line = file.readLine();
        QStringList str;
        QString s;
        str=line.split("\t");
        pointf.setX(str.at(0).toFloat());
        pointf.setY(str.at(1).toFloat());
        listPoint << pointf;
    }
    m_scatterSeries->replace(listPoint);
    m_chart->createDefaultAxes();
    m_chart->axes(Qt::Horizontal).first()->setRange(-30, 30); //设置水平坐标范围
    m_chart->axes(Qt::Vertical).first()->setRange(-30, 30);       //设置垂直坐标范围
    m_chart->legend()->hide();                                    //隐藏图例
    containerChart->setChart(m_chart);

}


