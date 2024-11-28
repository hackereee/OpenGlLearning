#include <media.h>
#include <common/gl_common.h>
#include <iostream>
#include <Program/shader.h>
#include <glfw/glfw3.h>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/time.h>
}

void initSDL();

GLObject *createGLObject()
{
    return new GLObject(800, 600, "DDYPlayer");
}

/// @brief 顶点及纹理坐标
const float vertices[] = {
    // 顶点坐标          // 纹理坐标
    -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 0.0f, 1.0f, 0.0f};

// 使用元素缓冲对象
const int indices[] = {
    0, 1, 2,
    2, 3, 1};

GLObject *glObject;
Media *media;

GLObject::GLObject(int width, int height, const char *title)
{
    window = initGlEnv(800, 600, "Titanic");
    shader = std::make_shared<Shader>("shaders/media/media.vert", "shaders/media/media.frag");
    shader->use();
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 创建纹理
    GLuint texture[3];
    glGenTextures(3, texture);
    // Y
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    std::string textureY = "textureY";
    shader->setInt(textureY, 0);
    // 设置环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);     // x轴
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);     // y轴
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 缩小
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 放大
    // U
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    std::string textureU = "textureU";
    shader->setInt(textureU, 1);
    // 设置环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);     // x轴
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);     // y轴
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 缩小
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 放大
    // V
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    std::string textureV = "textureV";
    shader->setInt(textureV, 2);
    // 设置环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);     // x轴
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);     // y轴
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 缩小
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 放大

    // 开启多级渐远纹理
    // glGenerateMipmap(GL_TEXTURE_2D);
    // 开启垂直同步
    glfwSwapInterval(1);
}

AudidoMedia::AudidoMedia(AVFormatContext *pFormatContext, AVCodecParameters *pCodecParamters) : pFormatContext(pFormatContext), pCodecParamters(pCodecParamters)
{
}

VideoMedia::VideoMedia(AVFormatContext *pFormatContext, AVCodecParameters *pCodecParamters) : pFormatContext(pFormatContext), pCodecParamters(pCodecParamters)
{
}

MixedMedia::MixedMedia(VideoMedia *videoMedia, AudidoMedia *audioMedia) : videoMedia(videoMedia), audioMedia(audioMedia)
{
}


AudidoMedia::~AudidoMedia()
{
    avcodec_parameters_free(&pCodecParamters);
    avformat_free_context(pFormatContext);   
}

VideoMedia::~VideoMedia()
{
    avcodec_parameters_free(&pCodecParamters);
    avformat_free_context(pFormatContext);
}

MixedMedia::~MixedMedia()
{
    delete videoMedia;
    delete audioMedia;
}

void logInfo(const char *msg)
{
    std::clog << msg << std::endl;
}

void logError(const char *msg)
{
    std::cerr << msg << std::endl;
}


void initMedia(const char *path)
{
    if(!path){
        logError("path is null");
        return;
    }
    AVFormatContext *pFormatCtx = avformat_alloc_context();
    if(avformat_open_input(&pFormatCtx, path, NULL, NULL) < 0){
        logError("open file failed");
        return;
    }
    if(avformat_find_stream_info(pFormatCtx, NULL) < 0){
        logError("find stream info failed");
        return;
    }
    int videoStreamIndex = -1;
    int audioStreamIndex = -1;
    for(int i = 0; i < pFormatCtx->nb_streams; i++){
        if(pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){
            videoStreamIndex = i;
        }else if(pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO){
            audioStreamIndex = i;
        }
    }
    if(videoStreamIndex < 0 && audioStreamIndex < 0){
        logError("find video stream failed");
        return;
    }
    AudidoMedia* audioMedia = nullptr;
    VideoMedia* videoMedia = nullptr;
    if(audioStreamIndex >= 0){
        AVCodecParameters *pCodecParamters = pFormatCtx->streams[audioStreamIndex]->codecpar;
        audioMedia = new AudidoMedia(pFormatCtx, pCodecParamters);
    }
    if(videoStreamIndex >= 0){
        AVCodecParameters *pCodecParamters = pFormatCtx->streams[videoStreamIndex]->codecpar;
        videoMedia = new VideoMedia(pFormatCtx, pCodecParamters);
    }

    if(audioMedia && videoMedia){
        media = new MixedMedia( videoMedia, audioMedia);
    }else if(audioMedia){
        media = audioMedia;
    }else if(videoMedia){
        media = videoMedia;
    }else {
        logError("media is null");
        return;
    }
    

}

void printPicture()
{
    if (!glObject)
    {
        glObject = createGLObject();
    }
}



void playSound()
{
}

void playMedia(const char *mediaPath)
{
}
