#include "RegressionWidget.hpp"
#include "MultivariateLinearRegressionModel.hpp"
#include "qtabwidget.h"

#include <QtDataVisualization/Q3DSurface>
#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDoubleValidator>
#include <QtCharts/QValueAxis>
#include <QTableWidget>
#include <QScatter3DSeries>

namespace Regression
{
RegressionWidget::RegressionWidget(QWidget* parent)
    : QWidget(parent)
    , m_model(nullptr)
    // , m_surface(new Q3DSurface())
    // , m_surfaceSeries(new QSurface3DSeries())
    //, m_residualChartView(new QChartView(this))
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

    m_tabWidget = new QTabWidget(this);

    m_surfaceGraph = new Q3DSurface();
    m_surfaceContainer = QWidget::createWindowContainer(m_surfaceGraph, this);
    m_surfaceContainer->setMinimumSize(400, 400);
    m_surfaceContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_scatterGraph = new Q3DScatter();
    m_scatterContainer = QWidget::createWindowContainer(m_scatterGraph, this);
    m_scatterContainer->setMinimumSize(400, 400);
    m_scatterContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget* graphTab = new QWidget(this);
    QHBoxLayout* graphLayout = new QHBoxLayout(graphTab);
    graphLayout->addWidget(m_surfaceContainer);
    graphLayout->addWidget(m_scatterContainer);
    m_tabWidget->addTab(graphTab, "3D график");


    // m_residualChartView->setRenderHint(QPainter::Antialiasing);
    // m_residualChartView->setMinimumSize(800, 300);
    // m_residualChartView->setMaximumSize(800, 300);
    // m_residualChartView->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);



    m_tableWidget = new QTableWidget(this);
    m_tableWidget->setColumnCount(5);
    m_tableWidget->setHorizontalHeaderLabels({"x1", "x2", "y_real", "y_pred", "ошибка"});


    QWidget* tableTab = new QWidget(this);
    QVBoxLayout* tableLayout = new QVBoxLayout(tableTab);
    tableLayout->addWidget(m_tableWidget);
    m_tabWidget->addTab(tableTab, "Таблица");

    mainLayout->addWidget(m_tabWidget);

    //mainLayout->addWidget(m_residualChartView);

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
    m_sigmaInput->setValidator(new QDoubleValidator(0.1, 100.0, 2, this));
    m_t1Input = new QLineEdit("0.0", this);
    m_t2Input = new QLineEdit("10.0", this);
    m_s1Input = new QLineEdit("0.0", this);
    m_s2Input = new QLineEdit("10.0", this);
    m_sampleSizeInput = new QLineEdit("100", this);
    m_testSizeInput = new QLineEdit("10", this);

    m_sampleSizeInput->setValidator(new QIntValidator(2, 10000000, this));
    m_testSizeInput->setValidator(new QIntValidator(1, 100000000, this));

    inputLayout->addRow("Размер обучающей выборки:", m_sampleSizeInput);
    inputLayout->addRow("Размер тестовой выборки:", m_testSizeInput);


    inputLayout->addRow("x1: от t1 =", m_t1Input);
    inputLayout->addRow("x1: до t2 =", m_t2Input);
    inputLayout->addRow("x2: от s1 =", m_s1Input);
    inputLayout->addRow("x2: до s2 =", m_s2Input);
    m_rLabel = new QLabel("R²: ?", this);
    mainLayout->addWidget(m_rLabel);

    m_sigma2Label = new QLabel("σ²: ?", this);
    mainLayout->addWidget(m_sigma2Label);

    m_coefficientsLabel = new QLabel("Коэффициенты: ?", this);
    mainLayout->addWidget(m_coefficientsLabel);

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
    m_rLabel->setText(QString("R²: %1").arg(m_model->rSquared(), 0, 'f', 4));
    m_sigma2Label->setText(QString("σ²: %1").arg(m_model->estimatedSigmaSquared(), 0, 'f', 4));


    int n = m_sampleSizeInput->text().toInt();
    int m = m_testSizeInput->text().toInt();
    double t1 = m_t1Input->text().toDouble();
    double t2 = m_t2Input->text().toDouble();
    double s1 = m_s1Input->text().toDouble();
    double s2 = m_s2Input->text().toDouble();

