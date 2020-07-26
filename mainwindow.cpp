#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // создание окна рисования
    paintWgt = new PaintWidget(this);

    // добавление его на форму
    ui->painterLayout->addWidget(paintWgt);

    // установка валидаторов для полей времени и угла (для скорости проверка осущ. вручную)
    ui->timeEdit->setValidator(new QIntValidator(0,10000,this));
    ui->angleEdit->setValidator(new QIntValidator(0,90,this));


    // соединение кнопок смещения с их обработчиками (которые в окне отрисовки)
    connect(ui->oxOffsetDown,SIGNAL(clicked(bool)),
            paintWgt,SLOT(oxOffsetDownPressed()));
    connect(ui->oxOffsetUp,SIGNAL(clicked(bool)),
            paintWgt,SLOT(oxOffsetUpPressed()));
    connect(ui->oyOffsetDown,SIGNAL(clicked(bool)),
            paintWgt,SLOT(oyOffsetDownPressed()));
    connect(ui->oyOffsetUp,SIGNAL(clicked(bool)),
            paintWgt,SLOT(oyOffsetUpPressed()));

    // соединение кнопок масштаба с их обработчиками (которые в окне отрисовки)
    connect(ui->scaleDown,SIGNAL(clicked(bool)),
            paintWgt,SLOT(scaleDownPressed()));
    connect(ui->scaleUp,SIGNAL(clicked(bool)),
            paintWgt,SLOT(scaleUpPressed()));

    // соединение кнопок "Сброс" и "Обновить" с обработчиками (которые в этом окне)
    connect(ui->resetButton,SIGNAL(clicked(bool)),
            this,SLOT(resetPressed()));
    connect(ui->updateButton,SIGNAL(clicked(bool)),
            this,SLOT(updatePressed()));

    // пересылка нажатий кнопок "Сброс" и "Обновить" окну отрисовки
    connect(this,SIGNAL(reset()),
            paintWgt,SLOT(resetPressed()));
    connect(this,SIGNAL(newData(double,double,int)),
            paintWgt,SLOT(updatePressed(double,double,int)));

    // соединение слайдера с обработчиком изменения значения (в окне отрисовки)
    connect(ui->viewAngleSlider,SIGNAL(valueChanged(int)),
            paintWgt,SLOT(viewAngleChanged(int)));

    // соединение чекбокса координатной сетки с обработчиком (в окне отрисовки)
    connect(ui->longAxesCheck,SIGNAL(stateChanged(int)),
            paintWgt,SLOT(longAxesChanged(int)));

    // создание пункта меню и соединение с обработчиком нажатия на него
    QAction *act;
    act = ui->menuBar->addAction("&Помощь");
    connect(act,SIGNAL(triggered(bool)),
            this,SLOT(infoPressed()));
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updatePressed()
{
    // при нажатии на кнопку "Обновить", сначала выполняется этот слот, затем если данные корректны, они передаются
    // окну отрисовки через сигнал newData(...), которое перерасчитывает массив точек и перерисовывает окно
    bool convFlag;  // флаг "успешности" преобразования str to double.
    double convSpeed = ui->startSpeedEdit->text().toDouble(&convFlag);  // преобразование
    if (!convFlag){ // если не удачно
        QMessageBox::critical(this,"Ошибка","Введена неверная начальная скорость");
        return;
    }
    // если преобразование прошло успешно, то кидаем новые данные окну отрисовки
    emit newData(convSpeed,ui->angleEdit->text().toDouble(),ui->timeEdit->text().toInt());
}

void MainWindow::resetPressed()
{
    // можно было назначить обработчк кнопки "Сброс" сразу окну отрисовки, но нужно еще сбросить слайдер
    // "Угол обзора", так что сначало выполняется этот обработчик, затем в окне отрисовке выполняется сброс остальных
    // параметров сцены
    ui->viewAngleSlider->setValue(60);
    emit reset();
}

void MainWindow::infoPressed()
{
    // Создание окошка "Помощь"
    infoWindow *infoWnd = new infoWindow(this);
    infoWnd->setAttribute(Qt::WA_DeleteOnClose);    // окно далжно само удалить все свои ресурсы после закрытия
    infoWnd->setModal(true);    // делаем окно модальным (блокирует все остальные окна пока не закроется)

    infoWnd->show();    // отображение окна
}
