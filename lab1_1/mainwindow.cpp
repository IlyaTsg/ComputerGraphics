#include "mainwindow.h"
#include "math.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent),
    inx1(new QTextEdit),
    iny1(new QTextEdit),
    inx2(new QTextEdit),
    iny2(new QTextEdit),
    incx(new QTextEdit),
    incy(new QTextEdit),
    inradius(new QTextEdit),
    mirrorButton(new QPushButton("Отобразить")),
    clearButton(new QPushButton("Очистить")),
    drawButton(new QPushButton("Нарисовать")),
    scene(new QGraphicsScene)
{
    resize(800, 600);
    prepareTextEdits();

    QHBoxLayout *upLayout = new QHBoxLayout;
    QVBoxLayout *buttonLayout = new QVBoxLayout;
    QHBoxLayout *sceneLayout = new QHBoxLayout;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    upLayout->addWidget(inx1);
    upLayout->addWidget(iny1);
    upLayout->addWidget(inx2);
    upLayout->addWidget(iny2);
    upLayout->addWidget(incx);
    upLayout->addWidget(incy);
    upLayout->addWidget(inradius);
    buttonLayout->addWidget(mirrorButton);
    buttonLayout->addWidget(drawButton);
    buttonLayout->addWidget(clearButton);
    buttonLayout->setSpacing(2);
    upLayout->addLayout(buttonLayout);


    QGraphicsView *view = new QGraphicsView(scene);
    sceneLayout->addWidget(view);
    scene->addLine(300,0,-300,0, QPen(Qt::gray, 1, Qt::PenStyle::DashLine));
    scene->addLine(0,200,0,-200, QPen(Qt::gray, 1, Qt::PenStyle::DashLine));

    mainLayout->addLayout(upLayout, 1);
    mainLayout->addLayout(sceneLayout, 9);
    setLayout(mainLayout);

    QObject::connect(drawButton, SIGNAL(clicked()), SLOT(draw()));
    QObject::connect(clearButton, SIGNAL(clicked()), SLOT(clearScene()));
    QObject::connect(mirrorButton, SIGNAL(clicked()), SLOT(mirror()));
}

void Widget::prepareTextEdits()
{
    int height = 75;
    inx1->setPlaceholderText("Введите x1: ");
    inx1->setFixedHeight(height);
    iny1->setPlaceholderText("Введите y1: ");
    iny1->setFixedHeight(height);
    inx2->setPlaceholderText("Введите x2: ");
    inx2->setFixedHeight(height);
    iny2->setPlaceholderText("Введите y2: ");
    iny2->setFixedHeight(height);
    incx->setPlaceholderText("Введите x центра: ");
    incx->setFixedHeight(height);
    incy->setPlaceholderText("Введите y центра: ");
    incy->setFixedHeight(height);
    inradius->setPlaceholderText("Введите радиус: ");
    inradius->setFixedHeight(height);
}


Widget::~Widget()
{
}

void Widget::draw()
{
    scene->clear();
    x1 = inx1->toPlainText().toInt();
    y1 = iny1->toPlainText().toInt();
    x2 = inx2->toPlainText().toInt();
    y2 = iny2->toPlainText().toInt();
    cx = incx->toPlainText().toInt();
    cy = incy->toPlainText().toInt();
    radius = inradius->toPlainText().toInt();
    int dx = x2 - x1;
    int dy = -y2 + y1;
    qDebug() << x1 << y1 << x2 << y2;

    int k = 20;
    scene->addLine(300,0,-300,0, QPen(Qt::gray, 1, Qt::PenStyle::DashLine));
    scene->addLine(0,200,0,-200, QPen(Qt::gray, 1, Qt::PenStyle::DashLine));
    scene->addLine(x1, -y1, x2, -y2, QPen(Qt::red));
    scene->addLine(x2, -y2, x2 + dx*k, -y2 + dy*k, QPen(Qt::blue));
    scene->addLine(x1, -y1, x1 - dx*k, -y1 - dy*k, QPen(Qt::blue));

    int xr = cx - radius;
    int yr = -cy - radius;
    int height = 2*radius;

    scene->addEllipse(xr, yr, height, height);
}

void Widget::clearScene()
{
    scene->clear();
    scene->addLine(300,0,-300,0, QPen(Qt::gray, 1, Qt::PenStyle::DashLine));
    scene->addLine(0,200,0,-200, QPen(Qt::gray, 1, Qt::PenStyle::DashLine));
    inx1->clear();
    iny1->clear();
    inx2->clear();
    iny2->clear();
    incx->clear();
    incy->clear();
    inradius->clear();
}

void Widget::mirror()
{
    // Находим угол между прямой и осью X
    double phi = atan((double)(abs(y1-y2))/(double)abs(x2-x1));

    if(x1<x2){ // Первая точка левее
        if(y1<y2){ // Возрастает
            phi = M_PI-phi; // Находим смежный угол
        }
    }
    else{ // Вторая точка левее
        if(y1>y2){ // Возрастает
            phi = M_PI-phi; // Находим смежный угол
        }
    }

    // Матрица координат центра
    double center[3] = {(double)cx, (double)cy, 1};
    // Матрица перемещения начала координат
    double moving_coord[3][3] = {{1, 0, 0}, {0, 1, 0}, {(double)-x1, (double)-y1, 1}};
    // Матрица поворота
    double rotate_matrix[3][3]= {{cos(phi), sin(phi), 0}, {-sin(phi), cos(phi), 0}, {0, 0, 1}};
    // Матрица обратного перемещения начала координат
    double reverse_moving_coord[3][3] = {{1, 0, 0}, {0, 1, 0}, {(double)x1, (double)y1, 1}};
    // Матрица обратного поворота
    double reverse_rotate_matrix[3][3]= {{cos(phi), -sin(phi), 0}, {sin(phi), cos(phi), 0}, {0, 0, 1}};

    // Новые координты центра с учетом перемещения начала координат
    double moved_center[3] = {center[0]-x1, center[1]-y1, 1};

    // Поворот центра относительно нового начала координат
    double rotated_center[3] = {moved_center[0]*cos(phi)-moved_center[1]*sin(phi), moved_center[0]*sin(phi)+moved_center[1]*cos(phi), 1};

    // Отображаем относительно оси X
    double reverse_center[3] = {rotated_center[0], -rotated_center[1], 1};

    // Обратный поворот
    double reverse_rotated_center[3] = {reverse_center[0]*cos(phi)+reverse_center[1]*sin(phi), -reverse_center[0]*sin(phi)+reverse_center[1]*cos(phi), 1};

    // Обратное перемещение начала координат
    double reverse_moved_center[3] = {reverse_rotated_center[0]+x1, reverse_rotated_center[1]+y1, 1};

    double xr = reverse_moved_center[0] - radius;
    double yr = -reverse_moved_center[1] - radius;
    double height = 2*radius;
    scene->addEllipse(xr, yr, height, height, QPen(Qt::blue, 1, Qt::PenStyle::DashLine));
}
