#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "LinearRegressionModel.hpp"
#include "LinearRegressionWidget.hpp"
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

    LinearRegressionModel* m_linearModel;
    Regression::MultivariateLinearRegressionModel* m_multivariateModel;
    LinearRegressionWidget* m_linearWidget;
    Regression::RegressionWidget* m_multivariateWidget;
};

#endif // MAINWINDOW_H
