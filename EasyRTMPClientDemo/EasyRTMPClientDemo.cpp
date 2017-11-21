// EasyRTMPClientDemo.cpp : Defines the entry point for the console application.
//
#include <Windows.h>
#include "../include/EasyRTMPClientAPI.h"

#pragma comment( lib, "../lib/EasyRTMPClient.lib" )
#include <stdio.h>

FILE * g_audio = NULL;
FILE * g_video = NULL;

const char * getValueFromArray( char ** arr, int size, const char * key );

int Easy_APICALL RTMPDataCallBack( 
								  int _channelId, 
								  void * userPtr, 
								  int frameType, 
								  char *pBuf, 
								  RTSP_FRAME_INFO* _frameInfo
								  )									
{
	int ret = 0;

	if ( EASY_SDK_VIDEO_FRAME_FLAG == frameType )
	{
		if ( NULL == g_video )
		{
			if ( EASY_SDK_VIDEO_CODEC_H264 == _frameInfo->codec )
			{
				g_video = fopen( "EasyRTMPClient.h264", "wb" );
			}
			else
			{
				return -1;
			}
		}
		
		if ( EASY_SDK_VIDEO_FRAME_I == _frameInfo->type )
		{
			char sps[2048] = { 0 };
			char pps[2048] = { 0 };
			char* IFrame = NULL;
			unsigned int spsLen,ppsLen,iFrameLen = 0;

			spsLen = _frameInfo->reserved1;							// SPS数据长度
			ppsLen = _frameInfo->reserved2 - _frameInfo->reserved1;	// PPS数据长度
			iFrameLen = _frameInfo->length - spsLen - ppsLen;		// IDR数据长度

			memcpy(sps, pBuf, spsLen);			//SPS数据，包含00 00 00 01
			memcpy(pps, pBuf+spsLen, ppsLen);	//PPS数据，包含00 00 00 01
			IFrame = pBuf + spsLen + ppsLen;	//IDR数据，包含00 00 00 01

			printf("Get I H264(%d * %d) SPS/PPS/IDR Len:%d/%d/%d \ttimestamp:%u.%u\n",
				   _frameInfo->width, _frameInfo->height, spsLen, ppsLen, iFrameLen, 
				   _frameInfo->timestamp_sec, _frameInfo->timestamp_usec );
		}
		else if ( EASY_SDK_VIDEO_FRAME_P == _frameInfo->type )
		{
			printf("Get P H264(%d * %d) Len:%d/%d/%d \ttimestamp:%u.%u\n",
					_frameInfo->width, _frameInfo->height, _frameInfo->length, 
				   _frameInfo->timestamp_sec, _frameInfo->timestamp_usec );
		}

		fwrite( pBuf, 1, _frameInfo->length, g_video ); 
	}
	else if ( EASY_SDK_AUDIO_FRAME_FLAG == frameType )
	{
		if ( NULL == g_audio )
		{
			if ( EASY_SDK_AUDIO_CODEC_AAC == _frameInfo->codec )
			{
				g_audio = fopen( "EasyRTMPClient.aac", "wb" );
			}
			else if ( EASY_SDK_AUDIO_CODEC_MP3 == _frameInfo->codec )
			{
				g_audio = fopen( "EasyRTMPClient.mp3", "wb" );

			}
			else
			{
				return -1;
			}
		}
	
		printf("Get %s Len:%d \ttimestamp:%u.%u\n", 
			EASY_SDK_AUDIO_CODEC_AAC == _frameInfo->codec ? "AAC" : "MP3",
			_frameInfo->length, _frameInfo->timestamp_sec, _frameInfo->timestamp_usec );

		ret = fwrite( pBuf, 1, _frameInfo->length, g_audio ); 
	}
	else if ( EASY_SDK_EVENT_FRAME_FLAG == frameType )
	{
		if ( g_audio )
		{
			fclose( g_audio  );
			g_audio = NULL;
		}

		if ( g_video )
		{
			fclose( g_video  );
			g_video = NULL;
		}

		printf( 
			   "rtmp module exited. error:%03d  code:%s \r\n", 
			   EasyRTMP_GetErrCode( userPtr ), 
			   NULL == pBuf ? "unknow" : pBuf 
			   );
	}
	else if ( EASY_SDK_LOG_FRAME_FLAG == frameType )
	{
		printf( "%s \r\n", pBuf );
	}

	return ret;
}

int main(int argc, char * argv[])
{
	void * rtmp = NULL;
	int ret = 0;
	const char * p = NULL;
	char url[128] = {0};

	if ( argc <= 2 )
	{
		goto _HELP;
	}

	p = getValueFromArray( argv, argc, "-h" );
	if ( ( const char * )-1 == p )
	{
_HELP:
		printf( "EasyRTMPClientDemo.exe -i <rtmp-url>\r\n" );
		printf( "Help Mode: EasyRTMPClient.exe -h\r\n" );
		printf( "rtmp-url: source rtmp address\r\n" );
		printf( "For example: EasyRTMPClientDemo.exe -i rtmp://127.0.0.1/live/911\r\n" );
		
		return 0;
	}

	p = getValueFromArray( argv, argc, "-i" );
	if ( NULL == p || ( const char * )-1 == p )
	{
		goto _HELP;
	}

	EasyRTMP_Init( &rtmp );
	EasyRTMP_SetCallback( rtmp, RTMPDataCallBack );
	EasyRTMP_OpenStream( rtmp, p, rtmp, 500, 0, 0 );

	while ( 1 )
	{
		if ( EasyRTMP_GetErrCode( rtmp ) >= 0 )
		{
			Sleep( 1000 );
			continue;
		}
		Sleep( 10 );
		//break;
	}

	printf( "EasyRTMPClientDemo exited." );

	return 0;
}


const char * getValueFromArray( char ** arr, int size, const char * key )
{
	const char * p = NULL;
	
	for ( int i = 0; i < size; i++ )
	{
		if ( 0 == memcmp( key, arr[i], strlen( key ) + sizeof( char ) ) )
		{
			p = ( const char *  )-1;
			if ( i + 1 < size )
			{
				p = arr[i + 1];
				break;
			}
		}
	}
	
	return p;
}
