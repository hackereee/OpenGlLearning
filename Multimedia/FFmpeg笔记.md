# 基础知识
* CODEC：编解码器，通过指定算法直线编码（ENCODE）与解码（DECODE），常见视频编码算法有： MPEG-1, MPEG-2, H.264 (aka AVC), H.265 (aka HEVC), H.266 (aka VVC), VP8, 
VP9, AV1；常见音频编码算法有： Mp3, AAC, Opus, Ogg 
Vorbis, HE-AAC, Dolby Digital, FLAC, ALAC.
* 比特率（BITRATE）：用来计算视频/音频每一秒的数据量（kbps/Mbps）
* 容器/封装（CONTAINER）： 用于将多媒体数据对象进行封装，常见的格式有：MP4 (.mp4), MKV (.mkv), WEBM (.webm), MOV (.mov), MXF (.mxf ), ASF (.asf ), MPEG Transport Stream (.ts), CAF(Core Audio Format, .caf ), WEBP (.webp).
* MUX与DEMUX:我直接引用AI的回答：
>>MUX是将多个信号或数据流合并成一个信号的过程。它把不同来源的视频、音频、数据等信号组合成一个复合信号,以便在通信网络中传输。
>>
>>DEMUX则是MUX的逆过程,它从复合信号中分离出独立的视频、音频、数据等信号。在接收端,通过DEMUX可以获取到原始的媒体流。
>>
>>例如,在视频压缩编码中,视频和音频信号会各自独立编码,然后通过MUX合并成一个视频流,以便存储或传输。在播放时,通过DEMUX可以分离出视频和音频信号,送入各自的解码器进行解码,最后重新同步合成为完整的视频。
>>
>>所以MUX和DEMUX都是实现信号多路传输必不可少的技术,它们常用于数字电视、数字媒体的存储和传输系统中。MUX混合信号,DEMUX分离信号,两者组合起来实现信号的复用与分离。
* TRANSMUXING: TRANSMUXING是一种特殊形式的数字信号处理,它指的是在不解码的情况下,直接将数字媒体流中的音视频数据进行重muxing(重复用)的过程。
