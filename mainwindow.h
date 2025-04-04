#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Regression
{
class MultivariateLinearRegressionModel;
class RegressionWidget;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    Regression::MultivariateLinearRegressionModel* m_model;
    Regression::RegressionWidget* m_regressionWidget;
};

#endif // MAINWINDOW_H
