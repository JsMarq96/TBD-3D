#include "audio_controller.h"

sAudioController::sAudioController() {

};

sAudioController::~sAudioController() {
        
};


sAudioController::sAudioController(std::string id) {
    //audio = BASS_SampleLoad( false, id.c_str() ,0,0,3,0 ); 
    audio = BASS_StreamCreateFile(false, id.c_str(), 0, 0, 0);
}

sAudioController* sAudioController::Get(std::string id) {
    if (loaded_audios.find(id) == loaded_audios.end()) {
        loaded_audios[id] = new sAudioController(id);
    }

    return loaded_audios[id];
}

HCHANNEL sAudioController::play_3D(std::string name, Vector3 position) {
    sAudioController *audio_contr = Get(name);
    HCHANNEL channel_audio;

    BASS_3DVECTOR pos( position.x , position.y, position.z );
    BASS_ChannelSet3DPosition(audio_contr->audio, &pos, NULL, NULL );
    BASS_ChannelSet3DAttributes( audio_contr->audio, BASS_3DMODE_NORMAL, 0.0f, 40.f, 360, 360, 1.0f );
    
    BASS_Apply3D();

    channel_audio = BASS_ChannelPlay(audio_contr->audio, FALSE);

    return channel_audio;
}

HCHANNEL* sAudioController::play(const char* name) {

}