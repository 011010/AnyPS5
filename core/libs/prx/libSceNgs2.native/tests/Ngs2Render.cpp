#include "prx/libSceNgs2.native/include/Ngs2Types.hpp"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>

extern "C" {
int APS5_VABI sceNgs2SystemResetOption(Ngs2SystemOption*);
int APS5_VABI sceNgs2SystemQueryBufferSize(const Ngs2SystemOption*, Ngs2ContextBufferInfo*);
int APS5_VABI sceNgs2SystemCreate(const Ngs2SystemOption*, const Ngs2ContextBufferInfo*, uintptr_t*);
int APS5_VABI sceNgs2SystemCreateWithAllocator(const Ngs2SystemOption*, const Ngs2BufferAllocator*, uintptr_t*);
int APS5_VABI sceNgs2SystemDestroy(uintptr_t, Ngs2ContextBufferInfo*);
int APS5_VABI sceNgs2SystemGetInfo(uintptr_t, Ngs2SystemInfo*, size_t);
int APS5_VABI sceNgs2SystemSetGrainSamples(uintptr_t, uint32_t);
int APS5_VABI sceNgs2SystemRender(uintptr_t, const Ngs2RenderBufferInfo*, uint32_t);
int APS5_VABI sceNgs2RackQueryBufferSize(uint32_t, const Ngs2RackOption*, Ngs2ContextBufferInfo*);
int APS5_VABI sceNgs2RackCreate(uintptr_t, uint32_t, const Ngs2RackOption*, const Ngs2ContextBufferInfo*, uintptr_t*);
int APS5_VABI sceNgs2RackCreateWithAllocator(uintptr_t, uint32_t, const Ngs2RackOption*, const Ngs2BufferAllocator*, uintptr_t*);
int APS5_VABI sceNgs2RackDestroy(uintptr_t, Ngs2ContextBufferInfo*);
int APS5_VABI sceNgs2RackGetVoiceHandle(uintptr_t, uint32_t, uintptr_t*);
int APS5_VABI sceNgs2VoiceControl(uintptr_t, const Ngs2VoiceParamHeader*);
int APS5_VABI sceNgs2VoiceRunCommands(uintptr_t, const Ngs2VoiceCommand*, size_t);
int APS5_VABI sceNgs2VoiceGetState(uintptr_t, Ngs2VoiceState*, size_t);
int APS5_VABI sceNgs2VoiceGetStateFlags(uintptr_t, uint32_t*);
}

static void Check(bool value, int line) {
    if (!value) {
        std::fprintf(stderr, "NGS2 check failed at line %d\n", line);
        std::abort();
    }
}
#define Require(value) Check((value), __LINE__)

static constexpr std::uint32_t Grain = 8;

static std::vector<std::uint64_t> buffers[16];
static std::size_t usedBuffers = 0;

static Ngs2ContextBufferInfo Buffer(const Ngs2ContextBufferInfo& query) {
    auto& storage = buffers[usedBuffers++];
    storage.resize(query.host_buffer_size / sizeof(std::uint64_t) + 1);
    Ngs2ContextBufferInfo info{};
    info.host_buffer = storage.data();
    info.host_buffer_size = query.host_buffer_size;
    return info;
}

static uintptr_t CreateSystem() {
    Ngs2ContextBufferInfo query{};
    Require(sceNgs2SystemQueryBufferSize(nullptr, &query) == SCE_NGS2_OK && query.host_buffer_size != 0);
    const auto info = Buffer(query);
    uintptr_t system = 0;
    Require(sceNgs2SystemCreate(nullptr, &info, &system) == SCE_NGS2_OK && system != 0);
    Require(sceNgs2SystemSetGrainSamples(system, Grain) == SCE_NGS2_OK);
    return system;
}

static uintptr_t CreateRack(uintptr_t system, std::uint32_t rackId) {
    Ngs2ContextBufferInfo query{};
    Require(sceNgs2RackQueryBufferSize(rackId, nullptr, &query) == SCE_NGS2_OK);
    const auto info = Buffer(query);
    uintptr_t rack = 0;
    Require(sceNgs2RackCreate(system, rackId, nullptr, &info, &rack) == SCE_NGS2_OK && rack != 0);
    return rack;
}

static uintptr_t Voice(uintptr_t rack) {
    uintptr_t voice = 0;
    Require(sceNgs2RackGetVoiceHandle(rack, 0, &voice) == SCE_NGS2_OK && voice != 0);
    return voice;
}

template <typename TParam>
static void Control(uintptr_t voice, std::uint32_t id, TParam param) {
    param.header = {static_cast<std::uint16_t>(sizeof(TParam)), 0, id};
    Require(sceNgs2VoiceControl(voice, &param.header) == SCE_NGS2_OK);
}

