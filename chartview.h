#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#pragma execution_character_set("utf-8")

#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

enum PointDragMode{
    SINGLE = 1,
    UNITED
};

const int POINT_COUNT = 9;

class ChartView : public QChartView
{
    Q_OBJECT

public:
    ChartView(QWidget *parent = 0);
    ~ChartView();
    void mouseMoveEvent(QMouseEvent *event) override;
    void update();
    void reset();

public slots:
    void onCurveFinished(const QVector<int> &mapArray);
    void onPointDragModeChanged(int pointDragMode);

signals:
    void pointsChanged(const QVector<QPointF> &points);

private slots:
    void handleReleasedPoint(const QPointF &point);
    void handlePressedPoint(const QPointF &point);

private:
    void dragSingle(qreal &newX, qreal &newY);
    void dragUnited(qreal &newX, qreal &newY);


    QScatterSeries      *mScatter;
    QLineSeries         *mLine;

    bool                isDragging;

    int                 currentPointIndex;

    PointDragMode            mPointDragMode;

};

#endif // CHARTVIEW_H
