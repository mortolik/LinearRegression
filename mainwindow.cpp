#include "mainwindow.h"
#include "RegressionWidget.hpp"
#include "MultivariateLinearRegressionModel.hpp"

#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    m_linearModel = new LinearRegressionModel(this);
    m_multivariateModel = new Regression::MultivariateLinearRegressionModel(this);

    m_linearWidget = new LinearRegressionWidget();
    m_linearWidget->setModel(m_linearModel);

    m_multivariateWidget = new Regression::RegressionWidget();
    m_multivariateWidget->setModel(m_multivariateModel);

    QTabWidget* tabWidget = new QTabWidget(this);
    tabWidget->addTab(m_linearWidget, "Одномерная регрессия");
    tabWidget->addTab(m_multivariateWidget, "Многомерная регрессия");

    tabWidget->setCurrentIndex(1);
    setCentralWidget(tabWidget);
    resize(600, 600);
}
