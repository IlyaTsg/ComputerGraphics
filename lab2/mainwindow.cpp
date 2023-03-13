#include "mainwindow.h"
#include "math.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent),
    inx(new QTextEdit),
    iny(new QTextEdit),
    pointNumText(new QTextEdit()),
    enterXText(new QTextEdit()),
    enterYText(new QTextEdit()),
    changeWidget(new QWidget),
    drawPointButton(new QPushButton("Добавить точку")),
    clearButton(new QPushButton("Очистить")),
    drawBezierButton(new QPushButton("Нарисовать кривую Безье")),
    changePointButton(new QPushButton("Изменить точку")),
    scene(new QGraphicsScene)
{
    resize(800, 600);
    prepareTextEdits();

    QHBoxLayout *upLayout = new QHBoxLayout;
    QVBoxLayout *XLayout = new QVBoxLayout;
    QVBoxLayout *YLayout = new QVBoxLayout;
    QVBoxLayout *buttonLayout = new QVBoxLayout;
    QHBoxLayout *sceneLayout = new QHBoxLayout;
    QVBoxLayout *mainLayout = new QVBoxLayout;

    XLayout->addWidget(new QLabel("Введите X:"));
    XLayout->addWidget(inx);

    YLayout->addWidget(new QLabel("Введите Y:"));
    YLayout->addWidget(iny);

    upLayout->addLayout(XLayout);
    upLayout->addLayout(YLayout);
    buttonLayout->addWidget(drawPointButton);
    buttonLayout->addWidget(drawBezierButton);
    buttonLayout->addWidget(clearButton);
    buttonLayout->addWidget(changePointButton);
    buttonLayout->setSpacing(2);
    upLayout->addLayout(buttonLayout);

    QGraphicsView *view = new QGraphicsView(scene);
    sceneLayout->addWidget(view);

    mainLayout->addLayout(upLayout, 1);
    mainLayout->addLayout(sceneLayout, 9);
    setLayout(mainLayout);

    QObject::connect(drawPointButton, SIGNAL(clicked()), SLOT(drawPoint()));
    QObject::connect(clearButton, SIGNAL(clicked()), SLOT(clearScene()));
    QObject::connect(drawBezierButton, SIGNAL(clicked()), SLOT(drawBezier()));
    QObject::connect(changePointButton, SIGNAL(clicked()), SLOT(changePoint()));
}

void Widget::prepareTextEdits()
{
    int height = 75;
    inx->setFixedHeight(height);
    iny->setFixedHeight(height);
}


Widget::~Widget()
{
}

void Widget::clearScene()
{
    scene->clear();
    inx->clear();
    iny->clear();
    points->clear();
}

void Widget::drawPoint()
{
    QPoint *new_point = new QPoint(inx->toPlainText().toInt(), iny->toPlainText().toInt());

    // Рисуем точку
    scene->addEllipse(new_point->x()-1, -new_point->y()-1, 2.0, 2.0);

    if(!points->isEmpty()){
        scene->addLine(points->last()->x(), -points->last()->y(), new_point->x(), -new_point->y(), QPen(Qt::black, 1, Qt::PenStyle::DashLine));
    }

    // Добавляем точку в вектор
    points->append(new_point);
    // Ставим номер около точки
    QGraphicsTextItem *text = scene->addText(QVariant(points->size()).toString());
    text->setPos(new_point->x(), -new_point->y());

    // Очистка полей ввода
    inx->clear();
    iny->clear();
}

