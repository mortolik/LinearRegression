#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MultivariateLinearRegressionModel.hpp"
#include "RegressionWidget.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    MultivariateLinearRegressionModel* m_model;
    RegressionWidget* m_regressionWidget;
};

#endif // MAINWINDOW_H
