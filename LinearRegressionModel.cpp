#include "LinearRegressionModel.hpp"
#include <QtMath>
#include <QRandomGenerator>

LinearRegressionModel::LinearRegressionModel(QObject* parent)
    : QObject(parent), m_a(1.0), m_b(0.0), m_sigma(1.0),
    m_estimatedA(0.0), m_estimatedB(0.0), m_rSquared(0.0) {}

void LinearRegressionModel::setTrueCoefficients(double a, double b, double sigma)
{
    m_a = a;
    m_b = b;
    m_sigma = sigma > 0 ? sigma : 1.0;
    emit modelChanged();
}

void LinearRegressionModel::generateSample(int n, double t1, double t2, bool sequentialX)
{
    m_trainingData.clear();
    if (n <= 0) return;

    auto* generator = QRandomGenerator::global();
    for (int i = 0; i < n; ++i) {
        double x = sequentialX ? (i + 1) : (t1 + generator->bounded(t2 - t1));
        double y = m_a * x + m_b + generator->bounded(m_sigma * 2) - m_sigma;
        m_trainingData.append({x, y});
    }
    emit modelChanged();
}

void LinearRegressionModel::train()
{
    if (m_trainingData.size() < 2) return;

    double sumX = 0, sumY = 0, sumXY = 0, sumXX = 0;
    for (const auto& [x, y] : m_trainingData)
    {
        sumX += x;
        sumY += y;
        sumXY += x * y;
        sumXX += x * x;
    }

    double n = m_trainingData.size();
    double denom = n * sumXX - sumX * sumX;
    if (denom == 0) return;

    m_estimatedA = (n * sumXY - sumX * sumY) / denom;
    m_estimatedB = (sumY - m_estimatedA * sumX) / n;

    double yMean = sumY / n;
    double ssTotal = 0, ssResidual = 0;
    for (const auto& [x, y] : m_trainingData)
    {
        double predicted = predict(x);
        ssTotal += (y - yMean) * (y - yMean);
        ssResidual += (y - predicted) * (y - predicted);
    }
    m_rSquared = 1 - (ssResidual / ssTotal);
}

double LinearRegressionModel::predict(double x) const
{
    return m_estimatedA * x + m_estimatedB;
}

double LinearRegressionModel::rSquared() const
{
    return m_rSquared;
}

const QVector<std::pair<double, double>>& LinearRegressionModel::trainingData() const
{
    return m_trainingData;
}

void LinearRegressionModel::generateTestSample(int m, double t1, double t2, bool sequentialX)
{
    m_testData.clear();
    if (m_trainingData.isEmpty() || m_sigma <= 0 || m_a == 0) return;

    auto* generator = QRandomGenerator::global();
    for (int i = 0; i < m; ++i)
    {
        double x = sequentialX ? (m_trainingData.size() + i + 1) : (t1 + generator->bounded(t2 - t1));
        double y = m_a * x + m_b + generator->bounded(m_sigma * 2) - m_sigma;
        m_testData.append({x, y});
    }
    emit modelChanged();
}

const QVector<std::pair<double, double>>& LinearRegressionModel::testData() const
{
    return m_testData;
}

