#ifndef LINEARREGRESSIONMODEL_H
#define LINEARREGRESSIONMODEL_H

#include <QObject>
#include <QVector>
#include <utility>

class LinearRegressionModel : public QObject
{
    Q_OBJECT
public:
    explicit LinearRegressionModel(QObject* parent = nullptr);

    void setTrueCoefficients(double a, double b, double sigma);
    void generateSample(int n, double t1, double t2, bool sequentialX = false);
    void train();
    double predict(double x) const;
    double rSquared() const;

    const QVector<std::pair<double, double>>& trainingData() const;

    void generateTestSample(int m, double t1, double t2, bool sequentialX);
    const QVector<std::pair<double, double>>& testData() const;

signals:
    void modelChanged();

private:
    double m_a;
    double m_b;
    double m_sigma;
    double m_estimatedA;
    double m_estimatedB;
    double m_rSquared;
    QVector<std::pair<double, double>> m_trainingData;
    QVector<std::pair<double, double>> m_testData;
};

#endif // LINEARREGRESSIONMODEL_H
