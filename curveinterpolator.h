#ifndef CURVEINTERPOLATOR_H
#define CURVEINTERPOLATOR_H

#include <QObject>
#include <QString>
#include <QVector>

class CurveInterpolator : public QObject
{
    Q_OBJECT
public:
    explicit CurveInterpolator(QObject *parent = 0);

signals:
    void CurveChangged(const QVector<uchar> &mapArray);

public slots:
    void setMethod(QString method);

    void interpolate(const QVector<QPointF> &points);

    void onMethodChanged(QString interpolationMethod);

private:
    void fitting(double x[], double y[], int n, double des[]);
    int spl2(double x[], double y[], int n, double ddy1, double ddyn, double t[], int m, double z[]);
    QString mMethod;
};

#endif // CURVEINTERPOLATOR_H