void Widget::drawBezier()
{
    scene->clear();
    for(int i=0; i<points->size()-1; i++){
        // Рисуем точку
        scene->addEllipse(points->at(i)->x()-1, -points->at(i)->y()-1, 2.0, 2.0);

        // Ставим номер около точки
        QGraphicsTextItem *text = scene->addText(QVariant(i+1).toString());
        text->setPos(points->at(i)->x(), -points->at(i)->y());

        scene->addLine(points->at(i+1)->x(), -points->at(i+1)->y(), points->at(i)->x(), -points->at(i)->y(), QPen(Qt::black, 1, Qt::PenStyle::DashLine));
    }
    scene->addEllipse(points->last()->x()-1, -points->last()->y()-1, 2.0, 2.0);
    QGraphicsTextItem *text = scene->addText(QVariant(points->size()).toString());
    text->setPos(points->last()->x(), -points->last()->y());

    // QPoint point;
    QPoint center, prev_center;
    QLine *line;
    QVector<QPoint*> new_vec;

    for(int i=0; i<points->size();){
        if(prev_center.isNull()){
            // Если точек больше 3
            if(i+3<points->size()){
                // Если 4я точка не последняя в векторе
                if(points->at(i+3) != points->last()){
                    // Находим координату доп. точки
                    line = new QLine(points->at(i+3)->x(), points->at(i+3)->y(),points->at(i+2)->x(), points->at(i+2)->y());
                    center = line->center();
                }
                else{
                    center = *(points->last());
                }
                prev_center = center;

                new_vec.append(points->at(i));
                new_vec.append(points->at(i+1));
                new_vec.append(points->at(i+2));
                new_vec.append(&center);
                drawFragment(new_vec);

                if(points->at(i+3) == points->last()){
                    break;
                }
                i += 3;
            }
            // Если введено 3 точки
            else if(points->size() == 3){
                new_vec.append(points->at(i));
                new_vec.append(points->at(i+1));
                new_vec.append(points->at(i+2));
                drawFragment(new_vec);
                break;
            }
        }
        else{
            // Если точек начиная с последней более 2
            if(i+2<points->size()){
                // Если 3 точка не последняя в векторе
                if(points->at(i+2) != points->last()){
                    // Находим координату доп. точки
                    line = new QLine(points->at(i+2)->x(), points->at(i+2)->y(),points->at(i+1)->x(), points->at(i+1)->y());
                    center = line->center();
                }
                else{
                    center = *(points->last());
                }

                new_vec.append(&prev_center);
                new_vec.append(points->at(i));
                new_vec.append(points->at(i+1));
                new_vec.append(&center);
                drawFragment(new_vec);
                prev_center = center;

                // Если 3я точка была последняя
                if(points->at(i+2) == points->last()){
                    break;
                }
                i += 2;
            }
            else if(i+2>=points->size()){
                new_vec.append(&prev_center);
                new_vec.append(points->at(i));
                new_vec.append(points->at(i+1));
                drawFragment(new_vec);
                break;
            }
        }
    }
}

void Widget::drawFragment(QVector<QPoint*>& new_points)
{
    QPoint point;
    if(new_points.size() == 4){
        // Вычисляем кривую Безье
        for(double t=0.0; t<=1.0; t+=0.001){
            point = (*new_points.at(0))*(1.0-t)*(1.0-t)*(1.0-t) + 3.0*(1.0-t)*(1.0-t)*t*(*new_points.at(1)) + 3*(1.0-t)*t*t*(*new_points.at(2)) + t*t*t*(*new_points.at(3));

            // Рисуем точку
            scene->addEllipse(point.x()-1, -point.y()-1, 2.0, 2.0, QPen(Qt::red, 0.5));
        }
    }
    else if(new_points.size() == 3){
        for(double t=0.0; t<=1.0; t+=0.001){
            point = (*new_points.at(0))*(1.0-t)*(1.0-t) + 2.0*(1.0-t)*t*(*new_points.at(1)) + t*t*(*new_points.at(2));

            // Рисуем точку
            scene->addEllipse(point.x()-1, -point.y()-1, 2.0, 2.0, QPen(Qt::red, 0.5));
        }
    }

    new_points.clear();
}

void Widget::changePoint()
{
    QLabel *pointNumLabel(new QLabel("Введите номер точки:")), *enterXLabel(new QLabel("Введите X:")), *enterYLabel(new QLabel("Введите Y:"));
    QPushButton *enterDataButton(new QPushButton("Ввести данные"));

    QVBoxLayout *mainLayout = new QVBoxLayout();
    QHBoxLayout *box1(new QHBoxLayout()), *box2(new QHBoxLayout()), *box3(new QHBoxLayout());

    box1->addWidget(pointNumLabel);
    box1->addWidget(pointNumText);

    box2->addWidget(enterXLabel);
    box2->addWidget(enterXText);

    box3->addWidget(enterYLabel);
    box3->addWidget(enterYText);

    mainLayout->addLayout(box1, 1);
    mainLayout->addLayout(box2, 1);
    mainLayout->addLayout(box3, 1);
    mainLayout->addWidget(enterDataButton);

    changeWidget->setLayout(mainLayout);
    changeWidget->setFixedSize(400, 200);

    QObject::connect(enterDataButton, SIGNAL(clicked()), SLOT(getPoint()));

    changeWidget->show();
}

void Widget::getPoint()
{
    int index_of_point = pointNumText->toPlainText().toInt();
    int new_X = enterXText->toPlainText().toInt();
    int new_Y = enterYText->toPlainText().toInt();

    // Изменяем координаты
    if(index_of_point-1>=0 || index_of_point<points->size()){
        points->at(index_of_point-1)->setX(new_X);
        points->at(index_of_point-1)->setY(new_Y);
    }

    changeWidget->close();
    drawBezier();
}

