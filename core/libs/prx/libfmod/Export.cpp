#include <atomic>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>
#include "SceTypes.hpp"
#include "prx/libc/include/General.hpp"

enum FMOD_OUTPUTTYPE {
    FMOD_OUTPUTTYPE_AUTODETECT = 0,
    FMOD_OUTPUTTYPE_UNKNOWN = 1,
    FMOD_OUTPUTTYPE_NOSOUND = 2,
    FMOD_OUTPUTTYPE_WAVWRITER = 3,
    FMOD_OUTPUTTYPE_NOSOUND_NRT = 4,
    FMOD_OUTPUTTYPE_WAVWRITER_NRT = 5,
    FMOD_OUTPUTTYPE_WASAPI = 6,
    FMOD_OUTPUTTYPE_ASIO = 7,
    FMOD_OUTPUTTYPE_PULSEAUDIO = 8,
    FMOD_OUTPUTTYPE_ALSA = 9,
    FMOD_OUTPUTTYPE_COREAUDIO = 10,
    FMOD_OUTPUTTYPE_AUDIOTRACK = 11,
    FMOD_OUTPUTTYPE_OPENSL = 12,
    FMOD_OUTPUTTYPE_AUDIOOUT = 13,
    FMOD_OUTPUTTYPE_AUDIO3D = 14,
    FMOD_OUTPUTTYPE_MAX = 15
};

enum FMOD_SPEAKERMODE {
    FMOD_SPEAKERMODE_DEFAULT = 0,
    FMOD_SPEAKERMODE_RAW = 1,
    FMOD_SPEAKERMODE_MONO = 2,
    FMOD_SPEAKERMODE_STEREO = 3,
    FMOD_SPEAKERMODE_QUAD = 4,
    FMOD_SPEAKERMODE_SURROUND = 5,
    FMOD_SPEAKERMODE_5POINT1 = 6,
    FMOD_SPEAKERMODE_7POINT1 = 7,
    FMOD_SPEAKERMODE_7POINT1POINT4 = 8,
    FMOD_SPEAKERMODE_MAX = 9
};

enum FMOD_SOUND_TYPE {
    FMOD_SOUND_TYPE_UNKNOWN = 0,
    FMOD_SOUND_TYPE_AIFF = 1,
    FMOD_SOUND_TYPE_ASF = 2,
    FMOD_SOUND_TYPE_DLS = 3,
    FMOD_SOUND_TYPE_FLAC = 4,
    FMOD_SOUND_TYPE_FSB = 5,
    FMOD_SOUND_TYPE_IT = 6,
    FMOD_SOUND_TYPE_MIDI = 7,
    FMOD_SOUND_TYPE_MOD = 8,
    FMOD_SOUND_TYPE_MPEG = 9,
    FMOD_SOUND_TYPE_OGGVORBIS = 10,
    FMOD_SOUND_TYPE_PLAYLIST = 11,
    FMOD_SOUND_TYPE_RAW = 12,
    FMOD_SOUND_TYPE_S3M = 13,
    FMOD_SOUND_TYPE_USER = 14,
    FMOD_SOUND_TYPE_XM = 15,
    FMOD_SOUND_TYPE_XMA = 16,
    FMOD_SOUND_TYPE_AUDIOQUEUE = 17,
    FMOD_SOUND_TYPE_AT9 = 18,
    FMOD_SOUND_TYPE_VB = 19,
    FMOD_SOUND_TYPE_MAX = 20
};

enum FMOD_SOUND_FORMAT {
    FMOD_SOUND_FORMAT_NONE = 0,
    FMOD_SOUND_FORMAT_PCM8 = 1,
    FMOD_SOUND_FORMAT_PCM16 = 2,
    FMOD_SOUND_FORMAT_PCM24 = 3,
    FMOD_SOUND_FORMAT_PCM32 = 4,
    FMOD_SOUND_FORMAT_PCMFLOAT = 5,
    FMOD_SOUND_FORMAT_BITSTREAM = 6,
    FMOD_SOUND_FORMAT_MAX = 7
};

enum FMOD_OPENSTATE {
    FMOD_OPENSTATE_READY = 0,
    FMOD_OPENSTATE_LOADING = 1,
    FMOD_OPENSTATE_ERROR = 2,
    FMOD_OPENSTATE_CONNECTING = 3,
    FMOD_OPENSTATE_BUFFERING = 4,
    FMOD_OPENSTATE_SEEKING = 5,
    FMOD_OPENSTATE_PLAYING = 6,
    FMOD_OPENSTATE_SETPOSITION = 7,
    FMOD_OPENSTATE_MAX = 8
};

enum FMOD_SOUNDGROUP_BEHAVIOR {
    FMOD_SOUNDGROUP_BEHAVIOR_FAIL = 0,
    FMOD_SOUNDGROUP_BEHAVIOR_MUTE = 1,
    FMOD_SOUNDGROUP_BEHAVIOR_STEALLOWEST = 2,
    FMOD_SOUNDGROUP_BEHAVIOR_MAX = 3
};

enum FMOD_DSP_TYPE {
    FMOD_DSP_TYPE_UNKNOWN = 0,
    FMOD_DSP_TYPE_MIXER = 1,
    FMOD_DSP_TYPE_OSCILLATOR = 2,
    FMOD_DSP_TYPE_LOWPASS = 3,
    FMOD_DSP_TYPE_ITLOWPASS = 4,
    FMOD_DSP_TYPE_HIGHPASS = 5,
    FMOD_DSP_TYPE_ECHO = 6,
    FMOD_DSP_TYPE_FADER = 7,
    FMOD_DSP_TYPE_FLANGE = 8,
    FMOD_DSP_TYPE_DISTORTION = 9,
    FMOD_DSP_TYPE_NORMALIZE = 10,
    FMOD_DSP_TYPE_LIMITER = 11,
    FMOD_DSP_TYPE_PARAMEQ = 12,
    FMOD_DSP_TYPE_PITCHSHIFT = 13,
    FMOD_DSP_TYPE_CHORUS = 14,
    FMOD_DSP_TYPE_VSTPLUGIN = 15,
    FMOD_DSP_TYPE_WINAMPPLUGIN = 16,
    FMOD_DSP_TYPE_ITECHO = 17,
    FMOD_DSP_TYPE_COMPRESSOR = 18,
    FMOD_DSP_TYPE_SFXREVERB = 19,
    FMOD_DSP_TYPE_LOWPASS_SIMPLE = 20,
    FMOD_DSP_TYPE_DELAY = 21,
    FMOD_DSP_TYPE_TREMOLO = 22,
    FMOD_DSP_TYPE_SEND = 24,
    FMOD_DSP_TYPE_RETURN = 25,
    FMOD_DSP_TYPE_HIGHPASS_SIMPLE = 26,
    FMOD_DSP_TYPE_PAN = 27,
    FMOD_DSP_TYPE_THREE_EQ = 28,
    FMOD_DSP_TYPE_FFT = 29,
    FMOD_DSP_TYPE_LOUDNESS_METER = 30,
    FMOD_DSP_TYPE_ENVELOPEFOLLOWER = 31,
    FMOD_DSP_TYPE_CONVOLUTIONREVERB = 32,
    FMOD_DSP_TYPE_TRANSCEIVER = 33,
    FMOD_DSP_TYPE_MAX = 34
};

