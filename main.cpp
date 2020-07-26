#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icon.png"));   // установки иконки приложения из ресурсов

    // установка цвета фона приложения на белый (по умолчанию - серый, не самый приятный)
    QPalette pal = a.palette();
    pal.setColor(QPalette::Background,Qt::white);
    a.setPalette(pal);

    MainWindow w;
    w.show();

    return a.exec();
}
