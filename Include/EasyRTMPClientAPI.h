/*
	Copyright (c) 2013-2015 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.easydarwin.org
*/
#ifndef _Easy_RTMPClient_API_H
#define _Easy_RTMPClient_API_H

#include "EasyTypes.h"

#define	RTMP_PROG_NAME	"EasyRTMPClient v1.6.17.0901"

/*
	_channelId:		通道号,暂时不用
	_channelPtr:	通道对应对象,暂时不用
	_frameType:		EASY_SDK_VIDEO_FRAME_FLAG/EASY_SDK_AUDIO_FRAME_FLAG/EASY_SDK_EVENT_FRAME_FLAG/...	
	_pBuf:			回调的数据部分，具体用法看Demo
	_frameInfo:		帧结构数据
*/
typedef int (Easy_APICALL *RTSPSourceCallBack)( int _channelId, void *_channelPtr, int _frameType, char *pBuf, RTSP_FRAME_INFO* _frameInfo);


/*
	userPtr: 用户自定义数据指针
	frameType: EASY_SDK_VIDEO_FRAME_FLAG/EASY_SDK_AUDIO_FRAME_FLAG/EASY_SDK_EVENT_FRAME_FLAG/...	
	pBuf: 原始音频、视频数据流
	segLen: 当前帧数据分片长度
	length: 当前帧长度   
	timestamp: 当前帧时间戳
remark:
  if ( NULL == pBuf && -1 == segLen ) 
  { 
      1. len值指定为当前音频/视频（音频/视频由frameType区分）编码类型，
      2. len值作为编码类型时，其可能的值参见EasyTypes.h 定义音频/视频编码类型
	  3. len值作为编码类型时，此类回调在一次rtmp会话过程中音频/视频各回调一次，
	     且回调时机总是优先于传递具体音频/视频数据之前
  }
*/
/*
typedef int (Easy_APICALL *RTMPSourceCallBack)( 
											   void * userPtr,
											   int frameType,
											   unsigned char * pBuf,
											   unsigned int segLen,
											   unsigned int length,
											   unsigned int timestamp 
											   );
*/



#define RTMPSourceCallBack                    RTSPSourceCallBack
#define Easy_RTMP_Handle                      void *
#define EASY_SDK_AUDIO_CODEC_MP3              2

typedef enum
{
	Easy_RTMP_Unsupported_codec = -100,
	Easy_RTMP_Handshake_error = -101,
	Easy_RTMP_Network_exception = -102,
	Easy_RTMP_Connect_error = -103,
	Easy_RTMP_Createstream_error = -104,
	Easy_RTMP_Play_error = -105
}Easy_RTMP_ERROR_CODE;


#ifdef __cplusplus
extern "C"
{
#endif
	/* 获取最后一次错误的错误码 */
	Easy_API int Easy_APICALL EasyRTMP_GetErrCode(Easy_RTMP_Handle handle);

	/* 激活 */
#ifdef ANDROID
	Easy_API int Easy_APICALL EasyRTMP_Activate(char *license, char* userPtr);
#else
	Easy_API int Easy_APICALL EasyRTMP_Activate(char *license);
#endif

	/* 创建RTMPClient句柄  返回0表示成功，返回非0表示失败 */
	Easy_API int Easy_APICALL EasyRTMP_Init(Easy_RTMP_Handle *handle);

	/* 释放RTMPClient 参数为RTMPClient句柄 */
	Easy_API int Easy_APICALL EasyRTMP_Deinit(Easy_RTMP_Handle *handle);

	/* 设置数据回调 */
	Easy_API int Easy_APICALL EasyRTMP_SetCallback(
												   Easy_RTMP_Handle handle, 
												   RTMPSourceCallBack _callback
												   );  

	/* 打开网络流 */
	Easy_API int Easy_APICALL EasyRTMP_OpenStream(
												  Easy_RTMP_Handle handle, 
												  //int _channelid, 
												  const char *_url, 
												  //EASY_RTP_CONNECT_TYPE _connType, 
												  //unsigned int _mediaType, 
												  //char *_username, 
												  //char *_password, 
												  void *userPtr, //用户自定义数据指针
												  int _reconn/*1000表示长连接,即如果网络断开自动重连, 其它值为连接次数*/, 
												  int outFlvTag/*默认为0,即回调输出原始音视频帧, 如果为1,则输出flv tag包*/, 
												  //int heartbeatType/*0x00:不发送心跳 0x01:OPTIONS 0x02:GET_PARAMETER*/,
												  int _verbosity/*日志打印输出等级，0表示不输出*/
												  );
	
	/* 关闭网络流 */
	Easy_API int Easy_APICALL EasyRTMP_CloseStream(Easy_RTMP_Handle handle);

#ifdef __cplusplus
}
#endif

#endif
