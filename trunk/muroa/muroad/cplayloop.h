/***************************************************************************
 *   Copyright (C) 2005 by Martin Runge                                    *
 *   martin.runge@web.de                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef CPLAYLOOP_H
#define CPLAYLOOP_H

/**
  @author Martin Runge
*/


#include <cthreadslave.h>
#include "caudioioalsa.h"
#include "caudiooss.h"
#include "cmutex.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/time_duration.hpp>

#include <list>
#include <string>


class CAudioFrame;
class CSync;
class CPacketRingBuffer;
class CRingBuffer;
class CResampler;
class CPlayer;
class Cmuroad;
class CRTPPacket;

using namespace boost::posix_time;

class CPlayloop : public CThreadSlave
{
public:
  CPlayloop(CPlayer* parent, Cmuroad* config, CPacketRingBuffer* packet_ringbuffer );

    ~CPlayloop();

    void DoLoop();
    void playSilence(int num_frames);

    int sync(void);

    void setSync(CSync* sync_obj);


//    void appendAudioFrame(CAudioFrame* frame);

  
private:



    CPacketRingBuffer *m_packet_ringbuffer;
    CRingBuffer *m_ringbuffer;


    CResampler* m_resampler;
    double m_resample_factor;
    double m_correction_factor;

    //CAudioIoAlsa *m_audio_sink;
    IAudioIO *m_audio_sink;


    void playAudio(CAudioFrame *frame);
    IAudioIO* initSoundSystem();

    void handleSyncObj(CSync* sync_obj);

    ptime* m_start_time;

    uint32_t m_stream_id, m_session_id;

    int m_seqnum;
    int m_counter;

    long m_num_multi_channel_samples_played;
    long m_num_multi_channel_samples_arrived;

    boost::posix_time::time_duration m_average_time_diff;
    int m_average_size;

//    std::list<CAudioFrame*> m_frame_list;


private:
    void adjustResamplingFactor(int bytes_in_playback_ringbuffer);

    int getDelayInMultiChannelSamples();
    boost::posix_time::time_duration calcSoundCardDelay();
    boost::posix_time::time_duration calcResamplerDelay();
    boost::posix_time::time_duration calcRingbufferDelay();

    boost::posix_time::time_duration getPlaybackDiff();
    boost::posix_time::time_duration getPlaybackDiffFromTime();
    
    int sleep(boost::posix_time::time_duration duration);
    int adjustFramesToDiscard(int num_frames_discarded);

    long long m_nr_of_last_frame_decoded;

    int m_desired_sample_rate;
    int m_num_channels;
    int m_sample_size;
    int m_frames_per_second_pre_resampler;
    int m_frames_per_second_post_resampler;

    short *m_silence_buffer;
    int m_frames_to_discard;

    FILE* m_debug_fd1;
    
    CPlayer* m_player;
    Cmuroad* m_config;
    
    int m_secs_idle;

  private:
    bool checkStream(CRTPPacket* packet);
};

#endif
