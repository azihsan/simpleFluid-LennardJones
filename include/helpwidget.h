#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include <QWidget>

namespace Ui {
class helpWidget;
}

class helpWidget : public QWidget
{
    Q_OBJECT

public:
    explicit helpWidget(QWidget *parent = 0);
    ~helpWidget();

private:
    Ui::helpWidget *ui;
};

#endif // HELPWIDGET_H
