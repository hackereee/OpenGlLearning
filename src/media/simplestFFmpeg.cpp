#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include "simplestFFmpeg.h"
#include <iostream>

void playVideo(){
    const char* input = "media/Titanic.ts";
    AVFormatContext *pFormatCtx = avformat_alloc_context();
    if(avformat_open_input(&pFormatCtx, input, NULL, NULL) != 0){
        std::cout << "open file failed" << std::endl;
        return;
    }

    if(avformat_find_stream_info(pFormatCtx, NULL) < 0){
        std::cout << "find stream info failed" << std::endl;
        return;
    }

    int videoStreamIndex = -1;

}
