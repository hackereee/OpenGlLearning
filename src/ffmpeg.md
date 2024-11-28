# 音视频同步
## 问题及概念
1. 音频与视频时钟是什么？
2. 如何创建一个准确的音视频时钟
* 视频以每一帧创建
* 音频以每一个样本创建
3. 视频同步音频算法


## 播放器设置顺序
###  基本结构
### 视频
* duration：每一帧需要播放的时长; 
实现： （按顺序回退）
1. 优先使用av_frame_get_pkt_duration 获取帧率时长，如果返回 > 0 则说明解码器设置了pkt_duration,直接将该值 * 时间基(time_base)即可
2. 使用当前帧与上一针的pts差值计算
3. 通过 av_guess_frame_rate()返回帧率，通过av_q2d方法将帧率转为秒
4. 以上3步都无法正确返回时长时，采用固定时长，固定时长由固定帧率计算，这个误差会很大，但一般也不会走到这里，采用100FPS计算帧率也就是0.01秒/Frame

代码示例

```C++
double calculate_frame_duration(const AVFrame *current_frame, const AVFrame *previous_frame, 
                                AVRational time_base, AVRational frame_rate) {
    // 优先尝试使用 pkt_duration
    int64_t pkt_duration = av_frame_get_pkt_duration(current_frame);
    if (pkt_duration > 0) {
        return pkt_duration * av_q2d(time_base);
    }

    // 如果 pkt_duration 不可用，使用 PTS 差值
    if (current_frame && previous_frame) {
        return (current_frame->pts - previous_frame->pts) * av_q2d(time_base);
    }

    // 如果 PTS 不可用，退回到帧率计算
    if (av_q2d(frame_rate) > 0) {
        return 1.0 / av_q2d(frame_rate);
    }

    // 无法确定帧时长
    return 0.01;
}

```
### 音频
* duration: 每一帧音频的播放时长。
音频不像视频画面一样通过视频帧来组合，而是通过采样点采样率同时决定的，所以“一帧音频”为采样点数/采样率，采样点数就是没播放一个单位（一帧）音频的所有点数，所以音频duration计算如下：
1. 通过`av_frame_get_pkt_duration`获取时长，然后*时间基
2. 若1获取不到时长，则通过采样点数 / 采样率 
3. 若2还获取不到时长，我们返回以 1024 采样点 / 44100 采样率的时长