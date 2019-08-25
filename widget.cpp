#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    mChart = new ChartView(this);
    mInterpolator = new CurveInterpolator(this);


    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(mChart);
    this->setLayout(layout);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_dragModeBtn_clicked()
{

}

void Widget::on_resetCurveBtn_clicked()
{

}