enum FMOD_DSPCONNECTION_TYPE {
    FMOD_DSPCONNECTION_TYPE_STANDARD = 0,
    FMOD_DSPCONNECTION_TYPE_SIDECHAIN = 1,
    FMOD_DSPCONNECTION_TYPE_SEND = 2,
    FMOD_DSPCONNECTION_TYPE_SEND_SIDECHAIN = 3,
    FMOD_DSPCONNECTION_TYPE_MAX = 4
};

enum FMOD_DSP_FFT_WINDOW {
    FMOD_DSP_FFT_WINDOW_RECT = 0,
    FMOD_DSP_FFT_WINDOW_TRIANGLE = 1,
    FMOD_DSP_FFT_WINDOW_HAMMING = 2,
    FMOD_DSP_FFT_WINDOW_HANNING = 3,
    FMOD_DSP_FFT_WINDOW_BLACKMAN = 4,
    FMOD_DSP_FFT_WINDOW_BLACKMANHARRIS = 5,
    FMOD_DSP_FFT_WINDOW_MAX = 6
};

enum FMOD_PLUGINTYPE {
    FMOD_PLUGINTYPE_OUTPUT = 0,
    FMOD_PLUGINTYPE_CODEC = 1,
    FMOD_PLUGINTYPE_DSP = 2,
    FMOD_PLUGINTYPE_MAX = 3
};

enum FMOD_DRIVER_STATE {
    FMOD_DRIVER_STATE_CONNECTED = 1,
    FMOD_DRIVER_STATE_DEFAULT = 2
};

enum FMOD_PORT_TYPE {
    FMOD_PORT_TYPE_MUSIC = 0,
    FMOD_PORT_TYPE_COPY = 1,
    FMOD_PORT_TYPE_SPLIT = 2,
    FMOD_PORT_TYPE_FEEDBACK = 3,
    FMOD_PORT_TYPE_MAX = 4
};

enum FMOD_SYSTEM_CALLBACK_TYPE {
    FMOD_SYSTEM_CALLBACK_DEVICELISTCHANGED = 1,
    FMOD_SYSTEM_CALLBACK_DEVICELOST = 2,
    FMOD_SYSTEM_CALLBACK_MEMORYALLOCATIONFAILED = 4,
    FMOD_SYSTEM_CALLBACK_THREADCREATED = 8,
    FMOD_SYSTEM_CALLBACK_BADDSPCONNECTION = 16,
    FMOD_SYSTEM_CALLBACK_PREMIX = 32,
    FMOD_SYSTEM_CALLBACK_POSTMIX = 64,
    FMOD_SYSTEM_CALLBACK_ERROR = 128,
    FMOD_SYSTEM_CALLBACK_MIDMIX = 256,
    FMOD_SYSTEM_CALLBACK_THREADDESTROYED = 512,
    FMOD_SYSTEM_CALLBACK_PREUPDATE = 1024,
    FMOD_SYSTEM_CALLBACK_POSTUPDATE = 2048,
    FMOD_SYSTEM_CALLBACK_ALL = 0xFFFFFFFF
};

enum FMOD_CHANNELCONTROL_TYPE {
    FMOD_CHANNELCONTROL_CHANNEL = 0,
    FMOD_CHANNELCONTROL_CHANNELGROUP = 1,
    FMOD_CHANNELCONTROL_MAX = 2
};

enum FMOD_CHANNELCONTROL_CALLBACK_TYPE {
    FMOD_CHANNELCONTROL_CALLBACK_END = 0,
    FMOD_CHANNELCONTROL_CALLBACK_VIRTUALVOICE = 1,
    FMOD_CHANNELCONTROL_CALLBACK_SYNCPOINT = 2,
    FMOD_CHANNELCONTROL_CALLBACK_OCCLUSION = 3,
    FMOD_CHANNELCONTROL_CALLBACK_MAX = 4
};

enum FMOD_TAGTYPE {
    FMOD_TAGTYPE_UNKNOWN = 0,
    FMOD_TAGTYPE_ID3V1 = 1,
    FMOD_TAGTYPE_ID3V2 = 2,
    FMOD_TAGTYPE_VORBISCOMMENT = 3,
    FMOD_TAGTYPE_SHOUTCAST = 4,
    FMOD_TAGTYPE_ICECAST = 5,
    FMOD_TAGTYPE_ASF = 6,
    FMOD_TAGTYPE_MIDI = 7,
    FMOD_TAGTYPE_PLAYLIST = 8,
    FMOD_TAGTYPE_FMOD = 9,
    FMOD_TAGTYPE_USER = 10,
    FMOD_TAGTYPE_MAX = 11
};

enum FMOD_TAGDATATYPE {
    FMOD_TAGDATATYPE_BINARY = 0,
    FMOD_TAGDATATYPE_INT = 1,
    FMOD_TAGDATATYPE_FLOAT = 2,
    FMOD_TAGDATATYPE_STRING = 3,
    FMOD_TAGDATATYPE_STRING_UTF16 = 4,
    FMOD_TAGDATATYPE_STRING_UTF16BE = 5,
    FMOD_TAGDATATYPE_STRING_UTF8 = 6,
    FMOD_TAGDATATYPE_CDTOC = 7,
    FMOD_TAGDATATYPE_MAX = 8
};

