#include "mainwindow.h"

#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_model(new MultivariateLinearRegressionModel(this))
    , m_regressionWidget(new RegressionWidget(this))
{
    m_regressionWidget->setModel(m_model);
    setCentralWidget(m_regressionWidget);
    setWindowTitle("Многомерная линейная регрессия");
    resize(800, 600);
}
