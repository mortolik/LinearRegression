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
    // 1. Создаем модели
    m_linearModel = new LinearRegressionModel(this);
    m_multivariateModel = new Regression::MultivariateLinearRegressionModel(this);

    // 2. Создаем виджеты
    m_linearWidget = new LinearRegressionWidget();
    m_linearWidget->setModel(m_linearModel);

    m_multivariateWidget = new Regression::RegressionWidget();
    m_multivariateWidget->setModel(m_multivariateModel);

    // 3. Настраиваем вкладки
    QTabWidget* tabWidget = new QTabWidget(this);
    tabWidget->addTab(m_linearWidget, "Одномерная регрессия");
    tabWidget->addTab(m_multivariateWidget, "Многомерная регрессия");

    setCentralWidget(tabWidget);
    resize(600, 600);
}