struct FMOD_GUID {
    unsigned Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char Data4[8];
};

struct FMOD_VECTOR {
    float x;
    float y;
    float z;
};

struct FMOD_3D_ATTRIBUTES {
    FMOD_VECTOR position;
    FMOD_VECTOR velocity;
    FMOD_VECTOR forward;
    FMOD_VECTOR up;
};

struct FMOD_CREATESOUNDEXINFO {
    int cbsize;
    unsigned length;
    unsigned fileoffset;
    int numchannels;
    int defaultfrequency;
    FMOD_SOUND_FORMAT format;
    unsigned decodebuffersize;
    int initialsubsound;
    int numsubsounds;
    void* inclusionlist;
    unsigned inclusionlistnum;
    unsigned char dlsrawdata[8];
    char* encryptionkey;
};

struct FMOD_REVERB_PROPERTIES {
    int Instance;
    float Environment;
    float EnvSize;
    float EnvDiffusion;
    int Room;
    int RoomHF;
    int RoomLF;
    float DecayTime;
    float DecayHFRatio;
    float DecayLFRatio;
    int Reflections;
    float ReflectionsDelay;
    int Reverb;
    float ReverbDelay;
    float EchoTime;
    float EchoDepth;
    float ModulationTime;
    float ModulationDepth;
    float AirAbsorptionHF;
    float HFReference;
    float LFReference;
    float RoomRolloffFactor;
    float Diffusion;
    float Density;
    unsigned Flags;
};

struct FMOD_ADVANCEDSETTINGS {
    int cbSize;
    unsigned maxMPEGCodecs;
    unsigned maxADPCMCodecs;
    unsigned maxXMACodecs;
    unsigned maxVorbisCodecs;
    unsigned maxAT9Codecs;
    unsigned maxFADPCMCodecs;
    unsigned maxPCMCodecs;
    unsigned ASIONumChannels;
    char* ASIOChannelList;
    char* ASIOSpeakerList;
    float vol0virtualvol;
    unsigned defaultDecodeBufferSize;
    unsigned short profilePort;
    unsigned geometryMaxFadeTime;
    float distanceFilterCenterFreq;
    int reverb3Dinstance;
    unsigned DSPBufferPoolSize;
    unsigned stackSizeStream;
    unsigned stackSizeNonBlocking;
    unsigned stackSizeMixer;
    int resamplerMethod;
    unsigned commandQueueSize;
    unsigned handleInitialSize;
};

struct FMOD_CPU_USAGE {
    float dsp;
    float stream;
    float geometry;
    float update;
    float convolution1;
    float convolution2;
};

struct FMOD_TAG {
    FMOD_TAGTYPE type;
    FMOD_TAGDATATYPE datatype;
    char* name;
    void* data;
    unsigned datalen;
    bool updated;
};

struct FMOD_SYNC_POINT {
    void* opaque;
};

struct FMOD_DSP_DESCRIPTION {
    unsigned pluginsdkversion;
    char name[32];
    unsigned version;
    int numinputbuffers;
    int numoutputbuffers;
    void* create;
    void* release;
    void* reset;
    void* read;
    void* process;
    void* setposition;
    int numparameters;
    void* paramdesc;
    void* setparameterfloat;
    void* setparameterint;
    void* setparameterbool;
    void* setparameterdata;
    void* getparameterfloat;
    void* getparameterint;
    void* getparameterbool;
    void* getparameterdata;
    void* shouldiprocess;
    void* userdata;
    void* sys_register;
    void* sys_deregister;
    void* sys_mix;
};

struct FMOD_DSP_PARAMETER_DESC {
    int type;
    char name[16];
    char label[16];
    char* description;
};

struct FMOD_DSP_METERING_INFO {
    int numsamples;
    float peaklevel[32];
    float rmslevel[32];
    short numchannels;
};

struct FMOD_DSP_BUFFER_ARRAY {
    int numbuffers;
    int* buffernumchannels;
    int* bufferchannelmask;
    float** buffers;
    FMOD_SPEAKERMODE speakermode;
};

struct FMOD_DSP_STATE {
    void* instance;
    void* plugindata;
    int channelmask;
    FMOD_SPEAKERMODE source_speakermode;
    float* sidechaindata;
    int sidechainchannels;
    void* callbacks;
    int systemobject;
};

struct FMOD_SYSTEM {
    int unused;
};

struct FMOD_CHANNELCONTROL {
    int unused;
};

struct FMOD_CHANNELGROUP {
    int unused;
};

typedef int (*FMOD_SYSTEM_CALLBACK)(FMOD_SYSTEM* system, FMOD_SYSTEM_CALLBACK_TYPE type, void* commanddata1, void* commanddata2, void* userdata);
typedef int (*FMOD_CHANNELCONTROL_CALLBACK)(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype, void* commanddata1, void* commanddata2);

namespace {

std::mutex gFmodMutex;

constexpr int FMOD_OK = 0;
constexpr unsigned FMOD_VERSION_CURRENT = 0x00020206;

}

