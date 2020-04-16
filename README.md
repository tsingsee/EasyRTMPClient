# EasyRTMPClient v3.0 #

EasyRTMPClient功能组件是[TSINGSEE青犀开放平台](http://www.tsingsee.com/ "TSINGSEE青犀")开发和维护的一套非常稳定、易用、支持重连的RTMPClient工具，以SDK的形式提供，全平台支持（包括Windows/Linux 32&64，Android，iOS，ARM各平台），接口简单、兼容性强、且成熟稳定！

> 有了librtmp为什么还要做EasyRTMPClient？主要还是可集成的程度问题，EasyRTMPClient能清晰地回调出帧率、时间戳、首帧是否为I帧、ES数据流、能保存ES进行分析，一直都是围绕着“Easy”来进行架构和设计，能最接近现实项目中的应用需求，更重要的是EasyRTMPClient扩展了RTMP H.265支持！

## 调用示例 ##

- **EasyRTMPClient**：以RTMPClient的形式，从RTMP URL将音视频获取到本地；
	
	Windows编译方法，

    	Visual Studio 2010 编译：./EasyRTMPClient/win/EasyRTMPClient.sln

	Linux编译方法，
		
		chmod +x ./Buildit
		./Buildit

	Usage，
		
		--------------------------------------------------------------
		EasyRTMPClient.exe -d <rtmp-url>[ -s <save-file>]
		Help Mode:   EasyRTMPClient.exe -h
		rtmp-url : source rtmp address
		save-file : yes or no, default is yes
		For example: EasyRTMPClient.exe -d rtmp://live.hkstv.hk.lxdns.com/live/hks -s yes
		--------------------------------------------------------------

- **EasyPlayer-RTMP调用**：[EasyPlayer-RTMP](https://github.com/EasyDSS/EasyPlayer-RTMP "EasyPlayer-RTMP")是由**紫鲸团队**开发和维护的一个RTMP专用播放器项目，目前支持Windows、Android平台、iOS平台，EasyRTMPClient作为其RTMP部分功能组件，为EasyPlayer-RTMP提供稳定可靠、高兼容性的数据连接；

		
## 调用流程 ##
![](http://www.easydarwin.org/github/images/easyrtmpclient/EasyRTMPClient.png)


### RTMPSourceCallBack数据回调说明 ###
EasyRTMPClient可以回调出多种类型的数据：

	#define EASY_SDK_VIDEO_FRAME_FLAG			/* 视频帧数据 */
	#define EASY_SDK_AUDIO_FRAME_FLAG			/* 音频帧数据 */
	#define EASY_SDK_EVENT_FRAME_FLAG			/* 事件帧数据 */
	#define EASY_SDK_RTP_FRAME_FLAG				/* RTP帧数据 */
	#define EASY_SDK_SDP_FRAME_FLAG				/* SDP帧数据 */
	#define EASY_SDK_MEDIA_INFO_FLAG			/* 媒体类型数据 */

EASY\_SDK\_VIDEO\_FRAME\_FLAG数据可支持多种视频格式：
		
	#define EASY_SDK_VIDEO_CODEC_H265			/* H265  */
	#define EASY_SDK_VIDEO_CODEC_H264			/* H264  */


视频帧标识支持

	#define EASY_SDK_VIDEO_FRAME_I				/* I帧 */
	#define EASY_SDK_VIDEO_FRAME_P				/* P帧 */
	#define EASY_SDK_VIDEO_FRAME_B				/* B帧 */
	#define EASY_SDK_VIDEO_FRAME_J				/* JPEG */


EASY\_SDK\_AUDIO\_FRAME\_FLAG数据可支持多种音频格式：
	
	#define EASY_SDK_AUDIO_CODEC_AAC			/* AAC */


## 技术支持 ##

- 邮件：[support@tsingsee.com](mailto:support@tsingsee.com) 

- QQ交流群：<a href="http://jq.qq.com/?_wv=1027&k=2IDkJId" title="EasyPlayer" target="_blank">**544917793**</a>

> **我们同时提供Windows、Linux、Android、iOS、ARM版本的EasyRTMPClient库**：EasyRTMPClient SDK商业使用需要经过授权才能永久使用，商业授权方案可以通过以上渠道进行更深入的技术与合作咨询；



## 获取更多信息 ##

TSINGSEE青犀开放平台：[http://open.tsingsee.com](http://open.tsingsee.com "TSINGSEE青犀开放平台")

Copyright &copy; TSINGSEE.com 2012~2020
