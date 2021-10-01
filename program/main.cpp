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
    w->initMap();
    w->initCharts();
//    QSize screenSize = w->m_3Dgraph->screen()->size();
//    w->container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.5));
//    w->container->setMaximumSize(screenSize);
//    w->container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    w->container->setFocusPolicy(Qt::StrongFocus);
    QWidget *widget = new QWidget;
    widget->showMaximized();
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QVBoxLayout *vLayout = new QVBoxLayout();
    QVBoxLayout *vLayoutLeft = new QVBoxLayout();
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
    QLineEdit *lineedit_visualpath = new QLineEdit();
    hlayout_openfile_and_visualpath->addWidget(lineedit_visualpath);
    hlayout_openfile_and_visualpath->addWidget(openfile);

    QPushButton *visualize = new QPushButton(widget);
    visualize->setText(QStringLiteral("visualie"));
    vLayout->addLayout(hlayout_openfile_and_visualpath);
    vLayout->addWidget(visualize);

    QComboBox *select_z = new QComboBox();
    select_z->addItem(QStringLiteral("z=30"));
    select_z->addItem(QStringLiteral("z=60"));
    select_z->addItem(QStringLiteral("z=90"));
    vLayout->addWidget(new QLabel("选择截面"));
    vLayout->addWidget(select_z);

    QVBoxLayout *aboutReport = new QVBoxLayout();
    QHBoxLayout *com_and_exp = new QHBoxLayout();
    QPushButton *compare = new QPushButton();
    QPushButton *exportFile = new QPushButton();
    QTableWidget *table_1 = new QTableWidget();
    QTableWidget *table_2 = new QTableWidget();

    aboutReport->addLayout(com_and_exp);
    com_and_exp->addWidget(compare);
    compare->setText("compare");
    com_and_exp->addWidget(exportFile);
    exportFile->setText("export");
    aboutReport->addWidget(new QLabel("表格1"));
    aboutReport->addWidget(table_1);
    aboutReport->addWidget(new QLabel("表格2"));
    aboutReport->addWidget(table_2);
    vLayout->addLayout(aboutReport);
//    vLayout->setStretchFactor(openfile,1);
//    vLayout->setStretchFactor(visualize,1);

    QObject::connect(visualize,&QPushButton::clicked,w, &MainWindow::visualize);
    QObject::connect(openfile,&QPushButton::clicked,w, &MainWindow::openfile);
    widget->show();
    return a.exec();
}
