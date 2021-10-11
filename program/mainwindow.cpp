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
    initMap();
    initCharts();
    inittable();
    QObject::connect(select_z, SIGNAL(currentTextChanged(QString)), this,SLOT(rePrintChart()));

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
    lineedit_visualpath->setText(filepath);
    visualize();

    //郑博：：
    QStringList str;
    inf.file_name=f;
    str=inf.file_name.split("/");
    inf.file_name=str.at(str.size()-1);
    str.clear();
    str=inf.file_name.split("_");
    inf.PARTCOUNT=str.at(0);
    //str.at(1);
    inf.test_time=str.at(2);
    inf.test_time=inf.test_time.mid(0,2)+"年"+inf.test_time.mid(2,2)+"月"+inf.test_time.mid(4,2)+"日"+"\t"+inf.test_time.mid(6,2)+":"+inf.test_time.mid(8,2)+":"+inf.test_time.mid(10,2);
    //str.at(3);
    qDebug()<<inf.file_name<<endl;
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

        selectInterface.insert(zuobiao[2]);
        *data << QVector3D(zuobiao[0], zuobiao[1], zuobiao[2]);
    }
    float interfa;
    foreach(interfa,selectInterface){
        qDebug()<<interfa<<endl;
    }
    m_3Dseries->dataProxy()->resetArray(data);
    m_3Dseries->setBaseColor(QColor(255,0,0));//设置点的颜色
    m_scatterSeries->replace(listPoint);
}

void MainWindow::initMap()
{
    m_3Dgraph = new Q3DScatter();

    m_3Dgraph->scene()->activeCamera()->setYRotation(90);
    m_3Dgraph->scene()->activeCamera()->setZoomLevel(200.0f);
    container = QWidget::createWindowContainer(m_3Dgraph);

    QValue3DAxis *m_Z_Axis = new QValue3DAxis;
    QValue3DAxis *m_X_Axis = new QValue3DAxis;
    QValue3DAxis *m_Y_Axis = new QValue3DAxis;

    Q3DTheme *currentTheme = m_3Dgraph->activeTheme();
    currentTheme->setLabelBackgroundEnabled(true);
    color.setRgb(70,70,70);
    currentTheme->setBackgroundColor(color);
    currentTheme->setWindowColor(color);
    QScatterDataProxy *proxy = new QScatterDataProxy();
    m_3Dseries = new QScatter3DSeries(proxy);
    m_3Dseries->setMeshSmooth(true);
    m_3Dgraph->addSeries(m_3Dseries);
    //创建坐标轴

    m_Z_Axis->setTitle("Height");
    m_Z_Axis->setTitleVisible(true);
    m_Z_Axis->setAutoAdjustRange(true);


    m_3Dgraph->setAxisX(m_X_Axis);
    m_3Dgraph->setAxisY(m_Y_Axis);
    m_3Dgraph->setAxisZ(m_Z_Axis);
    m_3Dgraph->activeTheme()->setLabelBackgroundEnabled(false);
    m_3Dgraph->setShadowQuality(QAbstract3DGraph::ShadowQualityNone);
    QtDataVisualization::QScatterDataArray data;
    QFile file(f);
    qDebug()<<f<<endl;
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    while(!file.atEnd()){
        QString line = file.readLine();
        QStringList str;
        QString s;
        str=line.split("\t");
        double zuobiao[3];
        zuobiao[0] = str.at(0).toFloat();
        zuobiao[1] = str.at(1).toFloat();
        zuobiao[2] = str.at(2).toFloat();
        selectInterface.insert(zuobiao[2]);
        data << QVector3D(zuobiao[0], zuobiao[1], zuobiao[2]);
    }

    float interfa;
    foreach(interfa,selectInterface){
        qDebug()<<interfa<<endl;
        select_z->addItem(QString::number(interfa));
    }
    m_3Dseries->setMesh(QAbstract3DSeries::MeshBar);//数据点为圆球
    m_3Dseries->setSingleHighlightColor(QColor(0,0,255));//设置点选中时的高亮颜色
    m_3Dseries->setBaseColor(QColor(255,0,0));//设置点的颜色
    m_3Dseries->setItemSize(0.005);//设置点的大小
    m_3Dseries->dataProxy()->addItems(data);
}

