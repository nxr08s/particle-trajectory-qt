#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "infowindow.h"
#include "paintwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    PaintWidget *paintWgt;  // само окно рисования

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void newData(double speed,double angle,int time);   // сигнал что нажата кнопка обновить (через него передаются данные
                                                        // окну рисования)
    void reset();   // сигнал что нажата кнопка "Сброс"(передается окну рисования)

private slots:
    void updatePressed();   // обработчкие нажатия кнопки "Обновить"
    void resetPressed();    // обработчки нажатия кнопки "Сброс"
    void infoPressed();     // обработчик нажатия на меню "Помощь"

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