namespace FMOD {

class System;
class ChannelGroup;
class Channel;
class Sound;
class SoundGroup;
class DSP;
class DSPConnection;
class Geometry;
class Reverb3D;

class Sound {
public:
    std::string name;
    unsigned mode = 0;
    unsigned length = 0;
    int loopCount = 0;
    float minDistance = 0.0f;
    float maxDistance = 10000.0f;
    SoundGroup* soundGroup = nullptr;
    void* userData = nullptr;
    int APS5_VABI release() {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        delete this;
        return FMOD_OK;
    }
    int APS5_VABI getSystemObject(System** system);
    int APS5_VABI setMode(unsigned mode) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        this->mode = mode;
        return FMOD_OK;
    }
    int APS5_VABI getMode(unsigned* mode) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (mode) *mode = this->mode;
        return FMOD_OK;
    }
    int APS5_VABI set3DMinMaxDistance(float mindistance, float maxdistance) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        minDistance = mindistance;
        maxDistance = maxdistance;
        return FMOD_OK;
    }
    int APS5_VABI getNumSubSounds(int* numsubsounds) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (numsubsounds) *numsubsounds = 0;
        return FMOD_OK;
    }
    int APS5_VABI getLength(unsigned* length, unsigned lengthtype) {
        (void)lengthtype;
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (length) *length = this->length;
        return FMOD_OK;
    }
    int APS5_VABI getFormat(FMOD_SOUND_TYPE* type, FMOD_SOUND_FORMAT* format, int* channels, int* bits) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (type) *type = FMOD_SOUND_TYPE_UNKNOWN;
        if (format) *format = FMOD_SOUND_FORMAT_PCM16;
        if (channels) *channels = 2;
        if (bits) *bits = 16;
        return FMOD_OK;
    }
    int APS5_VABI getName(char* name, int namelen) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (name && namelen > 0) {
            std::strncpy(name, this->name.c_str(), static_cast<std::size_t>(namelen - 1));
            name[namelen - 1] = '\0';
        }
        return FMOD_OK;
    }
    int APS5_VABI setLoopCount(int loopcount) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        loopCount = loopcount;
        return FMOD_OK;
    }
    int APS5_VABI getLoopCount(int* loopcount) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (loopcount) *loopcount = loopCount;
        return FMOD_OK;
    }
    int APS5_VABI setSoundGroup(SoundGroup* soundgroup) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        soundGroup = soundgroup;
        return FMOD_OK;
    }
    int APS5_VABI getSoundGroup(SoundGroup** soundgroup) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (soundgroup) *soundgroup = soundGroup;
        return FMOD_OK;
    }
    int APS5_VABI getSubSound(int index, Sound** subsound) {
        (void)index;
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (subsound) *subsound = nullptr;
        return FMOD_OK;
    }
    int APS5_VABI setUserData(void* userdata) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        userData = userdata;
        return FMOD_OK;
    }
    int APS5_VABI getUserData(void** userdata) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (userdata) *userdata = userData;
        return FMOD_OK;
    }
    int APS5_VABI getOpenState(FMOD_OPENSTATE* openstate, unsigned* percentbuffered, bool* starving, bool* diskbusy) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (openstate) *openstate = FMOD_OPENSTATE_READY;
        if (percentbuffered) *percentbuffered = 100;
        if (starving) *starving = false;
        if (diskbusy) *diskbusy = false;
        return FMOD_OK;
    }
    int APS5_VABI readData(void* buffer, unsigned length, unsigned* read) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (buffer && length) std::memset(buffer, 0, length);
        if (read) *read = length;
        return FMOD_OK;
    }
    int APS5_VABI seekData(unsigned pcm) {
        (void)pcm;
        return FMOD_OK;
    }
};

class SoundGroup {
public:
    int maxAudible = 0;
    float volume = 1.0f;
    void* userData = nullptr;
    int APS5_VABI release() {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        delete this;
        return FMOD_OK;
    }
    int APS5_VABI setMaxAudible(int maxaudible) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        maxAudible = maxaudible;
        return FMOD_OK;
    }
    int APS5_VABI getMaxAudible(int* maxaudible) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (maxaudible) *maxaudible = maxAudible;
        return FMOD_OK;
    }
    int APS5_VABI setVolume(float volume) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        this->volume = volume;
        return FMOD_OK;
    }
    int APS5_VABI getVolume(float* volume) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (volume) *volume = this->volume;
        return FMOD_OK;
    }
    int APS5_VABI stop() { return FMOD_OK; }
    int APS5_VABI setUserData(void* userdata) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        userData = userdata;
        return FMOD_OK;
    }
    int APS5_VABI getUserData(void** userdata) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (userdata) *userdata = userData;
        return FMOD_OK;
    }
    int APS5_VABI getNumSounds(int* numsounds) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (numsounds) *numsounds = 0;
        return FMOD_OK;
    }
};

class DSPConnection {
public:
    DSP* input = nullptr;
    DSP* output = nullptr;
    void* userData = nullptr;
    int APS5_VABI getInput(DSP** input) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (input) *input = this->input;
        return FMOD_OK;
    }
    int APS5_VABI getOutput(DSP** output) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (output) *output = this->output;
        return FMOD_OK;
    }
    int APS5_VABI getType(FMOD_DSPCONNECTION_TYPE* type) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (type) *type = FMOD_DSPCONNECTION_TYPE_STANDARD;
        return FMOD_OK;
    }
    int APS5_VABI setUserData(void* userdata) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        userData = userdata;
        return FMOD_OK;
    }
    int APS5_VABI getUserData(void** userdata) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (userdata) *userdata = userData;
        return FMOD_OK;
    }
};

class DSP {
public:
    FMOD_DSP_TYPE type = FMOD_DSP_TYPE_UNKNOWN;
    bool active = true;
    bool bypass = false;
    void* userData = nullptr;
    int APS5_VABI release() {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        delete this;
        return FMOD_OK;
    }
    int APS5_VABI setActive(bool active) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        this->active = active;
        return FMOD_OK;
    }
    int APS5_VABI getActive(bool* active) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (active) *active = this->active;
        return FMOD_OK;
    }
    int APS5_VABI setBypass(bool bypass) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        this->bypass = bypass;
        return FMOD_OK;
    }
    int APS5_VABI getBypass(bool* bypass) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (bypass) *bypass = this->bypass;
        return FMOD_OK;
    }
    int APS5_VABI reset() { return FMOD_OK; }
    int APS5_VABI setParameterFloat(int index, float value) {
        (void)index; (void)value;
        return FMOD_OK;
    }
    int APS5_VABI getParameterFloat(int index, float* value) {
        (void)index;
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (value) *value = 0.0f;
        return FMOD_OK;
    }
    int APS5_VABI getType(FMOD_DSP_TYPE* type) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (type) *type = this->type;
        return FMOD_OK;
    }
    int APS5_VABI getInfo(char* name, unsigned* version, int* channels, int* configwidth, int* configheight) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (name) name[0] = '\0';
        if (version) *version = 0;
        if (channels) *channels = 0;
        if (configwidth) *configwidth = 0;
        if (configheight) *configheight = 0;
        return FMOD_OK;
    }
    int APS5_VABI setUserData(void* userdata) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        userData = userdata;
        return FMOD_OK;
    }
    int APS5_VABI getUserData(void** userdata) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (userdata) *userdata = userData;
        return FMOD_OK;
    }
};

