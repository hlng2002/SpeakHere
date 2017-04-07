//
//  voice_message_denoise.h
//  voicemessagedenoise
//
//  Created by kerrizhang(张庆) on 2017/4/6.
//  Copyright © 2017年 tencent. All rights reserved.
//

#ifndef voice_message_denoise_h
#define voice_message_denoise_h

class VoiceMessageDenoise{
public:
    VoiceMessageDenoise();
    ~VoiceMessageDenoise();
public:
    /*
     * This function initializes a NS instance
     * Input:
     *      -fs     :sampling frequency (fs only be 8000 or 16000)
     * Return value :0  ok
     *               <0 err
     */
    int Init(int fs);
    /*
     * This function process a frame
     * Input:
     *      -buf     :input 20ms pcm sample buffer (when process done, denoised sample also in this buffer)
     *      -leninbyte:input buf len in byte (fs*20*len(short)/1000)
     * Return value :0  ok
     *               <0 err
     */
    int Process(short* buf, int leninbyte);
    
    /*
     * This function uninit a NS instance
     * Return value :0  ok
     *               <0 err
     */
     int Uninit();
private:
    short* nsout_buf;
    short mode;
    int framelen_inms;
    int fs;
    bool init_flag;
    void *ns_inst;
};


#endif /* voice_message_denoise_h */
