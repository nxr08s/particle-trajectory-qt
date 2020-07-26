#include "paintwidget.h"

PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent)
{ 
    resetPressed(); // сбрасываем координаты
    updatePoints(); // расчитываем первичный массив точек (чтобы небыло пустого экрана вначале)
}

void PaintWidget::scaleDownPressed()
{
    userScale /= 1.5;
    update();
}

void PaintWidget::scaleUpPressed()
{
    userScale *= 1.5;
    update();
}

void PaintWidget::oxOffsetDownPressed()
{
    userXOffset -= 25;
    update();
}

void PaintWidget::oxOffsetUpPressed()
{
    userXOffset += 25;
    update();
}

void PaintWidget::oyOffsetDownPressed()
{
    userYOffset += 25;
    update();
}

void PaintWidget::oyOffsetUpPressed()
{
    userYOffset -= 25;
    update();
}

void PaintWidget::viewAngleChanged(int newAngle)
{
    viewAngle = newAngle;
    update();
}

void PaintWidget::resetPressed()
{
    userXOffset = 200;
    userYOffset = 0;
    userScale = 1.5;
    viewAngle = 60;
}

void PaintWidget::updatePressed(double speed, double ang, int time)
{
    v0 = speed;
    angle = ang;
    simTime = time;

    updatePoints();

    update();
}

void PaintWidget::longAxesChanged(int value)
{
    longAxes = bool(value);
    update();
}

void PaintWidget::updatePoints()
{
    // удаление всех предыдущих точек
    points.clear();

    // расчет скорости по оси X
    xSpeed = v0 * qCos(qDegreesToRadians(angle));

    // расчет радиуса окружности, по которому движется частица
    r = qAbs(m / q) * (v0 * qSin(qDegreesToRadians(angle)) / B);
    // расчет расстояния между витками спирали
    h = (2 * M_PI * m) / (q * B) * (v0 * qCos(qDegreesToRadians(angle)));

    // расчет массива точек для построение (без масштабирования)
    for (int t = -(simTime / 2); t < (simTime / 2); t++){
        points.append(QVector3D(xSpeed * t,                                     // координата X
                                r * qSin(qDegreesToRadians(qreal(t % 360))),    // координата Y
                                r * qCos(qDegreesToRadians(qreal(t % 360))))    // координата Z (прямо не используется)
        );
    }
}

void PaintWidget::paintEvent(QPaintEvent *event)
{
    const int yOffset = rect().center().y();    // смещение по Y в центр сцены


    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // включаем сглаживание


    // ---- Отрисовка осей ----
    // ось Y
    painter.drawLine(10, 10,
                     10, height() - 10);
    // ось X
    painter.drawLine(10, height() - 10,
                     width() - 10,height() - 10);


    // ---- Отрисовка значений по оси Х ----
    painter.drawText(xSpeed * 360 * xScale * userScale * qSin(qDegreesToRadians(viewAngle)) + userXOffset,
                     height() - 15,
                     QString::number(h) + QString(" m"));
    painter.drawText(-(xSpeed * 360 * xScale * userScale * qSin(qDegreesToRadians(viewAngle))) + userXOffset,
                     height() - 15,
                     QString::number(-h) + QString(" m"));
    painter.drawText(userXOffset,
                     height() - 15,
                     QString::number(0) + QString(" m"));

    // ---- Отрисовка линий по оси X ----
    painter.setPen((longAxes)?(Qt::lightGray):(Qt::black));
    painter.drawLine(xSpeed * 360 * xScale * userScale * qSin(qDegreesToRadians(viewAngle)) + userXOffset,
                     (longAxes)?(10):(height() - 13),
                     xSpeed * 360 * xScale * userScale * qSin(qDegreesToRadians(viewAngle)) + userXOffset,
                     height() - 7);
    painter.drawLine(-(xSpeed * 360 * xScale * userScale * qSin(qDegreesToRadians(viewAngle))) + userXOffset,
                     (longAxes)?(10):(height() - 13),
                     -(xSpeed * 360 * xScale * userScale * qSin(qDegreesToRadians(viewAngle))) + userXOffset,
                     height() - 7);
    painter.drawLine(userXOffset,(longAxes)?(10):(height() - 13),
                     userXOffset,height() - 7);
    painter.setPen(Qt::black);


    // ---- Отрисовка значений по оси Y ----
    painter.drawText(15,yOffset + userYOffset,
                     QString::number(0) + QString(" m"));
    painter.drawText(15,yOffset + userYOffset + r * yzScale * userScale,
                     QString::number(-r) + QString(" m"));
    painter.drawText(15,yOffset + userYOffset - r * yzScale * userScale,
                     QString::number(r) + QString(" m"));


    // ---- Отрисовка линий по оси X ----
    painter.setPen((longAxes)?(Qt::lightGray):(Qt::black));
    painter.drawLine(7,yOffset + userYOffset,
                     (longAxes)?(width() - 10):(13),yOffset + userYOffset);


    painter.drawLine(7,yOffset + userYOffset + r * yzScale * userScale,
                     (longAxes)?(width() - 10):(13),yOffset + userYOffset + r * yzScale * userScale);


    painter.drawLine(7,yOffset + userYOffset - r * yzScale * userScale,
                     (longAxes)?(width() - 10):(13),yOffset + userYOffset - r * yzScale * userScale);
    painter.setPen(Qt::black);

    // ---- Конец отрисовки осей ----


    // отрисовка самой траектории
    for (int i = 1; i < points.size(); i++){
        painter.drawLine(
                    // начало отрезка с учетом всех преобразований и псевдо 3д
                    (points[i - 1].x() * qSin(qDegreesToRadians(viewAngle)) * xScale +          // значение x c учетом угла обзора
                    points[i - 1].z() * yzScale * qCos(qDegreesToRadians(qreal(viewAngle)))) *  // прибавляем значение по оси Z, проецированное на нас
                    userScale +     // умножаем на масштаб (изменяемый в параметрах сцены)
                    userXOffset,    // и двигаем сцену на смещение (изменяется в параметрах сцены)

                    points[i - 1].y() * yzScale * userScale +   // значение с учетом подобранного масштаба и пользовательского масштаба
                    yOffset + userYOffset,  // прибавляяем смещение в центр сцены и пользовательское смещение (меняется в параметрах сцены)

                    // конец отрезка (аналогично началу)
                    (points[i].x() * qSin(qDegreesToRadians(viewAngle)) * xScale +
                    points[i].z() * yzScale * qCos(qDegreesToRadians(qreal(viewAngle)))) * userScale +
                    userXOffset,

                    points[i].y() * yzScale * userScale +
                    yOffset + userYOffset
        );
    }

    // даем порисовать стандартному отрисовщику
    QWidget::paintEvent(event);
}