class ChannelControl {
public:
    float volume = 1.0f;
    float pitch = 1.0f;
    bool paused = false;
    bool mute = false;
    bool playing = false;
    unsigned mode = 0;
    bool volumeRamp = false;
    void* userData = nullptr;
    int APS5_VABI setVolume(float volume) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        this->volume = volume;
        return FMOD_OK;
    }
    int APS5_VABI getVolume(float* volume) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (volume) *volume = this->volume;
        return FMOD_OK;
    }
    int APS5_VABI setPitch(float pitch) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        this->pitch = pitch;
        return FMOD_OK;
    }
    int APS5_VABI getPitch(float* pitch) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (pitch) *pitch = this->pitch;
        return FMOD_OK;
    }
    int APS5_VABI setPaused(bool paused) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        this->paused = paused;
        return FMOD_OK;
    }
    int APS5_VABI getPaused(bool* paused) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (paused) *paused = this->paused;
        return FMOD_OK;
    }
    int APS5_VABI setMute(bool mute) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        this->mute = mute;
        return FMOD_OK;
    }
    int APS5_VABI getMute(bool* mute) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (mute) *mute = this->mute;
        return FMOD_OK;
    }
    int APS5_VABI stop() {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        playing = false;
        return FMOD_OK;
    }
    int APS5_VABI isPlaying(bool* isplaying) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (isplaying) *isplaying = playing && !paused;
        return FMOD_OK;
    }
    int APS5_VABI setMode(unsigned mode) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        this->mode = mode;
        return FMOD_OK;
    }
    int APS5_VABI getMode(unsigned* mode) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (mode) *mode = this->mode;
        return FMOD_OK;
    }
    int APS5_VABI addFadePoint(unsigned long long dspclock, float volume) {
        (void)dspclock; (void)volume;
        return FMOD_OK;
    }
    int APS5_VABI setDelay(unsigned long long dspclock_start, unsigned long long dspclock_end, bool stopchannels) {
        (void)dspclock_start; (void)dspclock_end; (void)stopchannels;
        return FMOD_OK;
    }
    int APS5_VABI getDelay(unsigned long long* dspclock_start, unsigned long long* dspclock_end, bool* stopchannels) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (dspclock_start) *dspclock_start = 0;
        if (dspclock_end) *dspclock_end = 0;
        if (stopchannels) *stopchannels = false;
        return FMOD_OK;
    }
    int APS5_VABI setVolumeRamp(bool ramp) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        volumeRamp = ramp;
        return FMOD_OK;
    }
    int APS5_VABI getVolumeRamp(bool* ramp) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (ramp) *ramp = volumeRamp;
        return FMOD_OK;
    }
    int APS5_VABI setCallback(FMOD_CHANNELCONTROL_CALLBACK callback) {
        (void)callback;
        return FMOD_OK;
    }
    int APS5_VABI isVirtual(bool* isvirtual) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (isvirtual) *isvirtual = false;
        return FMOD_OK;
    }
    int APS5_VABI getIndex(int* index) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (index) *index = 0;
        return FMOD_OK;
    }
    int APS5_VABI getSystemObject(System** system);
    int APS5_VABI setUserData(void* userdata) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        userData = userdata;
        return FMOD_OK;
    }
    int APS5_VABI getUserData(void** userdata) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (userdata) *userdata = userData;
        return FMOD_OK;
    }
    int APS5_VABI set3DAttributes(const FMOD_VECTOR* pos, const FMOD_VECTOR* vel) {
        (void)pos; (void)vel;
        return FMOD_OK;
    }
    int APS5_VABI setLowPassGain(float gain) {
        (void)gain;
        return FMOD_OK;
    }
    int APS5_VABI getLowPassGain(float* gain) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (gain) *gain = 1.0f;
        return FMOD_OK;
    }
    int APS5_VABI setPan(float pan) {
        (void)pan;
        return FMOD_OK;
    }
    int APS5_VABI getDSPClock(unsigned long long* dspclock, unsigned long long* parentclock) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (dspclock) *dspclock = 0;
        if (parentclock) *parentclock = 0;
        return FMOD_OK;
    }
    int APS5_VABI getDSP(int index, DSP** dsp) {
        (void)index;
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (dsp) *dsp = nullptr;
        return FMOD_OK;
    }
};

class Channel : public ChannelControl {
public:
    Sound* sound = nullptr;
    ChannelGroup* group = nullptr;
    float frequency = 48000.0f;
    int priority = 128;
    unsigned position = 0;
    int loopCount = 0;
    int APS5_VABI setPriority(int priority) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        this->priority = priority;
        return FMOD_OK;
    }
    int APS5_VABI getPriority(int* priority) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (priority) *priority = this->priority;
        return FMOD_OK;
    }
    int APS5_VABI setFrequency(float frequency) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        this->frequency = frequency;
        return FMOD_OK;
    }
    int APS5_VABI getFrequency(float* frequency) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (frequency) *frequency = this->frequency;
        return FMOD_OK;
    }
    int APS5_VABI setPosition(unsigned position, unsigned postype) {
        (void)postype;
        std::lock_guard<std::mutex> lock(gFmodMutex);
        this->position = position;
        return FMOD_OK;
    }
    int APS5_VABI getPosition(unsigned* position, unsigned postype) {
        (void)postype;
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (position) *position = this->position;
        return FMOD_OK;
    }
    int APS5_VABI setChannelGroup(ChannelGroup* channelgroup) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        group = channelgroup;
        return FMOD_OK;
    }
    int APS5_VABI getChannelGroup(ChannelGroup** channelgroup) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (channelgroup) *channelgroup = group;
        return FMOD_OK;
    }
    int APS5_VABI setLoopCount(int loopcount) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        loopCount = loopcount;
        return FMOD_OK;
    }
    int APS5_VABI getCurrentSound(Sound** sound) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (sound) *sound = this->sound;
        return FMOD_OK;
    }
};

