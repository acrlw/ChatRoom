#include "widget.h"
#include "ui_widget.h"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QDateTime time = QDateTime::currentDateTime();                      //获取系统现在的时间
    QString current_date = time.toString("yyyy-MM-dd hh:mm:ss");        //设置显示格式
    ui->txtShow->setText("\n" +ui->txtShow->toPlainText()+ "                  " + current_date + " 欢迎来到扯淡聊天室~ 想聊啥就聊啥,口下积德哦" + "\n\n");
    socket = new QTcpSocket(this);
    timer = new QTimer(this);
    waitClose = new QTimer(this);
    socket->connectToHost("accelerated.6655.la",27787);         //这个ip和端口是我的花生壳端口映射结果，但已经失效了，在这里只是做一个例子
    connect(timer,SIGNAL(timeout()),this,SLOT(GagTime()));
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(socketError()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    connect(waitClose,SIGNAL(timeout()),this,SLOT(CloseChat()));
    QString str = "vv"+ QString::number(ver.length()) +":" + ver;
    socket->write(str.toUtf8());
}
void Widget::onReadyRead()
{
    QString cmd = socket->readAll();                                        //截获命令
    while(!cmd.isEmpty())                                                   //解决粘包问题
    {
        QString msgContext = cmd.mid(cmd.indexOf(":") + 1,cmd.mid(2,cmd.indexOf(":") - 2).toInt());
        QDateTime time = QDateTime::currentDateTime();                      //获取系统现在的时间
        QString current_date = time.toString("yyyy-MM-dd hh:mm:ss");        //设置显示格式
        if(QString::compare(cmd.left(2),"rg") == 0)                         //判断各种命令
        {
            QString name = msgContext.mid(0,msgContext.indexOf(","));
            QString ct = msgContext.mid(msgContext.indexOf(",") + 1 , msgContext.length());
            ui->txtShow->setText(ui->txtShow->toPlainText()+ name + "   " + current_date + "\n" + ct + "\n\n" );
        }
        if(QString::compare(cmd.left(2),"sn") == 0)                         //判断各种命令
            ui->txtShow->setText(ui->txtShow->toPlainText()+"\n"+ "                  " + current_date + " 有新的扯淡君加入！" + "\n\n");
        if(QString::compare(cmd.left(2),"sr") == 0)                         //判断各种命令
            ui->txtShow->setText(ui->txtShow->toPlainText()+"\n"+ "                  " + current_date + " 有位扯淡君阵亡了！" + "\n\n");
        if(QString::compare(cmd.left(2),"ss") == 0)                         //判断各种命令
            ui->txtShow->setText(ui->txtShow->toPlainText()+"\n"+ "                  " + current_date + "服务器说："+ msgContext + "\n\n");
        if(QString::compare(cmd.left(2),"gg") == 0)                         //判断各种命令
            ui->txtShow->setText(ui->txtShow->toPlainText()+"\n"+ "                  " + current_date + "有人被禁言了~");
        if(QString::compare(cmd.left(2),"up") == 0)                         //判断各种命令
        {
            ui->txtShow->setText(ui->txtShow->toPlainText()+"\n"+ "                  " + current_date + "服务器君说了,我们的修为不够,需要升级!3秒后我们将关闭客户端君\n\n");
            waitClose->start(3000);
        }
        if(QString::compare(cmd.left(2),"cl") == 0)                         //判断各种命令
        {
            this->close();
        }
        if(QString::compare(cmd.left(2),"fv") == 0)                         //判断各种命令
        {
            ui->txtShow->setText(ui->txtShow->toPlainText()+"\n"+ "                  " + current_date + "有新版本哦!请尽快升级扯淡君~\n\n");
            socket->abort();
            socket->close();
            socket->deleteLater();
            ui->pushButton->setEnabled(false);
        }
        if(QString::compare(cmd.left(2),"sv") == 0)                         //判断各种命令
            ui->txtShow->setText(ui->txtShow->toPlainText()+"\n"+ "                  " + current_date + "您的扯淡君是最新版本~\n\n");
        if(QString::compare(cmd.left(2),"ga") == 0)                         //禁言！
        {
            ui->txtShow->setText(ui->txtShow->toPlainText()+"\n"+ "                  " + current_date + "您被服务器君禁言了哦~\n\n");
            ui->pushButton->setEnabled(false);
            timer->start(msgContext.toInt());

        }
        int cmdLen = 3 + cmd.mid(2,cmd.indexOf(":") - 2).length() + msgContext.length();
        cmd = cmd.remove(0,cmdLen);
    }
}
void Widget::socketError()
{
    qDebug() << socket->errorString();
    QDateTime time = QDateTime::currentDateTime();                      //获取系统现在的时间
    QString current_date = time.toString("yyyy-MM-dd hh:mm:ss");        //设置显示格式
    if(socket->error() == QAbstractSocket::RemoteHostClosedError)
    {
        ui->txtShow->setText(ui->txtShow->toPlainText()+"\n"+ "                  " + current_date + " 好像我们的服务器君开小差了。。" + "\n\n");
        ui->pushButton->setEnabled(false);
    }
}
void Widget::CloseChat()
{
    this->close();
}

void Widget::GagTime()
{
    ui->pushButton->setEnabled(true);
}

void Widget::sendMesage()
{
    QDateTime time = QDateTime::currentDateTime();                      //获取系统现在的时间
    QString current_date = time.toString("yyyy-MM-dd hh:mm:ss");        //设置显示格式
    if(ui->txtName->text().length() < 10 && ui->txtContext->toPlainText().length() < 1000 && !ui->txtContext->toPlainText().isEmpty() && !ui->txtName->text().isEmpty())
    {
        QString cmd = "sg"+ QString::number(ui->txtContext->toPlainText().length() + ui->txtName->text().length() + 1) + ":" + ui->txtName->text() + "," + ui->txtContext->toPlainText();
        socket->write(cmd.toUtf8());
        ui->txtShow->setText(ui->txtShow->toPlainText()+ ui->txtName->text() + "   " + current_date + "\n" + ui->txtContext->toPlainText() + "\n\n");
    }
    else
    {
        ui->txtShow->setText(ui->txtShow->toPlainText()+"\n"+ "                  " + current_date + " 您的输入不合法哦！名称小于10位且不能为空,发送内容不能为空也不能大于1k~\n\n");
    }
            ui->txtContext->clear();
}

Widget::~Widget()
{

    delete ui;
}

void Widget::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Return && (e->modifiers() & Qt::ControlModifier))
        sendMesage();
}

void Widget::on_txtShow_textChanged()
{
    ui->txtShow->moveCursor(QTextCursor::End);
}
