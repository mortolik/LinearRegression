#include "mainwindow.h"
#include <QMenuBar>
#include <QStatusBar>
#include <QAction>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_model(new MultivariateLinearRegressionModel(this))
    , m_regressionWidget(new RegressionWidget(this))
{
    setWindowTitle("Анализ линейной регрессии");
    resize(1024, 768);

    setupStatusBar();
    initializeModel();

    setCentralWidget(m_regressionWidget);
}

MainWindow::~MainWindow()
{
    // No action needed
}

void MainWindow::setupStatusBar()
{
    statusBar()->showMessage("Готово");
}

void MainWindow::initializeModel()
{
    m_model->setTrueCoefficients(2.5, -1.3, 10.0, 1.5);
    m_model->generateSample(100, 0.0, 10.0, 0.0, 10.0);
    m_model->train();
    m_model->generateTestSample(50, 0.0, 10.0, 0.0, 10.0);

    m_regressionWidget->setModel(m_model);

    connect(m_model, &MultivariateLinearRegressionModel::rSquaredChanged, [this]() {
        statusBar()->showMessage(QString("R² = %1").arg(m_model->rSquared(), 0, 'f', 3));
    });
}
