#pragma once

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>

QT_FORWARD_DECLARE_CLASS(QLineEdit);
QT_FORWARD_DECLARE_CLASS(QTabWidget);
QT_FORWARD_DECLARE_CLASS(QPushButton);
QT_FORWARD_DECLARE_CLASS(QTableWidget);

QT_CHARTS_USE_NAMESPACE

namespace Regression
{
class MultivariateLinearRegressionModel;
class RegressionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RegressionWidget(QWidget* parent = nullptr);
    void setModel(MultivariateLinearRegressionModel* model);

private slots:
    void onRunClicked();

private:
    void setupUI();
    void createScatterPlot();
    void createResidualPlot();

    MultivariateLinearRegressionModel* m_model;
    QChartView* m_scatterChartView;
    QChartView* m_residualChartView;

    QLineEdit* m_a1Input;
    QLineEdit* m_a2Input;
    QLineEdit* m_bInput;
    QLineEdit* m_sigmaInput;
    QPushButton* m_runButton;

    QTabWidget* m_tabWidget;
    QTableWidget* m_tableWidget;

};
}
