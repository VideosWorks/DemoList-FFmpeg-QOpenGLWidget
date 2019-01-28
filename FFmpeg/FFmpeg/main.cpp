#include <iostream>

extern "C"
{
	#include "libavformat/avformat.h"
	#include "libswresample/swresample.h"
	#include "libswscale/swscale.h"
}

using namespace std; 

double rationalToDouble(AVRational rational)
{
	return rational.den == 0 ? 0.0 : double(rational.num) / double(rational.den);
}

int main()
{
	//1.���װ���������
	av_register_all();
	avcodec_register_all();
	avformat_network_init();

	char* url = "123.mp4";
	AVFormatContext* informatContext = nullptr; //�ڲ����ݣ�����
	AVDictionary *options = nullptr;  //���ݶ���
	int ret = avformat_open_input(&informatContext, url, NULL, &options);
	if (ret != 0)
	{
		char buf[1024] = {0};
		av_strerror(ret, buf, sizeof(buf));
		std::cout << "avformat_open_input failed " << buf << std::endl;
		getchar();
		return 0;
	}
	std::cout << "avformat_open_input success" << std::endl;
	
	//��ӡ��Ƶ����Ϣ
	avformat_find_stream_info(informatContext, NULL); //��Ҫ���ò��ܻ�ȡ���µ�fomat
	av_dump_format(informatContext, 0, url, 0); //is_output
	std::cout << "----------------------------------------" << std::endl;

	//��ȡ����Ƶ����Ϣ
	unsigned int nVideoIndex = 0;
	unsigned int nAudioIndex = 1;
	int nVideoWidth = 0;
	int nVideoHeight = 0;
	for (unsigned int nIndex = 0; nIndex < informatContext->nb_streams; ++nIndex)
	{
		std::cout << "video duration: " << informatContext->duration / AV_TIME_BASE << std::endl;

		AVStream* avstream = informatContext->streams[nIndex]; //AVStream�ڲ����ݣ�����
		if (avstream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)  
		{
			nVideoIndex = nIndex;
			std::cout << "��Ƶ��Ϣ" << std::endl;
			std::cout << "video index: " << nVideoIndex << std::endl;
			std::cout << "video bit rate: " << avstream->codecpar->bit_rate / 1000 << std::endl;
			std::cout << "video codec-id: " << avstream->codecpar->codec_id << std::endl;
			std::cout << "video format: " << avstream->codecpar->format << std::endl;
			std::cout << "video width: " << avstream->codecpar->width << std::endl;
			std::cout << "video height: " << avstream->codecpar->height << std::endl;
			std::cout << "video fps: " << rationalToDouble(avstream->avg_frame_rate) << std::endl;
			std::cout << "----------------------------------------" << std::endl;
			nVideoWidth = avstream->codecpar->width;
			nVideoHeight = avstream->codecpar->height;
		}
		else if (avstream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			nAudioIndex = nIndex;
			std::cout << "��Ƶ��Ϣ" << std::endl;
			std::cout << "audio index: " << nAudioIndex << std::endl;
			std::cout << "audio bit rate: " << avstream->codecpar->bit_rate / 1000 << std::endl;
			std::cout << "audio codec-id: " << avstream->codecpar->codec_id << std::endl;
			std::cout << "audio format: " << avstream->codecpar->format << std::endl;
			std::cout << "audio samplerate " << avstream->codecpar->sample_rate << std::endl;
			std::cout << "audio channels: " << avstream->codecpar->channels << std::endl;
			std::cout << "audio channel layout: " << avstream->codecpar->channel_layout << std::endl;
			std::cout << "----------------------------------------" << std::endl;
		}
	}

	//����һ�ֻ�ȡ����Ƶ��Ϣ�����ķ�ʽ
	nVideoIndex = av_find_best_stream(informatContext, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
	if (ret < 0)
	{
		char buf[1024] = { 0 };
		av_strerror(ret, buf, sizeof(buf));
		std::cout << "vedio av_find_best_stream failed " << buf << std::endl;
		getchar();
		return 0;
	}
	std::cout << "vedio av_find_best_stream index: " << nVideoIndex << std::endl;

	nAudioIndex = av_find_best_stream(informatContext, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
	if (ret < 0)
	{
		char buf[1024] = { 0 };
		av_strerror(ret, buf, sizeof(buf));
		std::cout << "audio av_find_best_stream failed " << buf << std::endl;
		getchar();
		return 0;
	}
	std::cout << "audio av_find_best_stream index: " << nAudioIndex << std::endl;

	//2.�������������
	//��Ƶ������
	AVCodec* videoCodec = avcodec_find_decoder(informatContext->streams[nVideoIndex]->codecpar->codec_id);
	if (!videoCodec)
	{
		std::cout << "avcodec_find_decoder failed " << std::endl;
		getchar();
		return 0;
	}
	std::cout << "vedio avcodec_find_decoder find index: " << informatContext->streams[nVideoIndex]->codecpar->codec_id << std::endl;

	AVCodecContext *videoCC = avcodec_alloc_context3(videoCodec);
	avcodec_parameters_to_context(videoCC, informatContext->streams[nVideoIndex]->codecpar); //�������ݵķ�ʽ
 	videoCC->thread_count = 8; //�����߳�����

	ret = avcodec_open2(videoCC, videoCodec, NULL);
	if (ret < 0)
	{
		char buf[1024] = { 0 };
		av_strerror(ret, buf, sizeof(buf));
		std::cout << "vedio avcodec_open2 failed " << buf << std::endl;
		getchar();
		return 0;
	}
	std::cout << "video avcodec_open2 success" << std::endl;

	//��Ƶ������
	AVCodec* audioCodec = avcodec_find_decoder(informatContext->streams[nAudioIndex]->codecpar->codec_id);
	if (!audioCodec)
	{
		std::cout << "avcodec_find_decoder failed " << std::endl;
		getchar();
		return 0;
	}
	std::cout << "audio avcodec_find_decoder find index: " << informatContext->streams[nAudioIndex]->codecpar->codec_id << std::endl;

	AVCodecContext* audioCC = avcodec_alloc_context3(audioCodec);
	avcodec_parameters_to_context(audioCC, informatContext->streams[nAudioIndex]->codecpar);

	ret = avcodec_open2(audioCC, audioCodec, NULL);
	if (ret < 0)
	{
		char buf[1024] = { 0 };
		av_strerror(ret, buf, sizeof(buf));
		std::cout << "audio avcodec_open2 failed " << buf << std::endl;
		getchar();
		return 0;
	}
	std::cout << "audio avcodec_open2 success" << std::endl;


	//3.���ظ�ʽת�����������
// 	SwsContext* swscontex = nullptr;
// 	swscontex = sws_getCachedContext(swscontex, 
// 		nVideoWidth, nVideoHeight, AV_PIX_FMT_YUV420P,
// 		nVideoWidth, nVideoHeight, AV_PIX_FMT_RGBA,
// 		SWS_BICUBIC, nullptr, nullptr, nullptr);
// 	

	//4.�ز�����ظ�ʽ������
	SwrContext* swrcontext = swr_alloc();
	swr_alloc_set_opts(swrcontext, 
		av_get_default_channel_layout(audioCC->channels), 
		AV_SAMPLE_FMT_FLT,
		audioCC->sample_rate, //�������
		av_get_default_channel_layout(audioCC->channels),
		(AVSampleFormat)audioCC->sample_fmt,
		audioCC->sample_rate, //��������
		0, 0);
	ret = swr_init(swrcontext);
	if (ret < 0)
	{
		char buf[1024] = { 0 };
		av_strerror(ret, buf, sizeof(buf));
		std::cout << "audio swr_init failed " << buf << std::endl;
		getchar();
		return 0;
	}

	AVPacket* avpacket = av_packet_alloc(); //AVPacket �ڲ����ݣ�����
	AVFrame* frame = av_frame_alloc();
	unsigned char* rgb = nullptr;
	uint8_t* pcm = nullptr;
	while (true)
	{
		ret = av_read_frame(informatContext, avpacket);
		if (ret < 0)
		{
			break;
			//ѭ������
// 			std::cout << "******************end*******************" << std::endl;
// 			int ms = 3000; //timestamp��μ��㣿����
// 			long long pos = (double)ms / (double)1000 * rationalToDouble(informatContext->streams[nVideoIndex]->time_base);
// 			av_seek_frame(informatContext, nVideoIndex, 0, AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_FRAME); //Ѱ�ҹؼ�֡
// 			continue;
		}

		AVCodecContext *codecContext = NULL;
		if (avpacket->stream_index == nVideoIndex)
		{
			codecContext = videoCC;
		}
		else if (avpacket->stream_index == nAudioIndex)
		{
			codecContext = audioCC;
		}

		//����packet���������
		ret = avcodec_send_packet(codecContext, avpacket); 
		//���ü���-1���Զ�����packet�ռ�
		av_packet_unref(avpacket);
		if (ret != 0)
		{
			char buf[1024] = { 0 };
			av_strerror(ret, buf, sizeof(buf));
			std::cout << "audio avcodec_send_packet failed " << buf << std::endl;
			continue;
		}

		while (true)
		{
			//�ӻ�������л�ȡ֡����
			ret = avcodec_receive_frame(codecContext, frame);
			if (ret != 0)
			{
				break;
			}
			std::cout << "recv format " << frame->format << "  " << frame->linesize[0] << std::endl;

			if (codecContext == videoCC) //��Ƶ���ظ�ʽת��
			{
				std::cout << "���ظ�ʽת��" << std::endl;
				if (!rgb)
				{
					rgb = new unsigned char[frame->width * frame->height * 4];
				}
				uint8_t* data[2] = { 0 };
				data[0] = rgb;

				int linesize[2] = { 0 };
				linesize[0] = /*frame->width * 4*/ 1024;  //���Ϊɶ�ᱨ������

				SwsContext* swscontex = nullptr;
				swscontex = sws_getCachedContext(swscontex,
					frame->width, frame->height, (AVPixelFormat)frame->format,
					frame->width, frame->width, AV_PIX_FMT_RGBA,
					SWS_BILINEAR, nullptr, nullptr, nullptr);

				//���������Ҫ�ú���⣿����
				ret = sws_scale(swscontex,
					frame->data, frame->linesize, 0, frame->height,
					data, linesize);

				std::cout << "scale size = " << ret << std::endl;
			}
			else if (codecContext == audioCC)//��Ƶ�ز���
			{
				std::cout << "��Ƶ�ز���" << std::endl;
				if (!pcm)
				{
					pcm = new uint8_t[frame->nb_samples * 2 * 2]; //˫ͨ����S16��ʽ
				}

				//��Ҫ�ú����nb_samples�ĺ���
				ret = swr_convert(swrcontext, &pcm, frame->nb_samples, (const uint8_t**)frame->data, frame->nb_samples);
				std::cout << "swr_convert size = " << ret << std::endl;
			}
		}
	}
	av_packet_free(&avpacket);
	av_frame_free(&frame);
	std::cout << "vedio and audio decodec success" << std::endl;

	//5.����ռ�
	avformat_close_input(&informatContext);
	avcodec_free_context(&videoCC);
	avcodec_free_context(&audioCC);
	swr_free(&swrcontext);
	delete rgb;
	rgb = nullptr;
	getchar();
	return 0;
}