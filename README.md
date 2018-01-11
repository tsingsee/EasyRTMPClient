# EasyRTSPClient SDK #

EasyRTMPClient是EasyDarwin开源流媒体团队开发、提供的一套非常稳定、易用、支持重连的RTMPClient工具，SDK形式提供，全平台支持（包括Windows/Linux 32&64，ARM各平台，Android，iOS），接口简单且成熟稳定！

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

- **EasyPlayer-RTMP调用**：EasyPlayer-RTMP是由EasyDarwin开源团队开发和维护的一个RTMP播放器项目，目前支持Windows(支持多窗口、包含ActiveX，npAPI Web插件)、Android平台，iOS平台，EasyRTMPClient作为其RTMP部分组件，为EasyPlayer-RTMP提供稳定可靠、高兼容性的数据连接；

		
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

- 邮件：[support@easydarwin.org](mailto:support@easydarwin.org) 

- Tel：13718530929

- QQ交流群：[544917793](http://jq.qq.com/?_wv=1027&k=2IDkJId "EasyPlayer")

> **我们同时提供Windows、Linux、ARM、Android、IOS版本的EasyRTMPClient库**：EasyRTMPClient SDK商业使用需要经过授权才能永久使用，商业授权方案可以通过以上渠道进行更深入的技术与合作咨询；


## 获取更多信息 ##

**EasyDarwin**开源流媒体服务器：[www.EasyDarwin.org](http://www.easydarwin.org)

**EasyDSS**商用流媒体解决方案：[www.EasyDSS.com](http://www.easydss.com)

**EasyNVR**无插件直播方案：[www.EasyNVR.com](http://www.easynvr.com)

Copyright &copy; EasyDarwin Team 2012-2017

![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
