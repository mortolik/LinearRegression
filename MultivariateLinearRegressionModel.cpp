#include "MultivariateLinearRegressionModel.hpp"

#include <cmath>
#include <QDebug>
#include <QRandomGenerator>
namespace Regression
{
MultivariateLinearRegressionModel::MultivariateLinearRegressionModel(QObject* parent)
    : QObject(parent)
    , m_a1(0)
    , m_a2(0)
    , m_b(0)
    , m_sigma(1)
    , m_estimatedA1(0)
    , m_estimatedA2(0)
    , m_estimatedB(0)
    , m_rSquared(0)
{
}

void MultivariateLinearRegressionModel::setTrueCoefficients(double a1, double a2, double b, double sigma)
{
    m_a1 = a1;
    m_a2 = a2;
    m_b = b;
    m_sigma = sigma;

    emit modelChanged();
}

void MultivariateLinearRegressionModel::generateSample(int n, double t1, double t2, double s1, double s2)
{
    m_trainingData.clear();
    auto* generator = QRandomGenerator::global();

    for (int i = 0; i < n; ++i)
    {
        double x1 = t1 + generator->bounded(1.0) * (t2 - t1);
        double x2 = s1 + generator->bounded(1.0) * (s2 - s1);
        double noise = generator->bounded(2.0 * m_sigma) - m_sigma;
        double y = trueFunction(x1, x2) + noise;
        m_trainingData.append({{x1, x2}, y});
    }

    emit modelChanged();
}

void MultivariateLinearRegressionModel::train()
{
    int n = m_trainingData.size();
    if (n < 3)
    {
        return;
    }

    double sumX1 = 0;
    double sumX2 = 0;
    double sumY = 0;
    double sumX1X1 = 0;
    double sumX2X2 = 0;
    double sumX1X2 = 0;
    double sumX1Y = 0;
    double sumX2Y = 0;

    for (const auto& point : m_trainingData)
    {
        double x1 = point.first.first;
        double x2 = point.first.second;
        double y = point.second;

        sumX1 += x1;
        sumX2 += x2;
        sumY += y;

        sumX1X1 += x1 * x1;
        sumX2X2 += x2 * x2;
        sumX1X2 += x1 * x2;

        sumX1Y += x1 * y;
        sumX2Y += x2 * y;
    }

    double det = n * sumX1X1 * sumX2X2 + sumX1 * sumX2 * sumX1X2 * 2
                 - sumX1 * sumX1 * sumX2X2 - sumX2 * sumX2 * sumX1X1 - n * sumX1X2 * sumX1X2;

    if (det == 0)
    {
        qWarning() << "Matrix is singular";
        return;
    }

    m_estimatedA1 = (sumX2X2 * (n * sumX1Y - sumX1 * sumY) + sumX1X2 * (sumX2 * sumY - n * sumX2Y)
                     + sumX2 * (sumX1 * sumX2Y - sumX2 * sumX1Y)) / det;

    m_estimatedA2 = (sumX1X1 * (n * sumX2Y - sumX2 * sumY) + sumX1X2 * (sumX1 * sumY - n * sumX1Y)
                     + sumX1 * (sumX2 * sumX1Y - sumX1 * sumX2Y)) / det;

    m_estimatedB = (sumY - m_estimatedA1 * sumX1 - m_estimatedA2 * sumX2) / n;

    double ssTotal = 0;
    double ssResidual = 0;
    double meanY = sumY / n;

    for (const auto& point : m_trainingData)
    {
        double y = point.second;
        double predicted = predict(point.first.first, point.first.second);
        ssTotal += (y - meanY) * (y - meanY);
        ssResidual += (y - predicted) * (y - predicted);
    }

    m_rSquared = 1 - (ssResidual / ssTotal);

    int p = 3;
    m_estimatedSigmaSquared = ssResidual / (n - p);

    emit coefficientsUpdated();
    emit rSquaredChanged();
}

double MultivariateLinearRegressionModel::predict(double x1, double x2) const
{
    return m_estimatedA1 * x1 + m_estimatedA2 * x2 + m_estimatedB;
}

QVector<double> MultivariateLinearRegressionModel::coefficients() const
{
    return {m_estimatedA1, m_estimatedA2, m_estimatedB};
}

double MultivariateLinearRegressionModel::rSquared() const
{
    return m_rSquared;
}

void MultivariateLinearRegressionModel::generateTestSample(int m, double t1, double t2, double s1, double s2)
{
    m_testData.clear();
    auto* generator = QRandomGenerator::global();

    for (int i = 0; i < m; ++i)
    {
        double x1 = t1 + generator->bounded(1.0) * (t2 - t1);
        double x2 = s1 + generator->bounded(1.0) * (s2 - s1);
        double noise = generator->bounded(2.0 * m_sigma) - m_sigma;
        double y = trueFunction(x1, x2) + noise;
        m_testData.append({{x1, x2}, y});
    }

    emit modelChanged();
}

QVector<QPair<QPair<double, double>, double>> MultivariateLinearRegressionModel::trainingData() const
{
    return m_trainingData;
}

QVector<QPair<QPair<double, double>, double>> MultivariateLinearRegressionModel::testData() const
{
    return m_testData;
}

double MultivariateLinearRegressionModel::trueFunction(double x1, double x2) const
{
    return m_a1 * x1 + m_a2 * x2 + m_b;
}
}
