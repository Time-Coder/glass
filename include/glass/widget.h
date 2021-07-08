#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class Scene3D;

class Widget: public QWidget
{
    Q_OBJECT

    Scene3D* scene = nullptr;

public:
    Widget(QWidget* parent = nullptr);
    ~Widget();
};

#endif // WIDGET_H
