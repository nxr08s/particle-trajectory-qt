#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QVector3D>
#include <qmath.h>

class PaintWidget : public QWidget
{
    Q_OBJECT
    
    QList<QVector3D> points;    // массив с точками для отрисовки

    // постоянные значения, никак не изменяются
    const double q = 1.602E-19;   // заряд частицы (электрона)
    const double m = 9.109E-31;   // масса частицы(электрона)
    const double B = 0.01;        // магнитная индукция поля

    const double xScale = 0.1;      // подобранный масштаб X, изменять не рекомендуется
    const double yzScale = 1E10;    // подобранный масштаб Y, изменять не рекомендуется

    // изменяемые значения для расчета
    double angle = 60;  // угол между начальной скоростю и вектором магнитной индукции
    double v0 = 5;      // начальная скорость (в м/с)
    double r;           // радиус окружности / спирали
    double h;           // расстояние между витками спирали
    int simTime = 1000; // время наблюдения
    double xSpeed;      // скорость по оси X

    // изменяемые пользователем значения сцены
    double viewAngle;       // угол обзора сцены (при 90 - вид сбоку, при 0 - вид вдоль)
    double userScale;       // пользовательский масштаб всей сцены
    int userXOffset;        // пользовательское смещение сцены по оси X
    int userYOffset;        // пользовательское смещение сцены по оси Y
    bool longAxes = false;  // требуется ли отрисовка длинных координатных линий
    

    
public:
    explicit PaintWidget(QWidget *parent = nullptr);

private:
    void updatePoints();    // перерасчет массива с точками

signals:
// обработчики кнопок
public slots:
    // обработчики кнопок масштаба
    void scaleDownPressed();
    void scaleUpPressed();

    // обработчики кнопок смещения
    void oxOffsetDownPressed();
    void oxOffsetUpPressed();
    void oyOffsetDownPressed();
    void oyOffsetUpPressed();

    // обработчик ползунка с углом обзора
    void viewAngleChanged(int newAngle);

    // обработчик кнопки "Сброс"
    void resetPressed();

    // обработчик кнопки "Обновить"
    void updatePressed(double speed, double ang, int time);

    // обработчик чек бокса с выбором типа осей
    void longAxesChanged(int value);
    
protected:
    // переопределенный метод отрисовки окна
    void paintEvent(QPaintEvent *event);
};

#endif // PAINTWIDGET_H
