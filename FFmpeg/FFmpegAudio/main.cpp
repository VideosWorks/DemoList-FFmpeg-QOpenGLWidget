
#include <QtCore/QCoreApplication>
#include <QThread>
#include <QAudioFormat>
#include <QAudioOutput>

//ffmpeg -i 123.mp4 -f s16le 123.aac  抽取音频数据
int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	QAudioFormat audioFormat;
	audioFormat.setSampleRate(44100);  //采样率
	audioFormat.setSampleSize(16);  //样本大小S16
	audioFormat.setChannelCount(2); //通道数
	audioFormat.setCodec("audio/pcm"); //编码格式
	audioFormat.setSampleType(QAudioFormat::UnSignedInt); //样本类型
	audioFormat.setByteOrder(QAudioFormat::LittleEndian);  //字节序 //为什么要采用小端字节序???

	QAudioOutput* pOutput = new QAudioOutput(audioFormat);
	QIODevice* io = pOutput->start();

	int nReadSize = 1024 * 2 * 2;  //单通道样本数量 * 样本类型 * 通道数
	char* pcm = new char[nReadSize];
	FILE* pfile = fopen("123.aac", "rb");
	while (!feof(pfile))
	{
		//缓冲区大小保证
		if (pOutput->bytesFree() < nReadSize)
		{
			QThread::msleep(1);
			continue;
		}

		//读取音频数据
		int len = fread(pcm, 1, nReadSize, pfile);
		if (len <= 0)
		{
			break;
		}

		//写入输出缓冲区
		io->write(pcm, len);
	}

	delete pOutput;
	delete pcm;
	fclose(pfile);

	return a.exec();
}
