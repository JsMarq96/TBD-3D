#ifndef _AUDIO_CONTROLLER_H_
#define _AUDIO_CONTROLLER_H_

#include <bass.h>
#include <unordered_map>
#include <string>
#include <iostream>

#include "framework.h"

struct sAudioController {
    HSTREAM audio;

    sAudioController();

    ~sAudioController();

    sAudioController(std::string id);

    static sAudioController* Get(std::string id);

    static HCHANNEL* play(const char* name);

    static HCHANNEL play_3D(std::string name, Vector3 position);

    static void stop(HCHANNEL chan) {
        BASS_ChannelPause(chan);
        BASS_ChannelStop(chan);
        std::cout << BASS_ErrorGetCode() << std::endl;
    };

    static bool has_finished(HCHANNEL chan) {
        return !BASS_ChannelIsActive(chan);
    }
};

static std::unordered_map<std::string, sAudioController*> loaded_audios = std::unordered_map<std::string, sAudioController*>();

#endif