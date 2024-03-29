﻿#ifndef GRAYSCALEMAPPER_H
#define GRAYSCALEMAPPER_H

#include <QObject>
#include <QVector>
#include <QImage>

class GrayScaleMapper : public QObject
{
    Q_OBJECT
public:
    explicit GrayScaleMapper(QObject *parent = 0);

    QImage mapGrayScale(const QImage &imageIn);

signals:

public slots:
    void updateMap(const QVector<uchar> &mapArray);

private:
    QVector<uchar> mMapArray;
};

#endif // GRAYSCALEMAPPER_H
