#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork>
#include <QEvent>
#include <QKeyEvent>
#include <QTextCursor>
#include <QTimer>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
protected:
    void keyPressEvent(QKeyEvent *e);
public slots:
    void onReadyRead();
    void sendMesage();
    void socketError();
    void GagTime();
    void CloseChat();
private slots:
    void on_txtShow_textChanged();

private:
    Ui::Widget *ui;
    QTcpSocket * socket;
    QString ver = "1.0";
    QTimer * timer;
    QTimer * waitClose;
};

#endif // WIDGET_H
