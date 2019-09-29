/*
Copyright (c) 2013-2017 EasyDarwin.ORG.  All rights reserved.
Github: https://github.com/EasyDarwin
WEChat: EasyDarwin
Website: http://www.EasyDarwin.org
*/
#include <stdio.h>
#include <string.h>
#include "EasyRTMPClientAPI.h"
#include "getopt.h"

#ifdef _WIN32
#include <winsock.h>

//available time to 20191130
#define KEY "59615A67426F69576B5A7341436D74646F7054317065314659584E35556C524E55454E73615756756443356C65475673567778576F502B6C2F69426C59584E35"
#else
#define KEY "59615A6742704F576B596F41436D74646F7054317066466C59584E35636E527463474E7361575675644E5A58444661672F36582B4947566863336B3D"
#endif

FILE* fVideo = NULL;
FILE* fAudio = NULL;

char fRTMPURL[100] = { 0 };		//rtsp source addrs
bool fSaveFile = true;		//true : save file     false ： don't save

int audioindex = 0;
int laststamp = 0;
/* RTMPClient数据回调 */
int __RTMPClientCallBack(int _channelId, void* _channelPtr, int _frameType, char* _pBuf, EASY_FRAME_INFO* _frameInfo)
{
	if (_frameType == EASY_SDK_VIDEO_FRAME_FLAG)//回调视频数据，包含00 00 00 01头
	{
		if (_frameInfo->codec == EASY_SDK_VIDEO_CODEC_H264)
		{
			/*
			每一个I关键帧都是SPS+PPS+IDR的组合
			|---------------------|----------------|-------------------------------------|
			|                     |                |                                     |
			0-----------------reserved1--------reserved2-------------------------------length
			*/
			if (_frameInfo->type == EASY_SDK_VIDEO_FRAME_I)
			{
				printf("Get I H264(%d * %d) IDR Len:%d \ttimestamp:%u.%u\n", _frameInfo->width, _frameInfo->height, _frameInfo->length, _frameInfo->timestamp_sec, _frameInfo->timestamp_usec);
			}
			else if (_frameInfo->type == EASY_SDK_VIDEO_FRAME_P)
			{
				printf("Get P H264(%d * %d) Len:%d \ttimestamp:%u.%u\n", _frameInfo->width, _frameInfo->height, _frameInfo->length, _frameInfo->timestamp_sec, _frameInfo->timestamp_usec);
			}

			if (fSaveFile)
			{
				if (fVideo == NULL)
				{
					fVideo = ::fopen("./video.h264", "wb");
				}

				::fwrite(_pBuf, 1, _frameInfo->length, fVideo);
			}
		}
		else if (_frameInfo->codec == EASY_SDK_VIDEO_CODEC_H265)
		{
			printf("Get H265(%d * %d) Len:%d \ttimestamp:%u.%u\n", _frameInfo->width, _frameInfo->height, _frameInfo->length, _frameInfo->timestamp_sec, _frameInfo->timestamp_usec);
		}
		else if (_frameInfo->codec == EASY_SDK_VIDEO_CODEC_MJPEG)
		{
			if (fSaveFile)
			{
				if (fVideo == NULL)
				{
					fVideo = ::fopen("./video.mjpeg", "wb");
				}

				::fwrite(_pBuf, 1, _frameInfo->length, fVideo);
			}

			printf("Get MJPEG Len:%d \ttimestamp:%u.%u\n", _frameInfo->length, _frameInfo->timestamp_sec, _frameInfo->timestamp_usec);
		}
		else if (_frameInfo->codec == EASY_SDK_VIDEO_CODEC_MPEG4)
		{
			if (fSaveFile)
			{
				if (fVideo == NULL)
				{
					fVideo = ::fopen("./video.mpeg4", "wb");
				}

				::fwrite(_pBuf, 1, _frameInfo->length, fVideo);
			}

			printf("Get MPEG4 Len:%d \ttimestamp:%u.%u\n", _frameInfo->length, _frameInfo->timestamp_sec, _frameInfo->timestamp_usec);
		}
	}
	else if (_frameType == EASY_SDK_AUDIO_FRAME_FLAG)//回调音频数据
	{
		if (_frameInfo->codec == EASY_SDK_AUDIO_CODEC_AAC)
		{
			if (fSaveFile)
			{
				if (fAudio == NULL)
				{
					fAudio = ::fopen("./audio.aac", "wb");
				}
			}

			int curstamp = _frameInfo->timestamp_sec*1000 + _frameInfo->timestamp_usec/1000;
			printf("Get AAC(%d-%d-%d) Len:%d \t timestamp:%u.%u index=%d, interval=%d\n", _frameInfo->sample_rate, _frameInfo->channels, _frameInfo->bits_per_sample,
				_frameInfo->length, _frameInfo->timestamp_sec, _frameInfo->timestamp_usec, audioindex++, curstamp - laststamp);
			laststamp = curstamp;
		}

		if (fSaveFile)
			::fwrite(_pBuf, 1, _frameInfo->length, fAudio);
	}
	else if (_frameType == EASY_SDK_EVENT_FRAME_FLAG)//回调连接状态事件
	{

	}
	else if (_frameType == EASY_SDK_MEDIA_INFO_FLAG)//回调出媒体信息
	{
		if (_pBuf != NULL)
		{
			EASY_MEDIA_INFO_T mediainfo;
			memset(&mediainfo, 0x00, sizeof(EASY_MEDIA_INFO_T));
			memcpy(&mediainfo, _pBuf, sizeof(EASY_MEDIA_INFO_T));
			printf("RTSP DESCRIBE Get Media Info: video:%u fps:%u audio:%u channel:%u sampleRate:%u \n",
				mediainfo.u32VideoCodec, mediainfo.u32VideoFps, mediainfo.u32AudioCodec, mediainfo.u32AudioChannel, mediainfo.u32AudioSamplerate);
		}
	}
	return 0;
}

