#include "LinearRegressionWidget.hpp"
#include <QPainter>
#include <QLineEdit>
#include <QCheckBox>
#include <QChartView>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>

LinearRegressionWidget::LinearRegressionWidget(QWidget* parent)
    : QWidget(parent), m_model(nullptr)
{
    setupUI();
}

void LinearRegressionWidget::setModel(LinearRegressionModel* model)
{
    m_model = model;
    if (m_model)
    {
        connect(m_model, &LinearRegressionModel::modelChanged, this, &LinearRegressionWidget::updateChart);
    }
}

void LinearRegressionWidget::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    m_tabWidget = new QTabWidget(this);

    m_chartView = new QtCharts::QChartView(this);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    QWidget* chartTab = new QWidget(this);
    QVBoxLayout* chartLayout = new QVBoxLayout(chartTab);
    chartLayout->addWidget(m_chartView);
    m_tabWidget->addTab(chartTab, "График");

    m_tableWidget = new QTableWidget(this);
    m_tableWidget->setColumnCount(3);
    m_tableWidget->setHorizontalHeaderLabels(QStringList() << "x" << "y_real" << "y_pred");

    QWidget* tableTab = new QWidget(this);
    QVBoxLayout* tableLayout = new QVBoxLayout(tableTab);
    tableLayout->addWidget(m_tableWidget);
    m_tabWidget->addTab(tableTab, "Таблица");

    layout->addWidget(m_tabWidget);

    QFormLayout* form = new QFormLayout();
    m_aInput = new QLineEdit("1.0", this);
    m_bInput = new QLineEdit("0.0", this);
    m_sigmaInput = new QLineEdit("1.0", this);
    m_t1Input = new QLineEdit("0.0", this);
    m_t2Input = new QLineEdit("10.0", this);
    m_sampleSizeInput = new QLineEdit("100", this);
    m_sequentialXCheck = new QCheckBox("Use sequential X", this);
    m_runButton = new QPushButton("Run", this);

    form->addRow("a:", m_aInput);
    form->addRow("b:", m_bInput);
    form->addRow("Sigma:", m_sigmaInput);
    form->addRow("t1:", m_t1Input);
    form->addRow("t2:", m_t2Input);
    form->addRow("Sample size:", m_sampleSizeInput);
    form->addRow(m_sequentialXCheck);

    layout->addLayout(form);
    layout->addWidget(m_runButton);

    connect(m_runButton, &QPushButton::clicked, this, &LinearRegressionWidget::onRunClicked);
}

void LinearRegressionWidget::onRunClicked()
{
    if (!m_model) return;

    m_model->setTrueCoefficients(
        m_aInput->text().toDouble(),
        m_bInput->text().toDouble(),
        m_sigmaInput->text().toDouble()
        );

    m_model->generateSample(
        qMax(2, m_sampleSizeInput->text().toInt()),
        m_t1Input->text().toDouble(),
        m_t2Input->text().toDouble(),
        m_sequentialXCheck->isChecked()
        );

    m_model->train();
    m_model->generateTestSample(
        10, // размер тестовой выборки
        m_t1Input->text().toDouble(),
        m_t2Input->text().toDouble(),
        m_sequentialXCheck->isChecked()
        );

    const auto& testData = m_model->testData();
    m_tableWidget->setRowCount(testData.size());

    int row = 0;
    for (const auto& [x, y_real] : testData)
    {
        double y_pred = m_model->predict(x);

        m_tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(x, 'f', 4)));
        m_tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(y_real, 'f', 4)));
        m_tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(y_pred, 'f', 4)));

        ++row;
    }

    m_tableWidget->resizeColumnsToContents();
}

void LinearRegressionWidget::updateChart()
{
    auto* chart = new QtCharts::QChart();
    chart->setTitle("Linear Regression");

    auto* scatter = new QtCharts::QScatterSeries();
    for (const auto& [x, y] : m_model->trainingData())
    {
        scatter->append(x, y);
    }

    auto* line = new QtCharts::QLineSeries();
    if (!m_model->trainingData().isEmpty())
    {
        double x1 = m_model->trainingData().first().first;
        double x2 = m_model->trainingData().last().first;
        line->append(x1, m_model->predict(x1));
        line->append(x2, m_model->predict(x2));
    }

    chart->addSeries(scatter);
    chart->addSeries(line);
    chart->createDefaultAxes();

    m_chartView->setChart(chart);
}
