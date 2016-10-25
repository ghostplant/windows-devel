#include <QtCore/QtCore>
#include <unistd.h>

class Thread: public QThread {
	Q_OBJECT
protected:
	void run() {
		while (1) {
			qDebug()<<"thread thread:"<<currentThreadId();
			sleep(2);
		}
	}
};

int main(int argc, char *argv[]) {
	QCoreApplication a(argc, argv);
	Thread thread;
	thread.start();
	while (1) {
		qDebug()<<"main thread:"<<QThread::currentThreadId();
		sleep(1);
	}
	return a.exec();
}

#include "qtmain.h"
