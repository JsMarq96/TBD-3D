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

    static HCHANNEL play(std::string name);

    static HCHANNEL play_3D(std::string name, Vector3 position);

    static void stop(HCHANNEL chan) {
        BASS_ChannelStop(chan);
    };

    static bool has_finished(HCHANNEL chan) {
        return !BASS_ChannelIsActive(chan);
    }

    static HCHANNEL play_sound_loop(std::string name);
};

static std::unordered_map<std::string, sAudioController*> loaded_audios = std::unordered_map<std::string, sAudioController*>();

#endif