    m_model->generateSample(n, t1, t2, s1, s2);
    m_model->train();
    QVector<double> coeffs = m_model->coefficients();
    QString coeffText = QString("Коэффициенты: a₁ predict = %1, a₂ predict = %2, b predict = %3")
                            .arg(coeffs[0], 0, 'f', 4)
                            .arg(coeffs[1], 0, 'f', 4)
                            .arg(coeffs[2], 0, 'f', 4);
    m_coefficientsLabel->setText(coeffText);
    createSurfaceGraph(); // создаём плоскость
    createScatterGraph(); // создаём точки

    m_model->generateTestSample(m, t1, t2, s1, s2);

    const auto& testData = m_model->testData();
    m_tableWidget->setRowCount(testData.size());

    int row = 0;
    for (const auto& [inputs, y_real] : testData) {
        double x1 = inputs.first;
        double x2 = inputs.second;
        double y_pred = m_model->predict(x1, x2);        
        double error = std::abs(y_real - y_pred);

        m_tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(x1, 'f', 4)));
        m_tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(x2, 'f', 4)));
        m_tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(y_real, 'f', 4)));
        m_tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(y_pred, 'f', 4)));
        m_tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(error, 'f', 4)));

        ++row;
    }

    m_tableWidget->resizeColumnsToContents();
}
void RegressionWidget::createSurfaceGraph()
{
    m_surfaceGraph->removeSeries(m_surfaceGraph->seriesList().value(0, nullptr));

    QSurfaceDataArray* surfaceData = new QSurfaceDataArray;
    int gridSize = 30;
    surfaceData->reserve(gridSize);

    double t1 = m_t1Input->text().toDouble();
    double t2 = m_t2Input->text().toDouble();
    double s1 = m_s1Input->text().toDouble();
    double s2 = m_s2Input->text().toDouble();

    for (int i = 0; i < gridSize; ++i)
    {
        QSurfaceDataRow* row = new QSurfaceDataRow(gridSize);
        double x1 = t1 + (t2 - t1) * i / (gridSize - 1);
        for (int j = 0; j < gridSize; ++j)
        {
            double x2 = s1 + (s2 - s1) * j / (gridSize - 1);
            double y = m_model->predict(x1, x2);
            (*row)[j].setPosition(QVector3D(x1, y, x2));
        }
        *surfaceData << row;
    }

    QSurface3DSeries* surfaceSeries = new QSurface3DSeries;
    surfaceSeries->dataProxy()->resetArray(surfaceData);
    surfaceSeries->setDrawMode(QSurface3DSeries::DrawSurface);
    surfaceSeries->setFlatShadingEnabled(false);
    m_surfaceGraph->addSeries(surfaceSeries);
}

void RegressionWidget::createScatterGraph()
{
    m_scatterGraph->removeSeries(m_scatterGraph->seriesList().value(0, nullptr));

    const auto& data = m_model->trainingData();
    QScatterDataArray* scatterData = new QScatterDataArray;
    scatterData->reserve(data.size());

    for (const auto& [inputs, y] : data)
    {
        float x1 = inputs.first;
        float x2 = inputs.second;
        float z = y;
        scatterData->append(QScatterDataItem(QVector3D(x1, z, x2)));
    }

    QScatter3DSeries* scatterSeries = new QScatter3DSeries;
    scatterSeries->dataProxy()->addItems(*scatterData);
    scatterSeries->setMesh(QAbstract3DSeries::MeshSphere);
    scatterSeries->setItemSize(0.1f);
    scatterSeries->setBaseColor(Qt::red);

    m_scatterGraph->addSeries(scatterSeries);
}

// void RegressionWidget::createResidualPlot()
// {
//     //QChart* chart = m_residualChartView->chart();
//     chart->removeAllSeries();
//     chart->setTitle("Остатки модели");

//     QScatterSeries* series = new QScatterSeries();
//     series->setName("Остатки");
//     series->setMarkerSize(8.0);

//     const auto& data = m_model->trainingData();
//     for (const auto& point : data)
//     {
//         double predicted = m_model->predict(point.first.first, point.first.second);
//         series->append(predicted, point.second - predicted);
//     }

//     chart->addSeries(series);
//     chart->createDefaultAxes();
// }
}