void usage(char const* progName)
{
	printf("Usage: %s <rtsp-url> \n", progName);
}

void PrintUsage(char const* progName)
{
	printf("Usage:\n");
	printf("--------------------------------------------------------------\n");
	printf("%s -d <rtmp-url>[ -s <save-file>]\n", progName);
	printf("Help Mode:   %s -h \n", progName);
	printf("rtmp-url : source rtmp address\nsave-file : yes or no, default is yes\n");
	printf("For example: %s -d rtmp://live.hkstv.hk.lxdns.com/live/hks -s yes\n", progName);
	printf("--------------------------------------------------------------\n");
}

int main(int argc, char** argv)
{
	int ch;

	char* fRTMPURL = NULL;
	// We need at least one "rtsp://" URL argument:
	if (argc < 2) 
	{
		PrintUsage(argv[0]);
		printf("Press Enter exit...\n");
		getchar();
		return 1;
	}

	while ((ch = getopt(argc,argv, "h:d:m:s:")) != EOF) 
	{
		switch(ch)
		{
		case 'h':
			PrintUsage(argv[0]);
			return 0;
			break;
		case 'd':
			fRTMPURL = optarg;
			break;
		case 's':
			if((strlen(optarg) == 2) && ((0 == strcmp(optarg, "NO"))|| (0 == strcmp(optarg, "no"))))
			{
				fSaveFile = false;
			}
			else
			{
				fSaveFile = true;//默认存文件
			}
			break;
		default:
			break;
		}
	}

	int iret = 0;
	iret = EasyRTMPClient_Activate(KEY);
	if(iret < 0)
	{
		printf("EasyRTMPClient_Activate error! ret=%d\n", iret);
	}
	Easy_Handle rmtp_handle = EasyRTMPClient_Create();
	iret = EasyRTMPClient_SetCallback(rmtp_handle, __RTMPClientCallBack);
	iret = EasyRTMPClient_StartStream(rmtp_handle, 0, fRTMPURL, NULL);

	printf("Press Enter exit...\n");
	getchar();
	EasyRTMPClient_Release(rmtp_handle);
	if(fVideo)
	{
		::fclose(fVideo);
		fVideo = NULL;
	}
	if(fAudio)
	{
		::fclose(fAudio);
		fAudio = NULL;
	}
	return 0;
}