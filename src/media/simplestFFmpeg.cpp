#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <common/gl_common.h>
#include "simplestFFmpeg.h"
#include <iostream>
#include <thread>
#include <Program/shader.h>
#include <condition_variable>
#include <vedioFrame.cpp>
#include <cstdint>

/// @brief 顶点及纹理坐标
const float vertices[] = {
    // 顶点坐标          // 纹理坐标
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f};

const int indices[] = {
    0, 1, 2,
    2, 3, 1};

const int BufferSize = 3;

VideoFrameQueue videoFrameQueue(BufferSize);


void initRenderer(int width, int height)
{
    initGlEnv(width, height, "SimplestFFmpeg");
    Shader shader("shaders/media.vert", "shaders/media.frag");
    shader.use();
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    //创建纹理
    GLuint texture;
    glGenTextures(1, &texture);
    //设置环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //设置过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //开启多级渐远纹理
    glGenerateMipmap(GL_TEXTURE_2D);
    // 开启垂直同步
    glfwSwapInterval(1);
    startRender(texture);
}

void startRender(const GLuint &texture)
{
    double startTime = glfwGetTime();
    while (true)
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(1);
        VedioFrame *frame = videoFrameQueue.pop();
        double currentTime = glfwGetTime();
        double playTime = frame->playTime;
        //如果错过播放时间帧，则跳过该帧渲染，如果不到那就等待下一帧的播放时间
        if (playTime > currentTime - startTime)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds((int)((playTime - currentTime + startTime) * 1000)));
        }else{
            std::cout << "frame delay" << currentTime - startTime - playTime << std::endl;
            continue;
        }
        // 更新纹理
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame->frame->width, frame->frame->height, 0, GL_RGB, GL_UNSIGNED_BYTE, frame->frame->data);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(glfwGetCurrentContext());
        glfwPollEvents();
    }
}


void playVideo()
{
    const char *input = "media/Titanic.ts";
    AVFormatContext *pFormatCtx = avformat_alloc_context();
    // 打开文件
    if (avformat_open_input(&pFormatCtx, input, NULL, NULL) != 0)
    {
        std::cout << "open file failed" << std::endl;
        return;
    }
    // 获取流信息
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
    {
        std::cout << "find stream info failed" << std::endl;
        return;
    }

    int videoStreamIndex = -1;

    for (int i = 0; i < pFormatCtx->nb_streams; i++)
    {
        // 找到视频流
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoStreamIndex = i;
            break;
        }
    }
    if (videoStreamIndex < 0)
    {
        std::cout << "find video stream failed" << std::endl;
        return;
    }
    // 编码信息
    AVCodecParameters *pCodecParamters = pFormatCtx->streams[videoStreamIndex]->codecpar;
    const AVCodec *pCodec = avcodec_find_decoder(pCodecParamters->codec_id);
    if (pCodec == nullptr)
    {
        std::cout << "find decoder failed" << std::endl;
        return;
    }
    // 获取解码上下文
    AVCodecContext *pCodecCtx = avcodec_alloc_context3(pCodec);
    if (avcodec_parameters_to_context(pCodecCtx, pCodecParamters) < 0)
    {
        std::cout << "parameters to context failed" << std::endl;
        return;
    }
    // 解码器打开
    if (avcodec_open2(pCodecCtx, pCodec, nullptr) < 0)
    {
        std::cout << "open codec failed" << std::endl;
        return;
    }

    std::thread renderThread(initRenderer, pCodecParamters->width, pCodecParamters->height);

    AVFrame *pFrame = av_frame_alloc();
    AVFrame *pFrameYUV = av_frame_alloc();
    SwsContext *pSwsCtx = sws_getContext(pCodecParamters->width, pCodecParamters->height, pCodecCtx->pix_fmt, pCodecParamters->width, pCodecParamters->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

    AVPacket packet;
    while (true)
    {
        if (av_read_frame(pFormatCtx, &packet) < 0)
        {
            std::cout << "read frame end" << std::endl;
            break;
        }
        if (packet.stream_index == videoStreamIndex)
        {
            //这里最好是发送和解码放不同线程并行处理，以免单线程处理过慢导致渲染的延时
            int ret = avcodec_send_packet(pCodecCtx, &packet);
            if (ret < 0)
            {
                std::cout << "send packet error, ret:" << ret << std::endl;
                continue;
            }

            ret = avcodec_receive_frame(pCodecCtx, pFrame);
            if(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF){
                //已经到文件末尾
                continue;
            }else if(ret < 0){
                //解码出错
                std::cout << "receive frame error, ret:" << ret << std::endl;
                break;
            }

            sws_scale(pSwsCtx, (const uint8_t *const *)pFrame->data, pFrame->linesize, 0, pCodecParamters->height, pFrameYUV->data, pFrameYUV->linesize);
            videoFrameQueue.push(new VedioFrame(pFrame->pts, pCodecCtx->time_base.den, pFrameYUV));


        }
    }
}
