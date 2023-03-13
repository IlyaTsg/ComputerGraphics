#ifndef WIDGET_H
#define WIDGET_H

#include <QtWidgets>

class Widget : public QWidget
{
    Q_OBJECT

private:
    QTextEdit *inx, *iny;
    QPushButton *drawPointButton, *clearButton, *drawBezierButton, *changePointButton;
    QGraphicsScene *scene;
    int x, y;
    QVector<QPoint*> *points = new QVector<QPoint*>; // Вектор точек
    QTextEdit *pointNumText, *enterXText, *enterYText;

    QWidget *changeWidget;

    void prepareTextEdits();

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void drawFragment(QVector<QPoint*>& new_points);

public slots:
    void drawPoint();
    void clearScene();
    void drawBezier();
    void changePoint();
    void getPoint();
};
#endif // WIDGET_H
