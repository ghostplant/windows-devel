#include <QtCore/QtCore>
#include <QtMultimedia/QAudioInput>
#include <QtMultimedia/QAudioOutput>

class QProgram: public QObject {
	Q_OBJECT
public:
	void beginRecord() {
		audioBuffer.open(QIODevice::WriteOnly | QIODevice::Truncate);
		inputAudio = new QAudioInput(format, this);
		QTimer::singleShot(5000, this, SLOT(stopRecording()));
		inputAudio->start(&audioBuffer);
		qWarning()<<"record begin!";
	}

	void beginPlay() {
		audioBuffer.open(QIODevice::ReadOnly);
		outputAudio = new QAudioOutput(format, this);
		connect(outputAudio,SIGNAL(stateChanged(QAudio::State)),SLOT(finishedPlaying(QAudio::State)));
		outputAudio->start(&audioBuffer);  
		qWarning()<<"play begin!";
	}

	QProgram() {
		format.setFrequency(44100);
		format.setChannels(1);
		format.setSampleSize(16);
		format.setCodec("audio/pcm");
		format.setByteOrder(QAudioFormat::LittleEndian);
		format.setSampleType(QAudioFormat::UnSignedInt);

		beginRecord();
	}

private slots:
	void stopRecording() {
		inputAudio->stop();
		delete inputAudio;
		audioBuffer.close();
		qWarning()<<"record end!";

		beginPlay();
	}

	void finishedPlaying(QAudio::State state) {
		if (state == QAudio::IdleState) {
			outputAudio->stop();
			delete outputAudio;
			audioBuffer.close();
			qWarning()<<"play end!";

			exit(0);
		}
	}

private:
	QAudioFormat format;
	QAudioInput* inputAudio;
	QAudioOutput* outputAudio;
	QBuffer audioBuffer;
};

int main(int argc, char *argv[]) {
	QCoreApplication app(argc,argv);
	QProgram program;
	return app.exec();
}

#include "main.h"
