#ifndef MEDIA_H
#define MEDIA_H
#include <glfw/glfw3.h>
#include <common/gl_common.h>

extern "C"
{
#include <libavcodec/avcodec.h>
}

class MediaContext
{
};

class Video

{

public:
    Video(/* args */);
    ~Video();
};

class Audio
{
public:
    Audio()
    {
    }
    ~Audio()
    {
    }
};

class Media
{


public:
    Media() {};
    ~Media() = default;
    virtual void play() = 0;
};

class AudidoMedia : public Media
{

private:
    AVFormatContext *pFormatContext;
    AVCodecParameters *pCodecParamters;

public:
    AudidoMedia(AVFormatContext *pFormatContext, AVCodecParameters *pCodecParamters);
    ~AudidoMedia();
    void play() override;
};

class VideoMedia : public Media
{

private:
    AVFormatContext *pFormatContext;
    AVCodecParameters *pCodecParamters;

public:
    VideoMedia(AVFormatContext *pFormatContext, AVCodecParameters *pCodecParamters);
    ~VideoMedia();
    void play() override;
};

class MixedMedia : public Media
{
private:
    VideoMedia *videoMedia;
    AudidoMedia *audioMedia;

public:
    MixedMedia(VideoMedia *videoMedia, AudidoMedia *audioMedia);
    ~MixedMedia();
    void play() override;
};

class GLObject
{
public:
    GLObject(int width, int height, const char *title);
    ~GLObject()
    {
        delete window;
    }
    GLFWwindow *window;
    std::shared_ptr<Shader> shader;
    GLuint vao, vbo, ebo;

private:
    void init();
};

void play(const char *mediaPath);

#endif
