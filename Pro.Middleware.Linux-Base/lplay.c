#include <stdio.h>  
#include <malloc.h>  
#include <unistd.h>  
#include <stdlib.h>  
#include <string.h>  
#include <getopt.h>  
#include <fcntl.h>  
#include <ctype.h>  
#include <errno.h>  
#include <limits.h>  
#include <time.h>  
#include <locale.h>  
#include <sys/unistd.h>  
#include <sys/stat.h>  
#include <sys/types.h>  
#include <alsa/asoundlib.h>  
#include <assert.h>  
#include "wav_parser.h"  
#include "sndwav_common.h"  
  
ssize_t SNDWAV_P_SaveRead(int fd, void *buf, size_t count)  
{  
    ssize_t result = 0, res;  
  
    while (count > 0) {  
        if ((res = read(fd, buf, count)) == 0)  
            break;  
        if (res < 0)  
            return result > 0 ? result : res;  
        count -= res;  
        result += res;  
        buf = (char *)buf + res;  
    }  
    return result;  
}  
  
void SNDWAV_Play(SNDPCMContainer_t *sndpcm, WAVContainer_t *wav, int fd)  
{  
    int load, ret;  
    off64_t written = 0;  
    off64_t c;  
    off64_t count = LE_INT(wav->chunk.length);  
  
    load = 0;  
    while (written < count) {  
        /* Must read [chunk_bytes] bytes data enough. */  
        do {  
            c = count - written;  
            if (c > sndpcm->chunk_bytes)  
                c = sndpcm->chunk_bytes;  
            c -= load; 
  
            if (c == 0)  
                break;  
            ret = SNDWAV_P_SaveRead(fd, sndpcm->data_buf + load, c);  
            if (ret < 0) {  
                fprintf(stderr, "Error safe_read\n");  
                exit(-1);  
            }  
            if (ret == 0)  
                break;  
            load += ret;  
        } while ((size_t)load < sndpcm->chunk_bytes);  
  
        /* Transfer to size frame */  
        load = load * 8 / sndpcm->bits_per_frame;  
        ret = SNDWAV_WritePcm(sndpcm, load);  
        if (ret != load)  
            break;  
          
        ret = ret * sndpcm->bits_per_frame / 8;  
        written += ret;  
        load = 0;  
    }  
}  

void setVolumn() {
    snd_mixer_t *mixerFd;
    snd_mixer_elem_t *elem;
    int result;
    
	// 打开混音器
	 if ((result = snd_mixer_open( &mixerFd, 0)) < 0)
	 {
	 	puts("snd_mixer_open error");
	        mixerFd = NULL;
	 }
	// Attach an HCTL to an opened mixer
	 if ((result = snd_mixer_attach( mixerFd, "default")) < 0)
	 {
	 	puts("snd_mixer_attach error");
	        snd_mixer_close(mixerFd);
	        mixerFd = NULL;
	 }
	// 注册混音器
	 if ((result = snd_mixer_selem_register( mixerFd, NULL, NULL)) < 0)
	{
		puts("snd_mixer_selem_register error");
	        snd_mixer_close(mixerFd);
	        mixerFd = NULL;
	}
	// 加载混音器
	if ((result = snd_mixer_load( mixerFd)) < 0)
	{
		puts("snd_mixer_load error");
	        snd_mixer_close(mixerFd);
	        mixerFd = NULL;
	}
	
 for(elem=snd_mixer_first_elem(mixerFd); elem; elem=snd_mixer_elem_next(elem))
    {
        if (snd_mixer_elem_get_type(elem) == SND_MIXER_ELEM_SIMPLE &&
             snd_mixer_selem_is_active(elem)) // 找到可以用的, 激活的elem
        {
            const char *type=snd_mixer_selem_get_name(elem);
            printf("%s : ",type);
            long minVolume,maxVolume,volume;
            snd_mixer_selem_get_playback_volume_range(elem, &minVolume, &maxVolume);
            snd_mixer_selem_get_playback_volume(elem, 0, &volume);
            printf("[%ld, %ld] = %ld\n",minVolume,maxVolume,volume);
                snd_mixer_selem_set_playback_volume_all(elem, maxVolume); // 设置音量
        }
    }
    sleep(1);
}

int main(int argc, char *argv[])  
{
    //setVolumn();
    char *filename;  
    char *devicename = "default";  
    int fd;  
    WAVContainer_t wav;  
    SNDPCMContainer_t playback;  
      
    if (argc != 2) {  
        fprintf(stderr, "Usage: ./lplay <FILENAME>\n");  
        return -1;  
    }  
      
    memset(&playback, 0x0, sizeof(playback));  
  
    filename = argv[1];  
    fd = open(filename, O_RDONLY);  
    if (fd < 0) {  
        fprintf(stderr, "Error open [%s]\n", filename);  
        return -1;  
    }  
      
    if (WAV_ReadHeader(fd, &wav) < 0) {  
        fprintf(stderr, "Error WAV_Parse [%s]\n", filename);  
        goto Err;  
    }  
  
    if (snd_output_stdio_attach(&playback.log, stderr, 0) < 0) {  
        fprintf(stderr, "Error snd_output_stdio_attach\n");  
        goto Err;  
    }  
  
    if (snd_pcm_open(&playback.handle, devicename, SND_PCM_STREAM_PLAYBACK, 0) < 0) {  
        fprintf(stderr, "Error snd_pcm_open [ %s]\n", devicename);  
        goto Err;  
    }  
  
    if (SNDWAV_SetParams(&playback, &wav) < 0) { 
        fprintf(stderr, "Error set_snd_pcm_params\n");  
        goto Err;  
    }  
    snd_pcm_dump(playback.handle, playback.log);  
  
    SNDWAV_Play(&playback, &wav, fd);  
  
    snd_pcm_drain(playback.handle);  
  
    close(fd);  
    free(playback.data_buf);  
    snd_output_close(playback.log);  
    snd_pcm_close(playback.handle);  
    return 0;  
  
Err:  
    close(fd);  
    if (playback.data_buf) free(playback.data_buf);  
    if (playback.log) snd_output_close(playback.log);  
    if (playback.handle) snd_pcm_close(playback.handle);  
    return -1;  
}