static void Event(uintptr_t voice, std::uint32_t eventId) {
    Control(voice, SCE_NGS2_VOICE_PARAM_EVENT, Ngs2VoiceEventParam{{}, eventId});
}

static void Patch(uintptr_t source, uintptr_t dest) {
    Control(source, SCE_NGS2_VOICE_PARAM_PATCH, Ngs2VoicePatchParam{{}, 0, 0, dest});
}

static uintptr_t Mastering(uintptr_t system, std::uint32_t channels) {
    const auto voice = Voice(CreateRack(system, SCE_NGS2_RACK_ID_MASTERING));
    Control(voice, SCE_NGS2_MASTERING_VOICE_PARAM_SETUP, Ngs2MasteringVoiceSetupParam{{}, channels, 0});
    Control(voice, SCE_NGS2_MASTERING_VOICE_PARAM_OUTPUT, Ngs2MasteringVoiceOutputParam{{}, 0, 0});
    Event(voice, SCE_NGS2_VOICE_EVENT_PLAY);
    return voice;
}

static uintptr_t Sampler(uintptr_t system, const std::vector<std::int16_t>& pcm, std::uint32_t repeats) {
    const auto voice = Voice(CreateRack(system, SCE_NGS2_RACK_ID_SAMPLER));
    Control(voice, SCE_NGS2_SAMPLER_VOICE_PARAM_SETUP, Ngs2SamplerVoiceSetupParam{{}, {SCE_NGS2_WAVEFORM_TYPE_PCM_I16L, 1, 48000, 0, 0, 0}});
    const Ngs2WaveformBlock block{0, pcm.size() * sizeof(std::int16_t), repeats, 0, static_cast<std::uint32_t>(pcm.size()), 0, 0x55};
    Control(voice, SCE_NGS2_SAMPLER_VOICE_PARAM_ADD_WAVEFORM_BLOCKS, Ngs2SamplerVoiceWaveformBlocksParam{{}, pcm.data(), 0, 1, &block});
    return voice;
}

static std::vector<std::uint32_t> callbackFlags;
static void APS5_VABI OnBlock(const Ngs2VoiceCallbackInfo* info) {
    Require(info->callback_data == 7 && info->user_data == 0x55);
    callbackFlags.push_back(info->flag);
}

static std::uint32_t Flags(uintptr_t voice) {
    std::uint32_t flags = 0xffffffff;
    Require(sceNgs2VoiceGetStateFlags(voice, &flags) == SCE_NGS2_OK);
    return flags;
}

static std::vector<std::int16_t> RenderI16(uintptr_t system) {
    std::vector<std::int16_t> out(Grain, -1);
    const Ngs2RenderBufferInfo info{out.data(), out.size() * sizeof(std::int16_t), SCE_NGS2_WAVEFORM_TYPE_PCM_I16L, 1};
    Require(sceNgs2SystemRender(system, &info, 1) == SCE_NGS2_OK);
    return out;
}

static void TestErrorsAndInfo() {
    Ngs2SystemOption option{};
    Require(sceNgs2SystemResetOption(&option) == SCE_NGS2_OK);
    Require(option.size == sizeof(option) && option.max_grain_samples == 512 && option.num_grain_samples == 256 && option.sample_rate == 48000);
    Require(sceNgs2SystemQueryBufferSize(&option, nullptr) == SCE_NGS2_ERROR_INVALID_OUT_ADDRESS);
    Require(sceNgs2RackQueryBufferSize(SCE_NGS2_RACK_ID_SAMPLER, nullptr, nullptr) == SCE_NGS2_ERROR_INVALID_OUT_ADDRESS);

    Ngs2SystemInfo info{};
    Require(sceNgs2SystemGetInfo(0x1234, &info, sizeof(info)) == SCE_NGS2_ERROR_INVALID_SYSTEM_HANDLE);
    Require(sceNgs2RackDestroy(0x1234, nullptr) == SCE_NGS2_ERROR_INVALID_RACK_HANDLE);

    const auto system = CreateSystem();
    CreateRack(system, SCE_NGS2_RACK_ID_SAMPLER);
    Require(sceNgs2SystemGetInfo(system, nullptr, sizeof(info)) == SCE_NGS2_ERROR_INVALID_OUT_ADDRESS);
    Require(sceNgs2SystemGetInfo(system, &info, sizeof(info) - 1) == SCE_NGS2_ERROR_INVALID_OUT_SIZE);
    Require(sceNgs2SystemGetInfo(system, &info, sizeof(info)) == SCE_NGS2_OK);
    Require(info.system_handle == system && info.uid != 0 && info.rack_count == 1 && info.sample_rate == 48000);
    Require(info.num_grain_samples == Grain && info.max_grain_samples == 512 && info.render_count == 0);

    Ngs2ContextBufferInfo released{};
    Require(sceNgs2SystemDestroy(system, &released) == SCE_NGS2_OK && released.host_buffer != nullptr);
    Require(sceNgs2SystemGetInfo(system, &info, sizeof(info)) == SCE_NGS2_ERROR_INVALID_SYSTEM_HANDLE);
}

