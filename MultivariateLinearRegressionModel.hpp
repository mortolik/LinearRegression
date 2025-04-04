#pragma once

#include <QPair>
#include <QObject>
#include <QVector>

namespace Regression
{
class MultivariateLinearRegressionModel : public QObject
{
    Q_OBJECT

public:
    explicit MultivariateLinearRegressionModel(QObject* parent = nullptr);

    void setTrueCoefficients(double a1, double a2, double b, double sigma);
    void generateSample(int n, double t1, double t2, double s1, double s2);
    void train();
    double predict(double x1, double x2) const;
    QVector<double> coefficients() const;
    double rSquared() const;
    void generateTestSample(int m, double t1, double t2, double s1, double s2);
    QVector<QPair<QPair<double, double>, double>> trainingData() const;
    QVector<QPair<QPair<double, double>, double>> testData() const;

signals:
    void modelChanged();
    void coefficientsUpdated();
    void rSquaredChanged();

private:
    double m_a1;
    double m_a2;
    double m_b;
    double m_sigma;
    double m_estimatedA1;
    double m_estimatedA2;
    double m_estimatedB;
    QVector<QPair<QPair<double, double>, double>> m_trainingData;
    QVector<QPair<QPair<double, double>, double>> m_testData;
    double m_rSquared;

    double trueFunction(double x1, double x2) const;
};
}