void MainWindow::initCharts()
{
    containerChart = new QChartView(this);
    m_chart = new QChart();
    m_scatterSeries = new QScatterSeries(m_chart);
    m_chart->setTheme(QChart::ChartThemeDark);
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
    m_chart->axes(Qt::Vertical).first()->setRange(-50, 50);       //设置垂直坐标范围
    m_chart->legend()->hide();                                    //隐藏图例
    containerChart->setChart(m_chart);

    //郑博：：
    containerChart->winId();     //添加此句之后bug消失

    //瑶博：：
    containerChart->setRubberBand(QChartView::RectangleRubberBand);//用鼠标拉出矩形放大框
}

void MainWindow::rePrintChart(){
    QFile file(f);
    qDebug()<<f<<endl;
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QList<QPointF> listPoint;
    while(!file.atEnd()){
        QPointF pointf;
        QString line = file.readLine();
        QStringList str;
        QString s;
        str=line.split("\t");
        pointf.setX(str.at(0).toFloat());
        pointf.setY(str.at(1).toFloat());

        if(str.at(2).toFloat()==select_z->currentText().toFloat()){
            listPoint << pointf;
        }
    }
    m_scatterSeries->replace(listPoint);
}

//郑博：：
void MainWindow::pdfDrawForm(QPainter* paint,int y,int horzBorder,int row,int column,int unitHeight,QFont &font,QStringList& list)
{
    paint->setFont(font);

    paint->setPen(QPen(QBrush(QColor(0,0,0)),2));

    int Width = paint->viewport().width()-horzBorder*2;

    int unitWidth = Width/column;

    for(int i=0;i<row;i++)
    {
        int x= horzBorder;

        for(int j=0;j<column;j++)
        {
            paint->drawText(QRect(x,y, unitWidth, unitHeight), Qt::AlignCenter,list[i*column+j]);

            paint->drawRect(QRect(x,y, unitWidth, unitHeight));

            x+=unitWidth;
        }

        y += unitHeight;
    }

}
void MainWindow::export_report()
{
    QString fileName = QFileDialog::getSaveFileName(this,"Select Save Location","C://Users/me/Desktop/report.pdf","pdf Files (*.pdf)");
    //创建了一个窗口，以此确定文件名和保存路径
    //第2个参数代表窗口的标题，第4个参数代表文件的类型
    //第3个参数代表默认路径及文件名，如设置为QApplication::applicationDirPath()，则无默认路径和默认文件名，如设置为"C://report.pdf"，则默认路径为C://，默认文件名为report.pdf
    QFile pdfFile(fileName);//创建文件
    pdfFile.open(QIODevice::WriteOnly);//打开文件
    QPdfWriter *pdfWriter = new QPdfWriter(&pdfFile);//创建PDF写入对象
    pdfWriter->setPageSize(QPagedPaintDevice::A4);//设置纸张大小
    pdfWriter->setResolution(300);//设置纸张的分辨率为300，因此像素为3508*2479
    pdfWriter->setTitle("?????");
    int iMargin = 30;  //设置页边距
    pdfWriter->setPageMargins(QMarginsF(iMargin,iMargin-20,iMargin,iMargin));//设置页边距  left, top,right, bottom
    QPainter *pdfPainter = new QPainter(pdfWriter);//添加内容使用QPainter
    QStringList list;  //用于绘制表格,是表格中数据的数组

    //设置标题
    QTextOption option(Qt::AlignHCenter | Qt::AlignVCenter);
    option.setWrapMode(QTextOption::WordWrap);
    int iTop = 10; //距离头部的距离,为页面的Y坐标值
    int iContentWidth = 2250;
    //标题字体
    QFont font;
    font.setFamily("simhei.ttf");
    int fontSize = 22;
    font.setPointSize(fontSize);
    pdfPainter->setFont(font);
    pdfPainter->drawText(QRect(0, iTop, iContentWidth, 90), "叶片检测结果报告", option);
    int nPDFWidth = pdfPainter->viewport().width();     //页面宽度
    int nPDFHeight = pdfPainter->viewport().height();   //页面高度

    //int y=10;   //等同于iTOP

    iTop+=140;

    pdfPainter->setPen(QPen(QBrush(QColor(0,0,0)),5));
    pdfPainter->drawLine(0,iTop,nPDFWidth,iTop);                //画横线
    pdfPainter->drawLine(0,iTop+18,nPDFWidth,iTop+18);
    iTop+=60;

    fontSize = 16;
    font.setPointSize(fontSize);
    pdfPainter->setFont(font);
    pdfPainter->drawText(QRect(100,iTop, nPDFWidth, 70), "零件名称: %1");
    pdfPainter->drawText(QRect(nPDFWidth/2+100,iTop, nPDFWidth, 70),QString("检测时间: %1").arg(inf.test_time));
    iTop+=90;


    pdfPainter->drawText(QRect(100,iTop, nPDFWidth, 70), QString("所选截面: %1").arg(inf.section));
    pdfPainter->drawText(QRect(nPDFWidth/2+100,iTop, nPDFWidth, 70),QString("PARTCOUNT: %1").arg(inf.PARTCOUNT));
    iTop+=90;

    pdfPainter->drawText(QRect(100,iTop, nPDFWidth, 70), "SERIAL NUMBER: %1");
    pdfPainter->drawText(QRect(nPDFWidth/2+100,iTop, nPDFWidth, 70),"CLOCK NUMBER: %1");
    iTop+=110;

    pdfPainter->drawText(QRect(100,iTop, nPDFWidth, 70), QString("文件名称: %1").arg(inf.file_name));
    iTop+=110;

    pdfPainter->drawLine(0,iTop,nPDFWidth,iTop);   //画横线
    iTop+=60;

    fontSize = 18;
    font.setPointSize(fontSize);
    pdfPainter->setFont(font);
    pdfPainter->drawText(QRect(0,iTop, nPDFWidth, 80), Qt::AlignVCenter | Qt::AlignLeft,"叶片误差分析");
    iTop+=120;

    fontSize = 15;
    font.setPointSize(fontSize);
    pdfPainter->setFont(font);
    pdfPainter->drawText(QRect(0,iTop, nPDFWidth, 80), Qt::AlignVCenter | Qt::AlignLeft,QString("所选截面: %1").arg(inf.section));
    pdfPainter->drawText(QRect(nPDFWidth/2+100,iTop, nPDFWidth/2-100, 70),"单位：毫米");
    iTop+=150;

    fontSize = 15;
    font.setPointSize(fontSize);
    pdfPainter->setFont(font);
    list.clear(); //防止之前绘制表格使得list中已含有数据
    list<<"边"<<"加权"<<"最小偏差"<<"最大偏差"<<"形状"<<"均差"<<"标准差";
    list<<"CVX"<<inf.CVX_WEIGHT<<inf.CVX_DEVIATION_MIN<<inf.CVX_DEVIATION_MAX<<inf.CVX_SHAPE<<inf.CVX_MEAN<<inf.CVX_STANDARD;
    list<<"CCV"<<inf.CCV_WEIGHT<<inf.CCV_DEVIATION_MIN<< inf.CCV_DEVIATION_MAX<<inf.CCV_SHAPE<<inf.CCV_MEAN<<inf.CCV_STANDARD;
    list<<"LE"<<inf.LE_WEIGHT<<inf.LE_DEVIATION_MIN<<inf.LE_DEVIATION_MAX<<inf.LE_SHAPE<<inf.LE_MEAN<<inf.LE_STANDARD;
    list<<"TE"<<inf.TE_WEIGHT<<inf.TE_DEVIATION_MAX<<inf.TE_DEVIATION_MIN<<inf.TE_SHAPE<<inf.TE_MEAN<<inf.TE_STANDARD;
    pdfDrawForm(pdfPainter,iTop,0,5,7,100,font,list);
    iTop+=600;

    pdfPainter->drawLine(0,iTop,nPDFWidth,iTop);   //画横线
    iTop+=60;

    fontSize = 18;
    font.setPointSize(fontSize);
    pdfPainter->setFont(font);
    pdfPainter->drawText(QRect(0,iTop, nPDFWidth, 80), Qt::AlignVCenter | Qt::AlignLeft,"叶片截面图");
    iTop+=150;

    //获取界面图片
    int imageBorder=150;        //设置图片水平边距为150
    qRegisterMetaType<MY_BUF3D>("MY_BUF3D");

    QScreen * screen = QGuiApplication::primaryScreen();   //截取屏幕所选位置的图像
    //screen->grabWindow(containerChart->winId(),0,0,-1,-1).save("captureImg.jpg","jpg");

    //QPixmap pixmap = containerChart->grab();//containerChart存储2维图像
    QPixmap pixmap = screen->grabWindow(containerChart->winId());



    //float x = (float)(nPDFWidth-imageBorder*2)/(float)pixmap.width();
    //pixmap= pixmap.scaled(nPDFWidth-imageBorder*2, x*pixmap.height(),Qt::IgnoreAspectRatio);    //根据大小比例,来放大缩小图片

    pixmap= pixmap.scaled(nPDFWidth-imageBorder*2,nPDFHeight-iTop,Qt::KeepAspectRatio); //改动

    pdfPainter->drawPixmap((nPDFWidth-pixmap.width())/2, iTop, pixmap);  //改动

    iTop+=pixmap.height()+90;

    //if(iTop<"待设定值")
    pdfWriter->newPage();     //PDF开始新的一页

    iTop=10;

    fontSize = 22;
    font.setPointSize(fontSize);
    pdfPainter->setFont(font);
    pdfPainter->drawText(QRect( 0, iTop, iContentWidth, 90), "叶片检测结果报告",option); //QRect(int x, int y, int width, int height)



    iTop+=140;

    pdfPainter->setPen(QPen(QBrush(QColor(0,0,0)),5));
    pdfPainter->drawLine(0,iTop,nPDFWidth,iTop);                //画横线
    pdfPainter->drawLine(0,iTop+18,nPDFWidth,iTop+18);
    iTop+=60;

    fontSize = 16;
    font.setPointSize(fontSize);
    pdfPainter->setFont(font);
    pdfPainter->drawText(QRect(100,iTop, nPDFWidth, 70), "零件名称: %1");
    pdfPainter->drawText(QRect(nPDFWidth/2+100,iTop, nPDFWidth, 70),QString("检测时间: %1").arg(inf.test_time));
    iTop+=90;


    pdfPainter->drawText(QRect(100,iTop, nPDFWidth, 70), QString("所选截面: %1").arg(inf.section));
    pdfPainter->drawText(QRect(nPDFWidth/2+100,iTop, nPDFWidth, 70),QString("PARTCOUNT: %1").arg(inf.PARTCOUNT));
    iTop+=90;

    pdfPainter->drawText(QRect(100,iTop, nPDFWidth, 70), "SERIAL NUMBER: %1");
    pdfPainter->drawText(QRect(nPDFWidth/2+100,iTop, nPDFWidth, 70),"CLOCK NUMBER: %1");
    iTop+=110;

    pdfPainter->drawText(QRect(100,iTop, nPDFWidth, 70), QString("文件名称: %1").arg(inf.file_name));
    iTop+=110;

    pdfPainter->drawLine(0,iTop,nPDFWidth,iTop);   //画横线
    iTop+=60;

    fontSize = 18;
    font.setPointSize(fontSize);
    pdfPainter->setFont(font);
    pdfPainter->drawText(QRect(0,iTop, nPDFWidth, 80), Qt::AlignVCenter | Qt::AlignLeft,"叶片参数分析");
    iTop+=120;

    fontSize = 15;
    font.setPointSize(fontSize);
    pdfPainter->setFont(font);
    pdfPainter->drawText(QRect(0,iTop, nPDFWidth, 80), Qt::AlignVCenter | Qt::AlignLeft,QString("所选截面: %1").arg(inf.section));
    pdfPainter->drawText(QRect(nPDFWidth/2+100,iTop, nPDFWidth/2-100, 70),"单位：毫米");
    iTop+=120;


    fontSize = 15;
    font.setPointSize(fontSize);
    pdfPainter->setFont(font);
    list.clear(); //防止之前绘制表格使得list中已含有数据
    list<<"LE RADIUS"<<inf.LE_RADIUS;
    list<<"TE RADIUS"<<inf.TE_RADIUS;
    list<<"LE THICK"<< inf.LE_THICK;
    list<<"TE THICK"<<inf.TE_THICK;
    list<<"MAC THICK"<< inf.MAC_THICK;
    list<<"CHORD WID"<< inf.CHORD_WID;
    list<<"CHORD WID"<< inf.CHORD_WID;
    list<<"STACK T"<<inf.STACK_T;
    list<<"STACK X"<<inf.STACK_X;
    list<<"STACK Y"<< inf.STACK_Y;
    list<<"TWIST ANG"<< inf.TWIST_ANG;
    list<<"CV CONT 最小值"<<inf.CV_CONT_MIN;
    list<<"CV CONT 最大值"<<inf.CV_CONT_MAX;
    list<<"CC CONT 最小值"<<inf.CC_CONT_MIN;
    list<<"CC CONT 最大值"<<inf.CC_CONT_MAX;
    list<<"LE CONT'R 最小值"<<inf.LE_CONT_MIN;
    list<<"LE CONT'R 最大值"<<inf.LE_CONT_MAX;
    list<<"TE CONT'R 最小值"<<inf.TE_CONT_MIN;
    list<<"TE CONT'R 最大值"<<inf.TE_CONT_MAX;
    list<<"BOW 总数"<<inf.BOW_AMOUNT;
    pdfDrawForm(pdfPainter,iTop,0,20,2,100,font,list);
    iTop+=560;



    delete pdfPainter;
    delete pdfWriter;
    pdfFile.close();
}
void MainWindow::inittable(){
    table_1->horizontalHeader()->setVisible(false);//表头不可见
    table_1->verticalHeader()->setVisible(false);//表头不可见
    table_2->horizontalHeader()->setVisible(false);//表头不可见
    table_2->verticalHeader()->setVisible(false);//表头不可见
    table_1->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置单元格不可编辑
    table_2->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置单元格不可编辑
    table_1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏滚动条
    //table_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏滚动条

    table_1->setColumnCount(5);//header5列
    table_1->setRowCount(8);//header8行
    table_1->setShowGrid(true);                //设置显示格子线
    //table_1->horizontalHeader()->setStretchLastSection(true);    //设置充满表宽度
    table_1->resizeColumnsToContents();//根据内容自动改变列的大小
    table_1->resizeRowsToContents();//根据内容自动改变行的大小
    table_1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);// 自适应所有列，让它布满空间
    table_1->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);// 自适应所有行，让它布满空间
    table_1->setItem(0,0,new QTableWidgetItem("LE RADIUS"));
    table_1->setItem(0,1,new QTableWidgetItem("TE RADIUS"));
    table_1->setItem(0,2,new QTableWidgetItem("LE THICK"));
    table_1->setItem(0,3,new QTableWidgetItem("TE THICK"));
    table_1->setItem(0,4,new QTableWidgetItem("MAC THICK"));
    table_1->setItem(2,0,new QTableWidgetItem("CHORD WID"));
    table_1->setItem(2,1,new QTableWidgetItem("CHORD WID"));
    table_1->setItem(2,2,new QTableWidgetItem("STACK T"));
    table_1->setItem(2,3,new QTableWidgetItem("STACK X"));
    table_1->setItem(2,4,new QTableWidgetItem("STACK Y"));
    table_1->setItem(4,0,new QTableWidgetItem("TWIST ANG"));
    table_1->setItem(4,1,new QTableWidgetItem("CV 最小值"));
    table_1->setItem(4,2,new QTableWidgetItem("CV 最大值"));
    table_1->setItem(4,3,new QTableWidgetItem("CC 最小值"));
    table_1->setItem(4,4,new QTableWidgetItem("CC 最大值"));
    table_1->setItem(6,0,new QTableWidgetItem("LE 最小值"));
    table_1->setItem(6,1,new QTableWidgetItem("LE 最大值"));
    table_1->setItem(6,2,new QTableWidgetItem("TE 最小值"));
    table_1->setItem(6,3,new QTableWidgetItem("TE 最大值"));
    table_1->setItem(6,4,new QTableWidgetItem("BOW 总数"));

    table_2->setColumnCount(7);//设置列数
    table_2->setRowCount(5);//设置行数
    table_2->setShowGrid(true);                //设置显示格子线
    table_2->resizeColumnsToContents();//根据内容自动改变列的大小
    table_2->resizeRowsToContents();//根据内容自动改变行的大小
    table_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);// 自适应所有列，让它布满空间
    table_2->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);// 自适应所有行，让它布满空间
    table_2->setItem(0,0,new QTableWidgetItem("边"));
    table_2->setItem(0,1,new QTableWidgetItem("加权"));
    table_2->setItem(0,2,new QTableWidgetItem("最小偏差"));
    table_2->setItem(0,3,new QTableWidgetItem("最大偏差"));
    table_2->setItem(0,4,new QTableWidgetItem("形状"));
    table_2->setItem(0,5,new QTableWidgetItem("均差"));
    table_2->setItem(0,6,new QTableWidgetItem("标准差"));
    table_2->setItem(1,0,new QTableWidgetItem("CVX"));
    table_2->setItem(2,0,new QTableWidgetItem("CCV"));
    table_2->setItem(3,0,new QTableWidgetItem("LE"));
    table_2->setItem(4,0,new QTableWidgetItem("TE"));




}
void MainWindow::getinformation(){
    inf.section=select_z->currentText();


    inf.LE_RADIUS="0.228";
    inf.TE_RADIUS="0.325";
    inf.LE_THICK="1.441";
    inf.TE_THICK="1.125";
    inf.MAC_THICK="3.439";
    inf.CHORD_WID="45.022";
    inf.CHORD_WID="78.596";
    inf.STACK_T="0.264";
    inf.STACK_X="0.012";
    inf.STACK_Y="0.081";
    inf.TWIST_ANG="-0.074";
    inf.CV_CONT_MAX="-0.039";
    inf.CV_CONT_MIN="0.035";
    inf.CC_CONT_MAX="0.031";
    inf.CC_CONT_MIN="-0.063";
    inf.LE_CONT_MAX="0.005";
    inf.LE_CONT_MIN="-0.085";
    inf.TE_CONT_MAX="0.084";
    inf.TE_CONT_MIN="-0.013";
    inf.BOW_AMOUNT="N/A";



    inf.CVX_WEIGHT='1';
    inf.CVX_DEVIATION_MAX="0.0349";
    inf.CVX_DEVIATION_MIN="-0.0388";
    inf.CVX_SHAPE="0.0736";
    inf.CVX_MEAN="0.0000";
    inf.CVX_STANDARD="0.0100";
    inf.CCV_WEIGHT="1";
    inf.CCV_DEVIATION_MAX="0.0314";
    inf.CCV_DEVIATION_MIN="-0.0630";
    inf.CCV_SHAPE="0.0944";
    inf.CCV_MEAN="-0.0031";
    inf.CCV_STANDARD="0.0140";
    inf.LE_WEIGHT='1';
    inf.LE_DEVIATION_MAX="0.0048";
    inf.LE_DEVIATION_MIN="-0.0853";
    inf.LE_SHAPE="0.0901";
    inf.LE_MEAN="-0.0270";
    inf.LE_STANDARD="0.0203";
    inf.TE_WEIGHT="1";
    inf.TE_DEVIATION_MAX="0.0841";
    inf.TE_DEVIATION_MIN="-0.0132";
    inf.TE_SHAPE="0.0972";
    inf.TE_MEAN="0.0239";
    inf.TE_STANDARD="0.0276";

}
void MainWindow::settable(){

    getinformation();
    table_1->setItem(1,0,new QTableWidgetItem(inf.LE_RADIUS));
    table_1->setItem(1,1,new QTableWidgetItem(inf.TE_RADIUS));
    table_1->setItem(1,2,new QTableWidgetItem(inf.LE_THICK));
    table_1->setItem(1,3,new QTableWidgetItem(inf.TE_THICK));
    table_1->setItem(1,4,new QTableWidgetItem(inf.MAC_THICK));
    table_1->setItem(3,0,new QTableWidgetItem(inf.CHORD_WID));
    table_1->setItem(3,1,new QTableWidgetItem(inf.CHORD_WID));
    table_1->setItem(3,2,new QTableWidgetItem(inf.STACK_T));
    table_1->setItem(3,3,new QTableWidgetItem(inf.STACK_X));
    table_1->setItem(3,4,new QTableWidgetItem(inf.STACK_Y));
    table_1->setItem(5,0,new QTableWidgetItem(inf.TWIST_ANG));
    table_1->setItem(5,1,new QTableWidgetItem(inf.CV_CONT_MAX));
    table_1->setItem(5,2,new QTableWidgetItem(inf.CV_CONT_MIN));
    table_1->setItem(5,3,new QTableWidgetItem(inf.CC_CONT_MAX));
    table_1->setItem(5,4,new QTableWidgetItem(inf.CC_CONT_MIN));
    table_1->setItem(7,0,new QTableWidgetItem(inf.LE_CONT_MAX));
    table_1->setItem(7,1,new QTableWidgetItem(inf.LE_CONT_MIN));
    table_1->setItem(7,2,new QTableWidgetItem(inf.TE_CONT_MAX));
    table_1->setItem(7,3,new QTableWidgetItem(inf.TE_CONT_MIN));
    table_1->setItem(7,4,new QTableWidgetItem(inf.BOW_AMOUNT));



    table_2->setItem(1,1,new QTableWidgetItem(inf.CVX_WEIGHT));
    table_2->setItem(1,2,new QTableWidgetItem(inf.CVX_DEVIATION_MAX));
    table_2->setItem(1,3,new QTableWidgetItem(inf.CVX_DEVIATION_MIN));
    table_2->setItem(1,4,new QTableWidgetItem(inf.CVX_SHAPE));
    table_2->setItem(1,5,new QTableWidgetItem(inf.CVX_MEAN));
    table_2->setItem(1,6,new QTableWidgetItem(inf.CVX_STANDARD));
    table_2->setItem(2,1,new QTableWidgetItem(inf.CCV_WEIGHT));
    table_2->setItem(2,2,new QTableWidgetItem(inf.CCV_DEVIATION_MAX));
    table_2->setItem(2,3,new QTableWidgetItem(inf.CCV_DEVIATION_MIN));
    table_2->setItem(2,4,new QTableWidgetItem(inf.CCV_SHAPE));
    table_2->setItem(2,5,new QTableWidgetItem(inf.CCV_MEAN));
    table_2->setItem(2,6,new QTableWidgetItem(inf.CCV_STANDARD));
    table_2->setItem(3,1,new QTableWidgetItem(inf.LE_WEIGHT));
    table_2->setItem(3,2,new QTableWidgetItem(inf.LE_DEVIATION_MAX));
    table_2->setItem(3,3,new QTableWidgetItem(inf.LE_DEVIATION_MIN));
    table_2->setItem(3,4,new QTableWidgetItem(inf.LE_SHAPE));
    table_2->setItem(3,5,new QTableWidgetItem(inf.LE_MEAN));
    table_2->setItem(3,6,new QTableWidgetItem(inf.LE_STANDARD));
    table_2->setItem(4,1,new QTableWidgetItem(inf.TE_WEIGHT));
    table_2->setItem(4,2,new QTableWidgetItem(inf.TE_DEVIATION_MAX));
    table_2->setItem(4,3,new QTableWidgetItem(inf.TE_DEVIATION_MIN));
    table_2->setItem(4,4,new QTableWidgetItem(inf.TE_SHAPE));
    table_2->setItem(4,5,new QTableWidgetItem(inf.TE_MEAN));
    table_2->setItem(4,6,new QTableWidgetItem(inf.TE_STANDARD));
}

