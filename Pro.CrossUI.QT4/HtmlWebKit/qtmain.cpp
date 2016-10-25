#include <QtGui/QApplication>
#include <QtWebKit/QWebView>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QWebView *view = new QWebView;
    view->load(QUrl("http://www.baidu.com"));
    view->show();
    return app.exec();
}