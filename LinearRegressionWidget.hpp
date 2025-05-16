#ifndef LINEARREGRESSIONWIDGET_H
#define LINEARREGRESSIONWIDGET_H

#include <QWidget>
#include "LinearRegressionModel.hpp"
#include "qtabwidget.h"
#include <qchartview.h>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
class QCheckBox;
class QTableWidget;
QT_END_NAMESPACE

class LinearRegressionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LinearRegressionWidget(QWidget* parent = nullptr);
    void setModel(LinearRegressionModel* model);

private slots:
    void onRunClicked();

private:
    void setupUI();
    void updateChart();

    LinearRegressionModel* m_model;
    QtCharts::QChartView* m_chartView;
    QLineEdit* m_aInput;
    QLineEdit* m_bInput;
    QLineEdit* m_sigmaInput;
    QLineEdit* m_t1Input;
    QLineEdit* m_t2Input;
    QLineEdit* m_sampleSizeInput;
    QCheckBox* m_sequentialXCheck;
    QPushButton* m_runButton;

    QTabWidget* m_tabWidget;
    QTableWidget* m_tableWidget;
};

#endif // LINEARREGRESSIONWIDGET_H
