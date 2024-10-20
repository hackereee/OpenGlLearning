#include <libavformat/avformat.h>
#include <mutex>

class VedioFrame{
    public: 
        VedioFrame(){}
        VedioFrame(int pts, int timeBase, AVFrame *frame):pts(pts),timeBase(timeBase){
            this->frame = av_frame_alloc();
            av_frame_ref(this->frame, frame);
        };
        ~VedioFrame(){
            av_frame_free(&frame);
        };
         AVFrame *frame;
    private:
        int pts;
        int timeBase;
       
};

// 双缓冲环形缓冲区
class VideoFrameQueue{
    public:
        VideoFrameQueue(int size):size(size){
            buffer = new VedioFrame*[size];
        };
        ~VideoFrameQueue(){
            delete[] buffer;
            //释放锁
            
        };
        void push(VedioFrame *frame){
            std::unique_lock<std::mutex> lock(mtx);
            // 如果缓冲区满了则释放锁并等待
            notFull.wait(lock, [this](){return (writeIndex + 1) % size != readIndex;});
            buffer[writeIndex] = frame;
            writeIndex = (writeIndex + 1) % size;
        };
        VedioFrame *pop(){
            std::unique_lock<std::mutex> lock(mtx);
            // 如果缓冲区为空则释放锁并等待
            notEmpty.wait(lock, [this](){return writeIndex != readIndex;});
            VedioFrame *frame = buffer[readIndex];
            readIndex = (readIndex + 1) % size;
            return frame;
        };
    private:
        std::mutex mtx;
        std::condition_variable notFull;   
        std::condition_variable notEmpty;
        VedioFrame **buffer;
        int size;
        int readIndex = 0;
        int writeIndex = 0;
};