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
    qDebug() << phi;

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
    qDebug() << phi;

    // Находим матрицу поворота
    double rotate_matrix[2][2]= {{cos(phi), sin(phi)}, {-sin(phi), cos(phi)}};
    // Находим матрицу обратного поворота
    double reverse_rotate_matrix[2][2] = {{cos(-phi), sin(-phi)}, {-sin(-phi), cos(-phi)}};

    double center[2] = {(double)cx, (double)cy}; // Координаты центра

    // Поворот центра умножением на матрицу поворота
    double rotate_center[2] = {center[0]*rotate_matrix[0][0]+center[1]*rotate_matrix[1][0],center[0]*rotate_matrix[0][1]+center[1]*rotate_matrix[1][1]};
    double xr = rotate_center[0] - radius;
    double yr = -rotate_center[1] - radius;
    double height = 2*radius;
    scene->addEllipse(xr, yr, height, height, QPen(Qt::red, 1, Qt::PenStyle::DashLine));

    // Отображение центра умножением на матрицу отображения относительно оси X
    double reverse_center[2] = {rotate_center[0], -rotate_center[1]};
    xr = reverse_center[0] - radius;
    yr = -reverse_center[1] - radius;
    height = 2*radius;
    scene->addEllipse(xr, yr, height, height, QPen(Qt::green, 1, Qt::PenStyle::DashLine));

    // Обратный поворот множением на матрицу обратного поворота
    double reverse_rotate_center[2] = {reverse_center[0]*reverse_rotate_matrix[0][0]+reverse_center[1]*reverse_rotate_matrix[1][0], reverse_center[0]*reverse_rotate_matrix[0][1]+reverse_center[1]*reverse_rotate_matrix[1][1]};
    qDebug() << "X:" << reverse_rotate_center[0] << "Y:" << reverse_rotate_center[1];
    xr = reverse_rotate_center[0] - radius;
    yr = -reverse_rotate_center[1] - radius;
    height = 2*radius;
    scene->addEllipse(xr, yr, height, height, QPen(Qt::blue, 1, Qt::PenStyle::DashLine));
    scene->addLine(-100,100,100,-100, QPen(Qt::red));
}


