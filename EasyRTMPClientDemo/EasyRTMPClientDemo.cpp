// EasyRTMPClientDemo.cpp : Defines the entry point for the console application.
//
#include <Windows.h>
#include "../EasyRTMPClient/EasyRTMPClientAPI.h"

#pragma comment( lib, "../Debug/EasyRTMPClient.lib" )
#include <stdio.h>

FILE * g_audio = NULL;
FILE * g_video = NULL;

const char * getValueFromArray( char ** arr, int size, const char * key );

int Easy_APICALL RTMPDataCallBack( 
								  int _channelId, 
								  void * userPtr, 
								  int frameType, 
								  char *pBuf, 
								  RTSP_FRAME_INFO* frameInfo
								  )									
{
	int ret = 0;

	if ( EASY_SDK_VIDEO_FRAME_FLAG == frameType )
	{
		if ( NULL == g_video )
		{
			if ( EASY_SDK_VIDEO_CODEC_H264 == frameInfo->codec )
			{
				g_video = fopen( "EasyRTMPClient.h264", "wb" );
			}
			else
			{
				return -1;
			}
		}
		
		if ( EASY_SDK_VIDEO_FRAME_I == frameInfo->type )
		{
			printf( "sps+pps+iframe \r\n" );
		}

		fwrite( pBuf, 1, frameInfo->length, g_video ); 
	}
	else if ( EASY_SDK_AUDIO_FRAME_FLAG == frameType )
	{
		if ( NULL == g_audio )
		{
			if ( EASY_SDK_AUDIO_CODEC_AAC == frameInfo->codec )
			{
				g_audio = fopen( "EasyRTMPClient.aac", "wb" );
			}
			else if ( EASY_SDK_AUDIO_CODEC_MP3 == frameInfo->codec )
			{
				g_audio = fopen( "EasyRTMPClient.mp3", "wb" );
			}
			else
			{
				return -1;
			}
		}
		ret = fwrite( pBuf, 1, frameInfo->length, g_audio ); 
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

	return ret;
}


int main(int argc, char * argv[])
{
	void * rtmp = NULL;
	int ret = 0;
	const char * p = NULL;
	char url[128] = {0};

	p = getValueFromArray( argv, argc, "-i" );

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
			if ( i + 1 < size )
			{
				p = arr[i + 1];
				break;
			}
		}
	}
	
	return p;
}