static void TestPcmBlockEnd() {
    const auto system = CreateSystem();
    const auto master = Mastering(system, 1);
    std::vector<std::int16_t> pcm;
    for (int i = 0; i < 12; i++) pcm.push_back(static_cast<std::int16_t>(i * 1000 - 4000));
    const auto sampler = Sampler(system, pcm, 0);
    Patch(sampler, master);
    Control(sampler, SCE_NGS2_VOICE_PARAM_CALLBACK, Ngs2VoiceCallbackParam{{}, OnBlock, 7, SCE_NGS2_VOICE_CALLBACK_FLAG_BLOCK_END, 0});
    Require(Flags(sampler) == 0);
    Event(sampler, SCE_NGS2_VOICE_EVENT_PLAY);
    Require(Flags(sampler) == SCE_NGS2_VOICE_STATE_FLAG_INUSE);

    callbackFlags.clear();
    auto out = RenderI16(system);
    for (std::uint32_t i = 0; i < Grain; i++) Require(out[i] == pcm[i]);
    Require(Flags(sampler) == (SCE_NGS2_VOICE_STATE_FLAG_INUSE | SCE_NGS2_VOICE_STATE_FLAG_PLAYING) && callbackFlags.empty());

    Ngs2SamplerVoiceState state{};
    Require(sceNgs2VoiceGetState(sampler, &state.voice_state, sizeof(state) - 8) == SCE_NGS2_ERROR_INVALID_OUT_SIZE);
    Require(sceNgs2VoiceGetState(sampler, &state.voice_state, sizeof(state)) == SCE_NGS2_OK);
    Require(state.num_decoded_samples == Grain && state.decoded_data_size == Grain * 2 && state.user_data == 0x55);
    Require(state.waveform_data == pcm.data() + Grain);

    out = RenderI16(system);
    for (std::uint32_t i = 0; i < Grain; i++) Require(out[i] == (i < 4 ? pcm[Grain + i] : 0));
    Require(callbackFlags.size() == 1 && callbackFlags[0] == SCE_NGS2_VOICE_CALLBACK_FLAG_BLOCK_END);
    Require(Flags(sampler) == 0);
    Require(sceNgs2VoiceGetState(sampler, &state.voice_state, sizeof(state)) == SCE_NGS2_OK);
    Require(state.num_decoded_samples == pcm.size() && state.waveform_data == pcm.data() + pcm.size());
    Require(sceNgs2SystemDestroy(system, nullptr) == SCE_NGS2_OK);
}

static void TestPitchAndRepeat() {
    const auto system = CreateSystem();
    const auto master = Mastering(system, 1);
    const std::vector<std::int16_t> pcm{0, 1000, 2000, 3000};
    const auto sampler = Sampler(system, pcm, 1);
    Patch(sampler, master);
    Control(sampler, SCE_NGS2_SAMPLER_VOICE_PARAM_PITCH, Ngs2SamplerVoicePitchParam{{}, 0.5f});
    Control(sampler, SCE_NGS2_VOICE_PARAM_CALLBACK,
            Ngs2VoiceCallbackParam{{}, OnBlock, 7, SCE_NGS2_VOICE_CALLBACK_FLAG_BLOCK_END | SCE_NGS2_VOICE_CALLBACK_FLAG_BLOCK_REPEAT, 0});
    Event(sampler, SCE_NGS2_VOICE_EVENT_PLAY);

    callbackFlags.clear();
    const std::int16_t first[Grain] = {0, 500, 1000, 1500, 2000, 2500, 3000, 1500};
    auto out = RenderI16(system);
    for (std::uint32_t i = 0; i < Grain; i++) Require(out[i] == first[i]);
    Require(callbackFlags.size() == 1 && callbackFlags[0] == SCE_NGS2_VOICE_CALLBACK_FLAG_BLOCK_REPEAT);

    const std::int16_t second[Grain] = {0, 500, 1000, 1500, 2000, 2500, 3000, 3000};
    out = RenderI16(system);
    for (std::uint32_t i = 0; i < Grain; i++) Require(out[i] == second[i]);
    Require(callbackFlags.size() == 2 && callbackFlags[1] == SCE_NGS2_VOICE_CALLBACK_FLAG_BLOCK_END);
    Require(Flags(sampler) == 0);
    Require(sceNgs2SystemDestroy(system, nullptr) == SCE_NGS2_OK);
}

