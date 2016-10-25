#include "ui_widget.h"

#include <QtGui/QtGui>
#include <QtMultimedia/QAudioInput>
#include <QtMultimedia/QAudioOutput>

using namespace std;

class Widget : public QWidget {
    Q_OBJECT
};

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	Widget widget;
        Ui::Form().setupUi(&widget);
	widget.setVisible(true);
	return app.exec();
}

#include "widget.h"