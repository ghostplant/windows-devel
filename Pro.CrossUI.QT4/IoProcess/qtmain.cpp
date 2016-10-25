#include <QtCore/QtCore>

int main(int argc, char *argv[]) {
	QCoreApplication app(argc, argv);

	QProcess proc;
	QStringList arguments;
	arguments << "-na";
	proc.start("netstat", arguments);

	if (!proc.waitForStarted()) // �ȴ���������
		return 1;
	proc.closeWriteChannel(); // �ر�дͨ��,��Ϊû�������д����,û�õ�

	QByteArray procOutput;
	while (!proc.waitForFinished());// �ȴ����̽���
	procOutput = proc.readAll();// ��ȡ�������������̨������
	qWarning() << procOutput.data(); // ������̵Ŀ���̨�������������
	return 0;
}