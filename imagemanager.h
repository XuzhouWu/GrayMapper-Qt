#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <QObject>
#include <QVector>
#include <QImage>
#include <QDir>

class ImageManager : public QObject
{
    Q_OBJECT
public:
    explicit ImageManager(QObject *parent = 0);

    const QImage &getImage(int index);
    int imageNums();

signals:
    void loadFinished();

public slots:
    void loadFromLocal(QString localFolderPath);

private:
    QVector<QImage> imageList;
};

#endif // IMAGEMANAGER_H
