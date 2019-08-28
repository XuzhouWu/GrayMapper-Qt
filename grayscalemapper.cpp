#include "grayscalemapper.h"
#include <intrin.h>
#include <QDebug>
#include<cstring>

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
//    int imgFormat = imageIn.format();
//    uint    pixelVal = 0;
//    for(int y = 0; y<height; y++){
//        for(int x = 0; x<width; x++){
//            pixelVal = qGreen(imageIn.pixel(x,y));
//            pixelVal = mMapArray[pixelVal];
//            imageOut.setPixel(x,y, qRgb(pixelVal, pixelVal, pixelVal));
//        }
//    }

//    for(int y = 0; y < height; y++) {
//        uchar* line = imageOut.scanLine(y);

//        for(int i = 0; i < width/4; i++) {
//            int idx = 16*i;
//            __m128i *data = (__m128i*)(line + idx);

//            *data = _mm_set_epi8(mMapArray[line[idx+12]],mMapArray[line[idx+13]],mMapArray[line[idx+13]],line[idx+13],
//                    mMapArray[line[idx+8]],mMapArray[line[idx+9]],mMapArray[line[idx+9]],line[idx+9],
//                    mMapArray[line[idx+4]],mMapArray[line[idx+5]],mMapArray[line[idx+5]],line[idx+5],
//                    mMapArray[line[idx+0]],mMapArray[line[idx+1]],mMapArray[line[idx+1]],line[idx+1]);
//        }
//    }

    for(int y = 0; y < height; y++) {
        uchar* line = imageOut.scanLine(y);
        for(int i = 0; i < width; i++) {
            memset(line+4*i,mMapArray[line[(i<<2)+1]],3);
        }
    }

    return imageOut;
}

void GrayScaleMapper::updateMap(const QVector<uchar> &mapArray)
{
    mMapArray = mapArray;
}
