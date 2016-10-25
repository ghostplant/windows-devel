#include <QtCore/QtCore>

int main(int argc, char *argv[]) {
	QCoreApplication app(argc, argv);

	QProcess proc;
	QStringList arguments;
	arguments << "-na";
	proc.start("netstat", arguments);

	if (!proc.waitForStarted()) // 等待进程启动
		return 1;
	proc.closeWriteChannel(); // 关闭写通道,因为没有向进程写数据,没用到

	QByteArray procOutput;
	while (!proc.waitForFinished());// 等待进程结束
	procOutput = proc.readAll();// 读取进程输出到控制台的数据
	qWarning() << procOutput.data(); // 保存进程的控制台输出读到的数据
	return 0;
}