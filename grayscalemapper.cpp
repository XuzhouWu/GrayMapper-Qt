#include "grayscalemapper.h"

GrayScaleMapper::GrayScaleMapper(QObject *parent)
    : QObject(parent)
{

}

QImage GrayScaleMapper::mapGrayScale(const QImage &imageIn)
{
//    return imageIn;
    QImage imageOut(imageIn);
    int height = imageIn.height();
    int width = imageIn.width();
    uint    pixelVal = 0;
    for(int y = 0; y<height; y++){
        for(int x = 0; x<width; x++){
            pixelVal = qGreen(imageIn.pixel(x,y));
            pixelVal = mMapArray[pixelVal];
            imageOut.setPixel(x,y, qRgb(pixelVal, pixelVal, pixelVal));
        }
    }
    return imageOut;
}

void GrayScaleMapper::updateMap(const QVector<int> &mapArray)
{
    mMapArray = mapArray;
}
