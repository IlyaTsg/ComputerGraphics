#ifndef WIDGET_H
#define WIDGET_H

#include <QtWidgets>

class Widget : public QWidget
{
    Q_OBJECT

private:
    QTextEdit *inx1, *iny1, *inx2, *iny2, *incx, *incy, *inradius;
    QPushButton *mirrorButton, *clearButton, *drawButton;
    QGraphicsScene *scene;
    int x1, y1, x2, y2, cx, cy, radius;

    void prepareTextEdits();

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void draw();
    void clearScene();
    void mirror();
};
#endif // WIDGET_H