class ChannelGroup : public ChannelControl {
public:
    std::string name;
    int APS5_VABI release() {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        delete this;
        return FMOD_OK;
    }
    int APS5_VABI addGroup(ChannelGroup* group, bool propagatedspclock, DSPConnection** connection) {
        (void)group; (void)propagatedspclock;
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (connection) *connection = nullptr;
        return FMOD_OK;
    }
    int APS5_VABI getNumGroups(int* numgroups) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (numgroups) *numgroups = 0;
        return FMOD_OK;
    }
    int APS5_VABI getGroup(int index, ChannelGroup** group) {
        (void)index;
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (group) *group = nullptr;
        return FMOD_OK;
    }
    int APS5_VABI getParentGroup(ChannelGroup** group) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (group) *group = nullptr;
        return FMOD_OK;
    }
    int APS5_VABI getName(char* name, int namelen) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (name && namelen > 0) {
            std::strncpy(name, this->name.c_str(), static_cast<std::size_t>(namelen - 1));
            name[namelen - 1] = '\0';
        }
        return FMOD_OK;
    }
    int APS5_VABI getNumChannels(int* numchannels) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (numchannels) *numchannels = 0;
        return FMOD_OK;
    }
    int APS5_VABI getChannel(int index, Channel** channel) {
        (void)index;
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (channel) *channel = nullptr;
        return FMOD_OK;
    }
};

class Geometry {
public:
    void* userData = nullptr;
    int APS5_VABI release() {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        delete this;
        return FMOD_OK;
    }
    int APS5_VABI addPolygon(float directocclusion, float reverbocclusion, bool doublesided, int numvertices, const FMOD_VECTOR* vertices, int* polygonindex) {
        (void)directocclusion; (void)reverbocclusion; (void)doublesided; (void)numvertices; (void)vertices;
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (polygonindex) *polygonindex = 0;
        return FMOD_OK;
    }
    int APS5_VABI setActive(bool active) {
        (void)active;
        return FMOD_OK;
    }
    int APS5_VABI setUserData(void* userdata) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        userData = userdata;
        return FMOD_OK;
    }
    int APS5_VABI getUserData(void** userdata) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (userdata) *userdata = userData;
        return FMOD_OK;
    }
};

class Reverb3D {
public:
    void* userData = nullptr;
    int APS5_VABI release() {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        delete this;
        return FMOD_OK;
    }
    int APS5_VABI set3DAttributes(const FMOD_VECTOR* pos, float mindistance, float maxdistance) {
        (void)pos; (void)mindistance; (void)maxdistance;
        return FMOD_OK;
    }
    int APS5_VABI setActive(bool active) {
        (void)active;
        return FMOD_OK;
    }
    int APS5_VABI setUserData(void* userdata) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        userData = userdata;
        return FMOD_OK;
    }
    int APS5_VABI getUserData(void** userdata) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (userdata) *userdata = userData;
        return FMOD_OK;
    }
};

