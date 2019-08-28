#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QDebug>
#include <windows.h>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget),
    mImageManager(NULL),
    mMapper(NULL),
    mInterpolator(NULL),
    imageLoadedFlag(false),
    imageIndex(0),
    fpsCounter(0)
{
    ui->setupUi(this);

    // 设置刷新定时器
    plotTimer = new QTimer;
    plotTimer->setInterval(PLOT_INTERVAL);
    plotTimer->setTimerType(Qt::PreciseTimer);
    connect(plotTimer, SIGNAL(timeout()), this, SLOT(plotUpdate()));

    // 设置帧率计时器
    fpsTimer = new QTimer;
    fpsTimer->setInterval(250);
    fpsTimer->setTimerType(Qt::PreciseTimer);
    connect(fpsTimer, SIGNAL(timeout()), this, SLOT(fpsUpdate()));

    //
    mImageManager = new ImageManager;
    mInterpolator = new CurveInterpolator;
    mMapper = new GrayScaleMapper;

    // 各部件连接
    connect(this, SIGNAL(pointDragModeChanged(int)), ui->curveWidget, SLOT(onPointDragModeChanged(int)));

    connect(mImageManager, SIGNAL(loadFinished()), this, SLOT(onImageLoadFinished()));
    connect(mInterpolator, SIGNAL(CurveChangged(QVector<uchar>)), mMapper, SLOT(updateMap(QVector<uchar>)));
    connect(mInterpolator, SIGNAL(CurveChangged(QVector<uchar>)), ui->curveWidget, SLOT(onCurveFinished(QVector<uchar>)));
    connect(ui->curveWidget, SIGNAL(pointsChanged(QVector<QPointF>)), mInterpolator, SLOT(interpolate(QVector<QPointF>)));

    ui->curveWidget->update();
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::on_dragModeBtn_clicked()
{
    static bool state = false;

    if (state) {
        emit pointDragModeChanged(SINGLE);
        ui->dragModeBtn->setText("SINGLE");
        ui->dragModeBtn->setStyleSheet("QPushButton{color: rgb(255, 255, 255);background:rgb(0, 170, 255);border:1px solid grey;border-radius: 10px;}");
        state = !state;
    } else {
        emit pointDragModeChanged(UNITED);
        ui->dragModeBtn->setText("UNITED");
        ui->dragModeBtn->setStyleSheet("QPushButton{color: rgb(255, 255, 255);background:rgb(237, 28, 36);border:1px solid grey;border-radius: 10px;}");
        state = !state;
    }
}

void MainWidget::on_startConvertionBtn_clicked()
{
    static bool state = false;
    if(imageLoadedFlag) {
        if (state) {
            plotTimer->stop();
            fpsTimer->stop();
            ui->startConvertionBtn->setText("开始转换");
            ui->startConvertionBtn->setStyleSheet("QPushButton{color: rgb(255, 255, 255);background:rgb(0, 170, 255);border:1px solid grey;border-radius: 10px;}");
            state = !state;
        } else {
            plotTimer->start();
            fpsTimer->start();
            ui->startConvertionBtn->setText("停止转换");
            ui->startConvertionBtn->setStyleSheet("QPushButton{color: rgb(255, 255, 255);background:rgb(237, 28, 36);border:1px solid grey;border-radius: 10px;}");
            state = !state;
        }
    } else {
        QMessageBox::information(this,"提示","图片未加载");
    }
}

void MainWidget::plotUpdate()
{
    QImage rawImage = mImageManager->getImage(imageIndex);
    imageIndex = (++imageIndex) % mImageManager->imageNums();

    QImage image = mMapper->mapGrayScale(rawImage);
    ui->imageLabel->setPixmap(QPixmap::fromImage(image).scaled(ui->imageLabel->size(),Qt::KeepAspectRatio));
    //    ui->imageLabel->setScaledContents(true);
    fpsCounter++;
}

void MainWidget::fpsUpdate()
{
    static LARGE_INTEGER freq={0};
    static LARGE_INTEGER lastTime = {0};
    static LARGE_INTEGER currentTime = {0};

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&currentTime);

    double interval = (currentTime.QuadPart - lastTime.QuadPart)/double(freq.QuadPart);
    double fps = (fpsCounter/interval);
    fpsCounter = 0;
    lastTime = currentTime;

    ui->fpsLabel->setText(QString::number(fps));
}

void MainWidget::onImageLoadFinished()
{
    imageLoadedFlag = true;
    QMessageBox::information(this,"提示","载入完成");
}

void MainWidget::on_loadImageBtn_clicked()
{
    QString file_path = QFileDialog::getExistingDirectory(this, "请选择文件路径...", "./");
    if(file_path.isEmpty())
    {
        return;
    }

    mImageManager->loadFromLocal(file_path);
}

void MainWidget::on_resetCurveBtn_clicked()
{
    ui->curveWidget->reset();
}
