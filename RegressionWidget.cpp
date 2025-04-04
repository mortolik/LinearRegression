#include "RegressionWidget.hpp"
#include "MultivariateLinearRegressionModel.hpp"

#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDoubleValidator>
#include <QtCharts/QValueAxis>

namespace Regression
{
RegressionWidget::RegressionWidget(QWidget* parent)
    : QWidget(parent)
    , m_model(nullptr)
    , m_scatterChartView(new QChartView(this))
    , m_residualChartView(new QChartView(this))
    , m_a1Input(new QLineEdit("2.5", this))
    , m_a2Input(new QLineEdit("-1.3", this))
    , m_bInput(new QLineEdit("10.0", this))
    , m_sigmaInput(new QLineEdit("1.5", this))
    , m_runButton(new QPushButton("Запустить", this))
{
    setupUI();
}

void RegressionWidget::setModel(MultivariateLinearRegressionModel* model)
{
    m_model = model;
}

void RegressionWidget::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);

    m_scatterChartView->setRenderHint(QPainter::Antialiasing);
    m_residualChartView->setRenderHint(QPainter::Antialiasing);

    mainLayout->addWidget(m_scatterChartView);
    mainLayout->addWidget(m_residualChartView);

    QFormLayout* inputLayout = new QFormLayout();
    inputLayout->addRow("Коэффициент a1:", m_a1Input);
    inputLayout->addRow("Коэффициент a2:", m_a2Input);
    inputLayout->addRow("Свободный член b:", m_bInput);
    inputLayout->addRow("Стандартное отклонение σ:", m_sigmaInput);

    connect(m_runButton, &QPushButton::clicked, this, &RegressionWidget::onRunClicked);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_runButton);

    mainLayout->addLayout(inputLayout);
    mainLayout->addLayout(buttonLayout);

    m_a1Input->setValidator(new QDoubleValidator(this));
    m_a2Input->setValidator(new QDoubleValidator(this));
    m_bInput->setValidator(new QDoubleValidator(this));
    m_sigmaInput->setValidator(new QDoubleValidator(0.1, 10.0, 2, this));
}

void RegressionWidget::onRunClicked()
{
    if (!m_model) return;

    m_model->setTrueCoefficients(
        m_a1Input->text().toDouble(),
        m_a2Input->text().toDouble(),
        m_bInput->text().toDouble(),
        m_sigmaInput->text().toDouble()
        );

    m_model->generateSample(100, 0.0, 10.0, 0.0, 10.0);
    m_model->train();

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
}