class System {
public:
    bool initialized = false;
    int maxChannels = 0;
    unsigned initFlags = 0;
    int driver = 0;
    FMOD_OUTPUTTYPE output = FMOD_OUTPUTTYPE_AUTODETECT;
    int softwareChannels = 0;
    int numListeners = 1;
    float dopplerScale = 1.0f;
    float distanceFactor = 1.0f;
    float rolloffScale = 1.0f;
    void* userData = nullptr;
    ChannelGroup* masterChannelGroup = nullptr;
    SoundGroup* masterSoundGroup = nullptr;
    static int APS5_VABI create(System** system) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (!system) return FMOD_OK;
        *system = new System();
        return FMOD_OK;
    }
    static int APS5_VABI create(System** system, unsigned headerversion) {
        (void)headerversion;
        return create(system);
    }
    int APS5_VABI release() {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        delete masterChannelGroup;
        delete masterSoundGroup;
        masterChannelGroup = nullptr;
        masterSoundGroup = nullptr;
        delete this;
        return FMOD_OK;
    }
    int APS5_VABI init(int maxchannels, unsigned flags, void* extradriverdata) {
        (void)extradriverdata;
        std::lock_guard<std::mutex> lock(gFmodMutex);
        maxChannels = maxchannels;
        initFlags = flags;
        initialized = true;
        if (!masterChannelGroup) masterChannelGroup = new ChannelGroup();
        if (!masterSoundGroup) masterSoundGroup = new SoundGroup();
        return FMOD_OK;
    }
    int APS5_VABI close() {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        initialized = false;
        return FMOD_OK;
    }
    int APS5_VABI update() { return FMOD_OK; }
    int APS5_VABI setOutput(FMOD_OUTPUTTYPE output) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        this->output = output;
        return FMOD_OK;
    }
    int APS5_VABI getOutput(FMOD_OUTPUTTYPE* output) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (output) *output = this->output;
        return FMOD_OK;
    }
    int APS5_VABI getNumDrivers(int* numdrivers) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (numdrivers) *numdrivers = 1;
        return FMOD_OK;
    }
    int APS5_VABI getDriverInfo(int id, char* name, int namelen, FMOD_GUID* guid, int* systemrate, FMOD_SPEAKERMODE* speakermode, int* speakermodechannels) {
        (void)id;
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (name && namelen > 0) {
            const char* driverName = "AnyPS5 Null Output";
            std::strncpy(name, driverName, static_cast<std::size_t>(namelen - 1));
            name[namelen - 1] = '\0';
        }
        if (guid) std::memset(guid, 0, sizeof(*guid));
        if (systemrate) *systemrate = 48000;
        if (speakermode) *speakermode = FMOD_SPEAKERMODE_STEREO;
        if (speakermodechannels) *speakermodechannels = 2;
        return FMOD_OK;
    }
    int APS5_VABI setDriver(int driver) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        this->driver = driver;
        return FMOD_OK;
    }
    int APS5_VABI getDriver(int* driver) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (driver) *driver = this->driver;
        return FMOD_OK;
    }
    int APS5_VABI setSoftwareChannels(int numsoftwarechannels) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        softwareChannels = numsoftwarechannels;
        return FMOD_OK;
    }
    int APS5_VABI getSoftwareChannels(int* numsoftwarechannels) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (numsoftwarechannels) *numsoftwarechannels = softwareChannels;
        return FMOD_OK;
    }
    int APS5_VABI setSoftwareFormat(int samplerate, FMOD_SPEAKERMODE speakermode, int numrawspeakers) {
        (void)samplerate; (void)speakermode; (void)numrawspeakers;
        return FMOD_OK;
    }
    int APS5_VABI getSoftwareFormat(int* samplerate, FMOD_SPEAKERMODE* speakermode, int* numrawspeakers) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (samplerate) *samplerate = 48000;
        if (speakermode) *speakermode = FMOD_SPEAKERMODE_STEREO;
        if (numrawspeakers) *numrawspeakers = 0;
        return FMOD_OK;
    }
    int APS5_VABI setDSPBufferSize(unsigned bufferlength, int numbuffers) {
        (void)bufferlength; (void)numbuffers;
        return FMOD_OK;
    }
    int APS5_VABI getDSPBufferSize(unsigned* bufferlength, int* numbuffers) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (bufferlength) *bufferlength = 1024;
        if (numbuffers) *numbuffers = 4;
        return FMOD_OK;
    }
    int APS5_VABI setAdvancedSettings(FMOD_ADVANCEDSETTINGS* settings) {
        (void)settings;
        return FMOD_OK;
    }
    int APS5_VABI getAdvancedSettings(FMOD_ADVANCEDSETTINGS* settings) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (settings) std::memset(settings, 0, sizeof(*settings));
        return FMOD_OK;
    }
    int APS5_VABI setCallback(FMOD_SYSTEM_CALLBACK callback, FMOD_SYSTEM_CALLBACK_TYPE callbackmask, void* userdata) {
        (void)callback; (void)callbackmask; (void)userdata;
        return FMOD_OK;
    }
    int APS5_VABI setPluginPath(const char* pluginpath) {
        (void)pluginpath;
        return FMOD_OK;
    }
    int APS5_VABI loadPlugin(const char* filename, unsigned* handle, FMOD_PLUGINTYPE plugintype) {
        (void)filename; (void)plugintype;
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (handle) *handle = 0;
        return FMOD_OK;
    }
    int APS5_VABI getVersion(unsigned* version) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (version) *version = FMOD_VERSION_CURRENT;
        return FMOD_OK;
    }
    int APS5_VABI getOutputHandle(void** handle) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (handle) *handle = nullptr;
        return FMOD_OK;
    }
    int APS5_VABI getChannelsPlaying(int* channels, int* realchannels) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (channels) *channels = 0;
        if (realchannels) *realchannels = 0;
        return FMOD_OK;
    }
    int APS5_VABI getCPUUsage(FMOD_CPU_USAGE* usage) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (usage) std::memset(usage, 0, sizeof(*usage));
        return FMOD_OK;
    }
    int APS5_VABI getFileUsage(long long* samplebytesread, long long* streambytesread, long long* otherbytesread) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (samplebytesread) *samplebytesread = 0;
        if (streambytesread) *streambytesread = 0;
        if (otherbytesread) *otherbytesread = 0;
        return FMOD_OK;
    }
    int APS5_VABI getSpectrum(float* spectrumarray, int numvalues, int channeloffset, FMOD_DSP_FFT_WINDOW window) {
        (void)channeloffset; (void)window;
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (spectrumarray && numvalues > 0) std::memset(spectrumarray, 0, sizeof(float) * static_cast<std::size_t>(numvalues));
        return FMOD_OK;
    }
    int APS5_VABI getWaveData(float* wavearray, int numvalues, int channeloffset) {
        (void)channeloffset;
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (wavearray && numvalues > 0) std::memset(wavearray, 0, sizeof(float) * static_cast<std::size_t>(numvalues));
        return FMOD_OK;
    }
    int APS5_VABI createSound(const char* name_or_data, unsigned mode, FMOD_CREATESOUNDEXINFO* exinfo, Sound** sound) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (!sound) return FMOD_OK;
        Sound* created = new Sound();
        if (name_or_data) created->name = name_or_data;
        created->mode = mode;
        if (exinfo) created->length = exinfo->length;
        *sound = created;
        return FMOD_OK;
    }
    int APS5_VABI createStream(const char* name_or_data, unsigned mode, FMOD_CREATESOUNDEXINFO* exinfo, Sound** sound) {
        return createSound(name_or_data, mode, exinfo, sound);
    }
    int APS5_VABI createDSP(FMOD_DSP_DESCRIPTION* description, DSP** dsp) {
        (void)description;
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (dsp) *dsp = new DSP();
        return FMOD_OK;
    }
    int APS5_VABI createDSPByType(FMOD_DSP_TYPE type, DSP** dsp) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (dsp) {
            *dsp = new DSP();
            (*dsp)->type = type;
        }
        return FMOD_OK;
    }
    int APS5_VABI playSound(Sound* sound, ChannelGroup* channelgroup, bool paused, Channel** channel) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (!channel) return FMOD_OK;
        Channel* created = new Channel();
        created->sound = sound;
        created->group = channelgroup ? channelgroup : masterChannelGroup;
        created->paused = paused;
        created->playing = true;
        *channel = created;
        return FMOD_OK;
    }
    int APS5_VABI playDSP(DSP* dsp, ChannelGroup* channelgroup, bool paused, Channel** channel) {
        (void)dsp;
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (!channel) return FMOD_OK;
        Channel* created = new Channel();
        created->group = channelgroup ? channelgroup : masterChannelGroup;
        created->paused = paused;
        created->playing = true;
        *channel = created;
        return FMOD_OK;
    }
    int APS5_VABI getChannel(int channelid, Channel** channel) {
        (void)channelid;
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (channel) *channel = nullptr;
        return FMOD_OK;
    }
    int APS5_VABI getMasterSoundGroup(SoundGroup** soundgroup) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (soundgroup) *soundgroup = masterSoundGroup;
        return FMOD_OK;
    }
    int APS5_VABI getMasterChannelGroup(ChannelGroup** channelgroup) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (channelgroup) *channelgroup = masterChannelGroup;
        return FMOD_OK;
    }
    int APS5_VABI setReverbProperties(int instance, FMOD_REVERB_PROPERTIES* prop) {
        (void)instance; (void)prop;
        return FMOD_OK;
    }
    int APS5_VABI getReverbProperties(int instance, FMOD_REVERB_PROPERTIES* prop) {
        (void)instance;
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (prop) std::memset(prop, 0, sizeof(*prop));
        return FMOD_OK;
    }
    int APS5_VABI createReverb3D(Reverb3D** reverb) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (reverb) *reverb = new Reverb3D();
        return FMOD_OK;
    }
    int APS5_VABI createGeometry(int maxpolygons, int maxvertices, Geometry** geometry) {
        (void)maxpolygons; (void)maxvertices;
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (geometry) *geometry = new Geometry();
        return FMOD_OK;
    }
    int APS5_VABI loadGeometry(const void* data, int datasize, Geometry** geometry) {
        (void)data; (void)datasize;
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (geometry) *geometry = new Geometry();
        return FMOD_OK;
    }
    int APS5_VABI setGeometrySettings(float maxworldsize) {
        (void)maxworldsize;
        return FMOD_OK;
    }
    int APS5_VABI getGeometrySettings(float* maxworldsize) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (maxworldsize) *maxworldsize = 0.0f;
        return FMOD_OK;
    }
    int APS5_VABI set3DNumListeners(int numlisteners) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        numListeners = numlisteners;
        return FMOD_OK;
    }
    int APS5_VABI get3DNumListeners(int* numlisteners) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (numlisteners) *numlisteners = numListeners;
        return FMOD_OK;
    }
    int APS5_VABI set3DListenerAttributes(int listener, const FMOD_VECTOR* pos, const FMOD_VECTOR* vel, const FMOD_VECTOR* forward, const FMOD_VECTOR* up) {
        (void)listener; (void)pos; (void)vel; (void)forward; (void)up;
        return FMOD_OK;
    }
    int APS5_VABI mixerSuspend() { return FMOD_OK; }
    int APS5_VABI mixerResume() { return FMOD_OK; }
    int APS5_VABI set3DSettings(float dopplerscale, float distancefactor, float rolloffscale) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        dopplerScale = dopplerscale;
        distanceFactor = distancefactor;
        rolloffScale = rolloffscale;
        return FMOD_OK;
    }
    int APS5_VABI get3DSettings(float* dopplerscale, float* distancefactor, float* rolloffscale) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (dopplerscale) *dopplerscale = dopplerScale;
        if (distancefactor) *distancefactor = distanceFactor;
        if (rolloffscale) *rolloffscale = rolloffScale;
        return FMOD_OK;
    }
    int APS5_VABI createChannelGroup(const char* name, ChannelGroup** channelgroup) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (!channelgroup) return FMOD_OK;
        *channelgroup = new ChannelGroup();
        if (name) (*channelgroup)->name = name;
        return FMOD_OK;
    }
    int APS5_VABI createSoundGroup(SoundGroup** soundgroup) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (soundgroup) *soundgroup = new SoundGroup();
        return FMOD_OK;
    }
    int APS5_VABI lockDSP() { return FMOD_OK; }
    int APS5_VABI unlockDSP() { return FMOD_OK; }
    int APS5_VABI getSoundRAM(int* currentalloced, int* maxalloced, int* total) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (currentalloced) *currentalloced = 0;
        if (maxalloced) *maxalloced = 0;
        if (total) *total = 0;
        return FMOD_OK;
    }
    int APS5_VABI setUserData(void* userdata) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        userData = userdata;
        return FMOD_OK;
    }
    int APS5_VABI getUserData(void** userdata) {
        std::lock_guard<std::mutex> lock(gFmodMutex);
        if (userdata) *userdata = userData;
        return FMOD_OK;
    }
    int APS5_VABI setStreamBufferSize(unsigned filebuffersize, unsigned filebuffersizetype) {
        (void)filebuffersize; (void)filebuffersizetype;
        return FMOD_OK;
    }
    int APS5_VABI attachChannelGroupToPort(FMOD_PORT_TYPE porttype, unsigned long long portindex, ChannelGroup* channelgroup) {
        (void)porttype; (void)portindex; (void)channelgroup;
        return FMOD_OK;
    }
    int APS5_VABI detachChannelGroupFromPort(ChannelGroup* channelgroup) {
        (void)channelgroup;
        return FMOD_OK;
    }
    int APS5_VABI setNetworkProxy(const char* proxy) {
        (void)proxy;
        return FMOD_OK;
    }
};

