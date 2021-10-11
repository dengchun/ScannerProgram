#include "mainwindow.h"

#include <QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QImage>
#include <QtGui/QScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *w = new MainWindow;
//    QSize screenSize = w->m_3Dgraph->screen()->size();
//    w->container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.5));
//    w->container->setMaximumSize(screenSize);
//    w->container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    w->container->setFocusPolicy(Qt::StrongFocus);

    QString qss;
    QFile qssFile(":/ElegantDark.qss");
    qssFile.open(QFile::ReadOnly);
    if(qssFile.isOpen())
    {
        qDebug()<<"hhh";
        qss = QLatin1String(qssFile.readAll());
        qApp->setStyleSheet(qss);
        qssFile.close();
    }

    QWidget *widget = new QWidget;
    widget->showMaximized();
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QVBoxLayout *vLayout = new QVBoxLayout();
    QHBoxLayout *vLayoutLeft = new QHBoxLayout();
    hLayout->addLayout(vLayoutLeft);
    vLayoutLeft->addWidget(w->container);
    vLayoutLeft->addWidget(w->containerChart);
    hLayout->addLayout(vLayout);
    hLayout->setStretchFactor(vLayoutLeft,2);
    hLayout->setStretchFactor(vLayout,1);
    vLayoutLeft->setStretchFactor(w->container,1);
    vLayoutLeft->setStretchFactor(w->containerChart,1);

    QHBoxLayout *hlayout_openfile_and_visualpath = new QHBoxLayout();
    QPushButton *openfile = new QPushButton(widget);
    openfile->setText(QStringLiteral("openfile"));

    w->lineedit_visualpath->setText(w->f);
    hlayout_openfile_and_visualpath->addWidget(w->lineedit_visualpath);
    hlayout_openfile_and_visualpath->addWidget(openfile);

    vLayout->addLayout(hlayout_openfile_and_visualpath);
    vLayout->addWidget(new QLabel("选择截面"));
    vLayout->addWidget(w->select_z);

    QVBoxLayout *aboutReport = new QVBoxLayout();
    QHBoxLayout *com_and_exp = new QHBoxLayout();
    QPushButton *compare = new QPushButton();
    QPushButton *exportFile = new QPushButton();

    aboutReport->addLayout(com_and_exp);
    com_and_exp->addWidget(compare);
    compare->setText("compare");
    com_and_exp->addWidget(exportFile);
    exportFile->setText("export");
    aboutReport->addWidget(new QLabel("表格1"));
    aboutReport->addWidget(w->table_1);
    aboutReport->addWidget(new QLabel("表格2"));
    aboutReport->addWidget(w->table_2);
    vLayout->addLayout(aboutReport);

    QObject::connect(openfile,&QPushButton::clicked,w, &MainWindow::openfile);
    QObject::connect(exportFile,&QPushButton::clicked,w, &MainWindow::export_report);
    QObject::connect(compare,&QPushButton::clicked,w, &MainWindow::settable);



    QWidget *secondWidget = new QWidget();
    QPushButton * secondButton = new QPushButton(secondWidget);



    QTabWidget *tWidget = new QTabWidget();
    tWidget->addTab(widget,"Tab1");
    tWidget->addTab(secondWidget,"Tab2");
    tWidget->showMaximized();



    tWidget->show();
    return a.exec();
}