static void TestSubmixerMatrix() {
    const auto system = CreateSystem();
    const auto master = Mastering(system, 2);
    const auto submixer = Voice(CreateRack(system, SCE_NGS2_RACK_ID_SUBMIXER));
    Control(submixer, SCE_NGS2_SUBMIXER_VOICE_PARAM_SETUP, Ngs2SubmixerVoiceSetupParam{{}, 2, 0});
    Patch(submixer, master);
    const Ngs2VoiceCommand play{2, 0, 4, 0, {.u = SCE_NGS2_VOICE_EVENT_PLAY}};
    Require(sceNgs2VoiceRunCommands(submixer, &play, 1) == SCE_NGS2_OK);

    const std::vector<std::int16_t> pcm(Grain, 16384);
    const auto sampler = Sampler(system, pcm, 0);
    Patch(sampler, submixer);
    const float levels[2] = {1.0f, 0.5f};
    Control(sampler, SCE_NGS2_VOICE_PARAM_MATRIX_LEVELS, Ngs2VoiceMatrixLevelsParam{{}, 0, 2, levels});
    Control(sampler, SCE_NGS2_VOICE_PARAM_PORT_MATRIX, Ngs2VoicePortMatrixParam{{}, 0, 0});
    Control(sampler, SCE_NGS2_VOICE_PARAM_PORT_VOLUME, Ngs2VoicePortVolumeParam{{}, 0, 0.5f});
    Require(sceNgs2VoiceRunCommands(sampler, &play, 1) == SCE_NGS2_OK);

    std::vector<float> out(Grain * 2, -1.0f);
    const Ngs2RenderBufferInfo info{out.data(), out.size() * sizeof(float), SCE_NGS2_WAVEFORM_TYPE_PCM_F32L, 2};
    Require(sceNgs2SystemRender(system, &info, 1) == SCE_NGS2_OK);
    for (std::uint32_t i = 0; i < Grain; i++) Require(out[i * 2] == 0.25f && out[i * 2 + 1] == 0.125f);

    Ngs2SubmixerVoiceState state{};
    Require(sceNgs2VoiceGetState(submixer, &state.voice_state, sizeof(state)) == SCE_NGS2_OK);
    Require(state.voice_state.state_flags == (SCE_NGS2_VOICE_STATE_FLAG_INUSE | SCE_NGS2_VOICE_STATE_FLAG_PLAYING));

    Require(sceNgs2SystemRender(system, &info, 1) == SCE_NGS2_OK);
    for (float sample : out) Require(sample == 0.0f);
    Require(sceNgs2SystemDestroy(system, nullptr) == SCE_NGS2_OK);
}

static int allocations = 0;
static std::int32_t APS5_VABI Allocate(Ngs2ContextBufferInfo* info) {
    Require(info->host_buffer == nullptr && info->host_buffer_size != 0 && info->user_data == 9);
    info->host_buffer = std::calloc(1, info->host_buffer_size);
    allocations++;
    return SCE_NGS2_OK;
}
static std::int32_t APS5_VABI Release(Ngs2ContextBufferInfo* info) {
    Require(info->host_buffer != nullptr && info->user_data == 9);
    std::free(info->host_buffer);
    allocations--;
    return SCE_NGS2_OK;
}

static void TestAllocator() {
    const Ngs2BufferAllocator allocator{Allocate, Release, 9};
    uintptr_t system = 0;
    Require(sceNgs2SystemCreateWithAllocator(nullptr, &allocator, &system) == SCE_NGS2_OK && allocations == 1);
    uintptr_t sampler = 0;
    uintptr_t master = 0;
    Require(sceNgs2RackCreateWithAllocator(system, SCE_NGS2_RACK_ID_SAMPLER, nullptr, &allocator, &sampler) == SCE_NGS2_OK);
    Require(sceNgs2RackCreateWithAllocator(system, SCE_NGS2_RACK_ID_MASTERING, nullptr, &allocator, &master) == SCE_NGS2_OK && allocations == 3);
    Ngs2ContextBufferInfo released{};
    Require(sceNgs2RackDestroy(sampler, &released) == SCE_NGS2_OK && allocations == 2 && released.host_buffer == nullptr);
    Require(sceNgs2SystemDestroy(system, nullptr) == SCE_NGS2_OK && allocations == 0);
    Require(sceNgs2RackDestroy(master, nullptr) == SCE_NGS2_ERROR_INVALID_RACK_HANDLE);
}

int main() {
    TestErrorsAndInfo();
    TestPcmBlockEnd();
    TestPitchAndRepeat();
    TestSubmixerMatrix();
    TestAllocator();
    return 0;
}
