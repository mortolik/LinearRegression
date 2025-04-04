#pragma once

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include "MultivariateLinearRegressionModel.hpp"

QT_CHARTS_USE_NAMESPACE

    class RegressionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RegressionWidget(QWidget* parent = nullptr);
    void setModel(MultivariateLinearRegressionModel* model);

private slots:
    void updateCharts();

private:
    void setupUI();
    void createScatterPlot();
    void createResidualPlot();

    MultivariateLinearRegressionModel* m_model;
    QChartView* m_scatterChartView;
    QChartView* m_residualChartView;
};
