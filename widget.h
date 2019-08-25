#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QBoxLayout>
#include "chartview.h"
#include "curveinterpolator.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);

    ~Widget();

signals:
    void pointDragModeChanged(int mode);

private slots:
    void on_dragModeBtn_clicked();

    void on_resetCurveBtn_clicked();

private:
    Ui::Widget          *ui;
    ChartView           *mChart;
    CurveInterpolator   *mInterpolator;
};

#endif // WIDGET_H