int APS5_VABI Sound::getSystemObject(System** system) {
    std::lock_guard<std::mutex> lock(gFmodMutex);
    if (system) *system = nullptr;
    return FMOD_OK;
}

int APS5_VABI ChannelControl::getSystemObject(System** system) {
    std::lock_guard<std::mutex> lock(gFmodMutex);
    if (system) *system = nullptr;
    return FMOD_OK;
}

}

extern "C" {

int APS5_VABI FMOD_System_Create(FMOD_SYSTEM** system) {
    std::lock_guard<std::mutex> lock(gFmodMutex);
    if (!system) return 0;
    *system = reinterpret_cast<FMOD_SYSTEM*>(new FMOD::System());
    return 0;
}

int APS5_VABI FMOD_Memory_Initialize(void* poolmem, int poollen, void* useralloc, void* userrealloc, void* userfree, unsigned memtypeflags) {
    (void)poolmem; (void)poollen; (void)useralloc; (void)userrealloc; (void)userfree; (void)memtypeflags;
    return 0;
}

int APS5_VABI FMOD_Memory_GetStats(int* currentalloced, int* maxalloced, int blocking) {
    (void)blocking;
    std::lock_guard<std::mutex> lock(gFmodMutex);
    if (currentalloced) *currentalloced = 0;
    if (maxalloced) *maxalloced = 0;
    return 0;
}

namespace {

__attribute__((used)) int fmod_nid_stub_00() { APS5_LOG_ERR("%s", "VS1Vg5yOLH0"); return 0; }
__attribute__((used)) int fmod_nid_stub_01() { APS5_LOG_ERR("%s", "WAU72lnwYic"); return 0; }

}

APS5_EXPORT("VS1Vg5yOLH0", fmod_nid_stub_00);
APS5_EXPORT("WAU72lnwYic", fmod_nid_stub_01);

}
