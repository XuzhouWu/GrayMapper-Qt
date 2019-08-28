#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QImage>
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>
#include <math.h>
#include "chartview.h"
#include "grayscalemapper.h"
#include "curveinterpolator.h"
#include "imagemanager.h"

namespace Ui {
class MainWidget;
}

const int PLOT_INTERVAL = 25;

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

signals:
    void pointDragModeChanged(int mode);

private slots:
    void on_dragModeBtn_clicked();

    void on_startConvertionBtn_clicked();

    void plotUpdate();

    void fpsUpdate();

    void onImageLoadFinished();

    void on_loadImageBtn_clicked();

    void on_resetCurveBtn_clicked();

private:
    Ui::MainWidget      *ui;
    QTimer              *plotTimer;
    QTimer              *fpsTimer;

    ImageManager        *mImageManager;
    GrayScaleMapper     *mMapper;
    CurveInterpolator   *mInterpolator;

    bool                imageLoadedFlag;
    int                 imageIndex;
    int                 fpsCounter;
};

#endif // MAINWIDGET_H
