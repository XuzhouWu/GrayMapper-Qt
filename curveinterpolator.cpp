#include "curveinterpolator.h"

CurveInterpolator::CurveInterpolator(QObject *parent) : QObject(parent)
{

}

void CurveInterpolator::setMethod(QString method)
{

}

void CurveInterpolator::interpolate(const QVector<QPointF> &points)
{
    QVector<double> x;
    QVector<double> y;
    for(auto point : points) {
        x.append(point.x());
        y.append(point.y());
    }
    double res[256];
//    fitting(x.data(),y.data(),9,res);
    double xout[256];
    for(auto i = 0; i < 256; i++) {
        xout[i] = double(i)/255;
    }
    spl2(x.data(), y.data(), 9, 0, 0, xout, 256, res);

    QVector<int> mapArray;
    for(auto i = 0; i < 256; i++) {
        auto tmp = 255*res[i];
        if(tmp>255)
            tmp = 255;
        else if(tmp < 0)
            tmp = 0;
        mapArray.append(int(tmp));
    }
    emit CurveChangged(mapArray);
}

void CurveInterpolator::onMethodChanged(QString interpolationMethod)
{

}

void CurveInterpolator::fitting(double x[], double y[], int n, double des[])
{
    double index1, index2, h[10], m[10];
    m[0] = m[n - 1] = 0; m[1] = 1;
    index1 = (y[2] - y[1]) / (x[2] - x[1]);
    for (int i = 0; i < n; i++)
    {
        h[i] = x[i + 1] - x[i];
    }
    for (int i = 1; i < n; i++)
    {
        index1 = (y[i + 1] - y[i]) / (x[i + 1] - x[i]);
        index2 = (y[i] - y[i - 1]) / (x[i] - x[i - 1]);
        m[i + 1] = (6 * (index1 - index2) - h[i - 1] * m[i - 1] - 2 * (h[i - 1] + h[i])*m[i]) / h[i];
    }
    //得到插值之后的方程系数
    double a[10], b[10], c[10], d[10];
    for (int i = 0; i < n; i++)
    {
        c[i] = m[i] / 2;
        d[i] = (m[i + 1] - m[i]) / (6 * h[i]);
        b[i] = (y[i + 1] - y[i]) / h[i] - h[i] * m[i] / 2 - h[i] * (m[i + 1] - m[i]) / 6;
        a[i] = y[i + 1] - h[i] * b[i] - h[i] * h[i] * c[i] - h[i] * h[i] * h[i] * d[i];
    }
    //输出是长度为256的向量
    double x1[256];
    int index = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 38; j++)
        {
            index = i * 8 + j;
            x1[index] = h[i] / 38 * j + x[i];
            des[index] = a[i] + b[i] * x1[index] + c[i] * x1[index] * x1[index] + d[i] * x1[index] * x1[index] * x1[index];
        }
    }
}

int CurveInterpolator::spl2(double x[], double y[], int n, double ddy1, double ddyn, double t[], int m, double z[])
{
    int i,j;
    double h0,h1(0),alpha,beta,*s,*dy;
    s= new double[n];
    dy = new double[n];

    dy[0]=-0.5;
    h0=x[1]-x[0];
    s[0]=3.0*(y[1]-y[0])/(2.0*h0)-ddy1*h0/4.0;

    for (j=1;j<=n-2;j++)
    {
            h1=x[j+1]-x[j];
            alpha=h0/(h0+h1);
            beta=(1.0-alpha)*(y[j]-y[j-1])/h0;
            beta=3.0*(beta+alpha*(y[j+1]-y[j])/h1);
            dy[j]=-alpha/(2.0+(1.0-alpha)*dy[j-1]);
            s[j]=(beta-(1.0-alpha)*s[j-1]);
            s[j]=s[j]/(2.0+(1.0-alpha)*dy[j-1]);
            h0=h1;
    }
    dy[n-1]=(3.0*(y[n-1]-y[n-2])/h1+ddyn*h1/2.0-s[n-2])/(2.0+dy[n-2]);
    for (j=n-2;j>=0;j--)        dy[j]=dy[j]*dy[j+1]+s[j];
    for (j=0;j<=n-2;j++)        s[j]=x[j+1]-x[j];

    for (j=0;j<=m-1;j++)
    {
            if (t[j]>=x[n-1]) i=n-2;
            else
            {
                    i=0;
                    while (t[j]>x[i+1]) i=i+1;
            }
            h1=(x[i+1]-t[j])/s[i];
            h0=h1*h1;
            z[j]=(3.0*h0-2.0*h0*h1)*y[i];
            z[j]=z[j]+s[i]*(h0-h0*h1)*dy[i];

            h1=(t[j]-x[i])/s[i];
            h0=h1*h1;
            z[j]=z[j]+(3.0*h0-2.0*h0*h1)*y[i+1];
            z[j]=z[j]-s[i]*(h0-h0*h1)*dy[i+1];
    }
    delete[] s;
    delete[] dy;
    return 0;
}

