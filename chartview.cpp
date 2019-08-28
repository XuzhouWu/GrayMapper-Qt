#include "chartview.h"
#include <QtCore/QtMath>
#include <QtCore/QDebug>

QT_CHARTS_USE_NAMESPACE

ChartView::ChartView(QWidget *parent)
    : QChartView(new QChart(), parent),
      mScatter(0),
      isDragging(false),
      mPointDragMode(SINGLE)
{
    // 开启抗锯齿

    setRenderHint(QPainter::Antialiasing);

    // 初始化散点图
    mScatter = new QScatterSeries();
    for(auto i = 0; i < POINT_COUNT; i++) {
        *mScatter<<QPointF(double(i)/(POINT_COUNT-1), double(i)/(POINT_COUNT-1));
    }

    // 初始化曲线
    mLine = new QLineSeries();
    for(auto i = 0; i < 256; i++) {
        *mLine<<QPointF(double(i)/255, double(i)/255);
    }

    chart()->addSeries(mLine);
    chart()->addSeries(mScatter);

    chart()->createDefaultAxes();
    chart()->legend()->hide();
    chart()->axisX()->setRange(0, 1);
    chart()->axisY()->setRange(0, 1);
    connect(mScatter, SIGNAL(pressed(QPointF)), this, SLOT(handlePressedPoint(QPointF)));
    connect(mScatter, SIGNAL(released(QPointF)), this, SLOT(handleReleasedPoint(QPointF)));
}

ChartView::~ChartView()
{
}

void ChartView::mouseMoveEvent(QMouseEvent *event)
{
    if(isDragging) {
        // 获取鼠标当前坐标
        auto const widgetPos = event->localPos();

        // 坐标转换
        auto const scenePos = mapToScene(QPoint(static_cast<int>(widgetPos.x()), static_cast<int>(widgetPos.y())));
        auto const chartItemPos = chart()->mapFromScene(scenePos);
        auto const valueGivenSeries = chart()->mapToValue(chartItemPos);

        // 修改当前拖动点的坐标
        qreal newX = valueGivenSeries.x();
        qreal newY = valueGivenSeries.y();

        if(mPointDragMode == SINGLE) {
            dragSingle(newX, newY);
        } else if(mPointDragMode == UNITED) {
            dragUnited(newX, newY);
        }
        emit pointsChanged(mScatter->pointsVector());
    }

}

void ChartView::update()
{
    emit pointsChanged(mScatter->pointsVector());
}

void ChartView::reset()
{
    QVector<QPointF> scatterPoints = mScatter->pointsVector();
    QVector<QPointF> linePoints = mLine->pointsVector();
    for(auto i = 0; i < POINT_COUNT; i++) {
        scatterPoints[i].setX(double(i)/(POINT_COUNT-1));
        scatterPoints[i].setY(double(i)/(POINT_COUNT-1));
    }
    for(auto i = 0; i < 256; i++) {
        linePoints[i].setX(double(i)/255);
        linePoints[i].setY(double(i)/255);
    }
    mScatter->replace(scatterPoints);
    mLine->replace(linePoints);
    update();
}

QVector<QPointF> ChartView::getPoints()
{
    return mScatter->pointsVector();
}

void ChartView::onCurveFinished(const QVector<uchar> &mapArray)
{
    QVector<QPointF> points = mLine->pointsVector();
    for(auto i = 0; i < points.size(); i++) {
        points[i].setY(double(mapArray[i])/255);
    }
    mLine->replace(points);
}

void ChartView::onPointDragModeChanged(int pointDragMode)
{
    mPointDragMode = static_cast<PointDragMode>(pointDragMode);
}

void ChartView::handlePressedPoint(const QPointF &point)
{
    // 获取当前拖动点的索引
    qreal distance(INT_MAX);
    int pointCount = mScatter->pointsVector().size();
    while(pointCount--) {
        qreal currentDistance = qSqrt((mScatter->at(pointCount).x() - point.x())
                                      * (mScatter->at(pointCount).x() - point.x())
                                      + (mScatter->at(pointCount).y() - point.y())
                                      * (mScatter->at(pointCount).y() - point.y()));
        if (currentDistance < distance) {
            distance = currentDistance;
            currentPointIndex = pointCount;
        }
    }

    // 表明已选中点
    isDragging = true;
}

void ChartView::dragSingle(qreal &newX, qreal &newY)
{
    // 修正x的范围
    switch(currentPointIndex) {
    case 0:
        if(newX > mScatter->at(currentPointIndex+1).x()) {
            newX = mScatter->at(currentPointIndex+1).x() - 0.001;
        } else if(newX < 0) {
            newX = 0.001;
        }
        break;
    case (POINT_COUNT-1):
        if(newX > 1) {
            newX = 1 - 0.001;
        } else if(newX < mScatter->at(currentPointIndex-1).x()) {
            newX = mScatter->at(currentPointIndex-1).x() + 0.001;
        }
        break;
    default:
        if(newX > mScatter->at(currentPointIndex+1).x()) {
            newX = mScatter->at(currentPointIndex+1).x() - 0.001;
        } else if(newX < mScatter->at(currentPointIndex-1).x()) {
            newX = mScatter->at(currentPointIndex-1).x() + 0.001;
        }
    }
    // 修正y的范围
    if(newY > 1) {
        newY = 1;
    } else if(newY < 0) {
        newY = 0;
    }
    //更新
    mScatter->replace(currentPointIndex,newX,newY);
}

void ChartView::dragUnited(qreal &newX, qreal &newY)
{
    QVector<QPointF> points = mScatter->pointsVector();

    // 修正x
    if(newX >= 1) {
        newX = 0.999;
    } else if(newX <= 0) {
        newX = 0.001;
    }
    qreal oldX = points[currentPointIndex].x();
    points[currentPointIndex].setX(newX);

    for(auto i = 0; i < POINT_COUNT; i++) {
        if(i < currentPointIndex){
            qreal unitedX = points[i].x()/oldX*newX;
            points[i].setX(unitedX);
        } else if (i > currentPointIndex) {
            qreal unitedX = (points[i].x() - oldX) / (1 - oldX) * (1 - newX) + newX;
            points[i].setX(unitedX);
        }
    }

    // 修正y的范围
    if(newY > 1) {
        newY = 1;
    } else if(newY < 0) {
        newY = 0;
    }
    points[currentPointIndex].setY(newY);

    mScatter->replace(points);
}

void ChartView::handleReleasedPoint(const QPointF &point)
{
    Q_UNUSED(point);
    // 解除选中
    isDragging = false;
//    emit pointsChanged(mScatter->pointsVector());
}

