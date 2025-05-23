#pragma once

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>

QT_FORWARD_DECLARE_CLASS(QLabel);
QT_FORWARD_DECLARE_CLASS(QLineEdit);
QT_FORWARD_DECLARE_CLASS(QTabWidget);
QT_FORWARD_DECLARE_CLASS(QPushButton);
QT_FORWARD_DECLARE_CLASS(QTableWidget);

QT_CHARTS_USE_NAMESPACE

#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurface3DSeries>
#include <QtDataVisualization/QScatter3DSeries>
#include <QtDataVisualization/Q3DScatter>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
    using namespace QtDataVisualization;

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
    void createSurfacePlot();
    //void createResidualPlot();

    MultivariateLinearRegressionModel* m_model;

    Q3DSurface* m_surfaceGraph = nullptr;
    Q3DScatter* m_scatterGraph = nullptr;
    QWidget* m_surfaceContainer = nullptr;
    QWidget* m_scatterContainer = nullptr;

    QLineEdit* m_a1Input;
    QLineEdit* m_a2Input;
    QLineEdit* m_bInput;
    QLineEdit* m_sigmaInput;
    QLineEdit* m_t1Input;
    QLineEdit* m_t2Input;
    QLineEdit* m_s1Input;
    QLineEdit* m_s2Input;

    QLabel* m_rLabel;
    QLabel* m_sigma2Label;
    QLabel* m_coefficientsLabel;

    QLineEdit* m_sampleSizeInput;
    QLineEdit* m_testSizeInput;

    QPushButton* m_runButton;

    QTabWidget* m_tabWidget;
    QTableWidget* m_tableWidget;

    void createSurfaceGraph();
    void createScatterGraph();
};
}
