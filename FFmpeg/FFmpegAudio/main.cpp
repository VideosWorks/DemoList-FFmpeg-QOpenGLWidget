
#include <QtCore/QCoreApplication>
#include <QThread>
#include <QAudioFormat>
#include <QAudioOutput>

//ffmpeg -i 123.mp4 -f s16le 123.aac  ��ȡ��Ƶ����
int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	QAudioFormat audioFormat;
	audioFormat.setSampleRate(44100);  //������
	audioFormat.setSampleSize(16);  //������СS16
	audioFormat.setChannelCount(2); //ͨ����
	audioFormat.setCodec("audio/pcm"); //�����ʽ
	audioFormat.setSampleType(QAudioFormat::UnSignedInt); //��������
	audioFormat.setByteOrder(QAudioFormat::LittleEndian);  //�ֽ��� //ΪʲôҪ����С���ֽ���???

	QAudioOutput* pOutput = new QAudioOutput(audioFormat);
	QIODevice* io = pOutput->start();

	int nReadSize = 1024 * 2 * 2;  //��ͨ���������� * �������� * ͨ����
	char* pcm = new char[nReadSize];
	FILE* pfile = fopen("123.aac", "rb");
	while (!feof(pfile))
	{
		//��������С��֤
		if (pOutput->bytesFree() < nReadSize)
		{
			QThread::msleep(1);
			continue;
		}

		//��ȡ��Ƶ����
		int len = fread(pcm, 1, nReadSize, pfile);
		if (len <= 0)
		{
			break;
		}

		//д�����������
		io->write(pcm, len);
	}

	delete pOutput;
	delete pcm;
	fclose(pfile);

	return a.exec();
}
