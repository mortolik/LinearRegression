#include "RegressionWidget.hpp"
#include <QVBoxLayout>
#include <QLabel>
#include <QtCharts/QValueAxis>

RegressionWidget::RegressionWidget(QWidget* parent)
    : QWidget(parent)
    , m_model(nullptr)
    , m_scatterChartView(new QChartView(this))
    , m_residualChartView(new QChartView(this))
{
    setupUI();
}

void RegressionWidget::setModel(MultivariateLinearRegressionModel* model)
{
    if (m_model == model)
        return;

    if (m_model)
        disconnect(m_model, nullptr, this, nullptr);

    m_model = model;

    if (m_model)
    {
        connect(m_model, &MultivariateLinearRegressionModel::modelChanged,
                this, &RegressionWidget::updateCharts);
        connect(m_model, &MultivariateLinearRegressionModel::coefficientsUpdated,
                this, &RegressionWidget::updateCharts);
    }

    updateCharts();
}

void RegressionWidget::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_scatterChartView->setRenderHint(QPainter::Antialiasing);
    m_residualChartView->setRenderHint(QPainter::Antialiasing);

    layout->addWidget(m_scatterChartView);
    layout->addWidget(m_residualChartView);

    setLayout(layout);
}

void RegressionWidget::updateCharts()
{
    if (!m_model || m_model->trainingData().isEmpty())
    {
        m_scatterChartView->chart()->removeAllSeries();
        m_residualChartView->chart()->removeAllSeries();
        return;
    }

    createScatterPlot();
    createResidualPlot();
}

void RegressionWidget::createScatterPlot()
{
    QChart* chart = m_scatterChartView->chart();
    chart->removeAllSeries();
    chart->setTitle("Данные и регрессионная модель");

    QScatterSeries* series = new QScatterSeries();
    series->setName("Обучающие данные");
    series->setMarkerSize(8.0);

    const auto& data = m_model->trainingData();
    for (const auto& point : data)
    {
        series->append(point.first.first, point.second);
    }

    chart->addSeries(series);
    chart->createDefaultAxes();
}

void RegressionWidget::createResidualPlot()
{
    QChart* chart = m_residualChartView->chart();
    chart->removeAllSeries();
    chart->setTitle("Остатки модели");

    QScatterSeries* series = new QScatterSeries();
    series->setName("Остатки");
    series->setMarkerSize(8.0);

    const auto& data = m_model->trainingData();
    for (const auto& point : data)
    {
        double predicted = m_model->predict(point.first.first, point.first.second);
        series->append(predicted, point.second - predicted);
    }

    chart->addSeries(series);
    chart->createDefaultAxes();
}
