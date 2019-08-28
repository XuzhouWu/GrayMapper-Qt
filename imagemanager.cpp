#include "imagemanager.h"

ImageManager::ImageManager(QObject *parent) : QObject(parent)
{

}

const QImage &ImageManager::getImage(int index)
{
    return imageList[index];
}

int ImageManager::imageNums()
{
    return imageList.size();
}

void ImageManager::loadFromLocal(QString localFolderPath)
{
    QDir dir(localFolderPath);
    if (!dir.exists())
    {
        return;
    }
    dir.setSorting(QDir::Name | QDir::LocaleAware);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QStringList filters;
    filters << "*.png";
    dir.setNameFilters(filters);
    QStringList allImageNameList = dir.entryList();
    if(allImageNameList.count() <= 0)
    {
        return;
    }
    for (int i = 1; i<= allImageNameList.count(); ++i)
    {

        QString new_path = localFolderPath+"/"+QString::number(i)+".png";
        QImage img = QImage(new_path).convertToFormat(QImage::Format_ARGB32);
        imageList.append(img);
    }
    emit loadFinished();
}
