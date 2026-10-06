#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <numbers>
#include <vector>

#include "prx/libc/include/General.hpp"
#include "Ngs2Internal.hpp"

static constexpr std::array<std::uint32_t, 8> COMB_TUNING = {1116, 1188, 1277, 1356, 1422, 1491, 1557, 1617};
static constexpr std::array<std::uint32_t, 4> ALLPASS_TUNING = {556, 441, 341, 225};
static constexpr std::array<float, 4> EARLY_TAPS = {1.0f, 1.31f, 1.73f, 2.19f};
static constexpr std::array<float, 4> EARLY_GAINS = {0.6f, 0.45f, 0.35f, 0.25f};
static constexpr std::uint32_t STEREO_SPREAD = 23;
static constexpr float TUNING_RATE = 44100.0f;
static constexpr float INPUT_GAIN = 0.015f;
static constexpr float OUTPUT_GAIN = 3.0f;
static constexpr float MAX_REFLECTIONS_DELAY = 0.3f;
static constexpr float MAX_REVERB_DELAY = 0.1f;
static constexpr float SILENCE = 1e-20f;

static float Flush(float value) {
    return std::fabs(value) < SILENCE ? 0.0f : value;
}

struct Ngs2DelayLine {
    std::vector<float> buffer;
    std::size_t cursor = 0;

    float Tap(std::size_t delay) const {
        delay = std::min(delay, buffer.size() - 1);
        return buffer[(cursor + buffer.size() - delay) % buffer.size()];
    }
    void Push(float value) {
        cursor = (cursor + 1) % buffer.size();
        buffer[cursor] = value;
    }
};

struct Ngs2Comb {
    std::vector<float> buffer;
    std::size_t cursor = 0;
    float feedback = 0.0f;
    float damp = 0.0f;
    float store = 0.0f;

    float Process(float input) {
        const float output = buffer[cursor];
        store = Flush(output * (1.0f - damp) + store * damp);
        buffer[cursor] = Flush(input + store * feedback);
        cursor = (cursor + 1) % buffer.size();
        return output;
    }
};

struct Ngs2Allpass {
    std::vector<float> buffer;
    std::size_t cursor = 0;

    float Process(float input, float feedback) {
        const float delayed = buffer[cursor];
        buffer[cursor] = Flush(input + delayed * feedback);
        cursor = (cursor + 1) % buffer.size();
        return delayed - input;
    }
};

struct Ngs2ReverbChannel {
    float roomLowpass = 0.0f;
    Ngs2DelayLine preDelay;
    std::array<Ngs2Comb, COMB_TUNING.size()> combs;
    std::array<Ngs2Allpass, ALLPASS_TUNING.size()> allpasses;
};

struct Ngs2ReverbState {
    Ngs2ReverbI3DL2Param params{};
    bool configured = false;
    std::uint32_t sampleRate = 0;
    std::uint32_t channels = 0;
    float dry = 0.0f;
    float wet = 0.0f;
    float roomHf = 1.0f;
    float roomCoefficient = 0.0f;
    float earlyGain = 0.0f;
    float lateGain = 0.0f;
    float allpassFeedback = 0.5f;
    std::size_t earlyDelay = 0;
    std::size_t lateDelay = 0;
    std::vector<Ngs2ReverbChannel> lines;
};

void Ngs2ReverbDeleter::operator()(Ngs2ReverbState* reverb) const {
    delete reverb;
}

static float MillibelGain(std::int32_t millibels) {
    return std::pow(10.0f, static_cast<float>(millibels) / 2000.0f);
}

static std::size_t ScaledLength(std::uint32_t tuning, std::uint32_t spread, float rate) {
    return std::max<std::size_t>(1, static_cast<std::size_t>((tuning + spread) * rate / TUNING_RATE));
}

static void Allocate(Ngs2ReverbState& reverb, std::uint32_t sampleRate, std::uint32_t channels) {
    const float rate = static_cast<float>(sampleRate);
    const auto maxDelay = static_cast<std::size_t>(std::ceil(std::max(MAX_REFLECTIONS_DELAY * EARLY_TAPS.back(), MAX_REFLECTIONS_DELAY + MAX_REVERB_DELAY) * rate));
    reverb.sampleRate = sampleRate;
    reverb.channels = channels;
    reverb.lines.assign(channels, {});
    for (std::uint32_t channel = 0; channel < channels; channel++) {
        auto& line = reverb.lines[channel];
        line.preDelay.buffer.assign(maxDelay + 1, 0.0f);
        const std::uint32_t spread = channel * STEREO_SPREAD;
        for (std::size_t i = 0; i < COMB_TUNING.size(); i++) line.combs[i].buffer.assign(ScaledLength(COMB_TUNING[i], spread, rate), 0.0f);
        for (std::size_t i = 0; i < ALLPASS_TUNING.size(); i++) line.allpasses[i].buffer.assign(ScaledLength(ALLPASS_TUNING[i], spread, rate), 0.0f);
    }
}

static void UpdateCoefficients(Ngs2ReverbState& reverb) {
    const auto& params = reverb.params;
    const float rate = static_cast<float>(reverb.sampleRate);
    const float hfReference = std::min(params.hf_reference, rate * 0.45f);
    reverb.dry = params.dry;
    reverb.wet = params.wet * MillibelGain(params.room);
    reverb.roomHf = MillibelGain(params.room_hf);
    reverb.roomCoefficient = 1.0f - std::exp(-2.0f * std::numbers::pi_v<float> * hfReference / rate);
    reverb.earlyGain = MillibelGain(params.reflections);
    reverb.lateGain = MillibelGain(params.reverb);
    reverb.allpassFeedback = 0.5f * params.diffusion / 100.0f;
    reverb.earlyDelay = static_cast<std::size_t>(params.reflections_delay * rate);
    reverb.lateDelay = static_cast<std::size_t>((params.reflections_delay + params.reverb_delay) * rate);
    for (auto& line : reverb.lines) {
        for (auto& comb : line.combs) {
            const float seconds = static_cast<float>(comb.buffer.size()) / rate;
            const float low = std::pow(10.0f, -3.0f * seconds / params.decay_time);
            const float high = std::pow(10.0f, -3.0f * seconds / (params.decay_time * params.decay_hf_ratio));
            const float ratio = std::min(high / low, 1.0f);
            comb.feedback = low;
            comb.damp = (1.0f - ratio) / (1.0f + ratio);
        }
    }
}

static bool InRange(float value, float low, float high) {
    return std::isfinite(value) && value >= low && value <= high;
}

void Ngs2SetReverbParams(Ngs2Voice& voice, const Ngs2ReverbI3DL2Param& params) {
    if (!std::isfinite(params.wet) || !std::isfinite(params.dry) || params.room < -10000 || params.room > 0 || params.room_hf < -10000 || params.room_hf > 0 ||
        !InRange(params.decay_time, 0.1f, 20.0f) || !InRange(params.decay_hf_ratio, 0.1f, 2.0f) || params.reflections < -10000 || params.reflections > 1000 ||
        !InRange(params.reflections_delay, 0.0f, MAX_REFLECTIONS_DELAY) || params.reverb < -10000 || params.reverb > 2000 ||
        !InRange(params.reverb_delay, 0.0f, MAX_REVERB_DELAY) || !InRange(params.diffusion, 0.0f, 100.0f) || !InRange(params.density, 0.0f, 100.0f) ||
        !InRange(params.hf_reference, 20.0f, 20000.0f)) {
        APS5_INVALID_ARG_EX;
    }
    if (!voice.reverb) voice.reverb.reset(new Ngs2ReverbState{});
    voice.reverb->params = params;
    voice.reverb->configured = false;
}

void Ngs2ClearReverb(Ngs2Voice& voice) {
    if (voice.reverb) voice.reverb->sampleRate = 0;
}

bool Ngs2ProcessReverb(Ngs2Voice& voice, std::uint32_t grain, std::uint32_t sampleRate) {
    auto& reverb = *voice.reverb;
    if (reverb.sampleRate != sampleRate || reverb.channels != voice.channels) {
        Allocate(reverb, sampleRate, voice.channels);
        reverb.configured = false;
    }
    if (!reverb.configured) {
        UpdateCoefficients(reverb);
        reverb.configured = true;
    }
    bool audible = false;
    for (std::uint32_t channel = 0; channel < voice.channels; channel++) {
        auto& line = reverb.lines[channel];
        float* samples = voice.samples.data() + static_cast<std::size_t>(channel) * grain;
        for (std::uint32_t i = 0; i < grain; i++) {
            const float input = samples[i];
            line.roomLowpass = Flush(line.roomLowpass + reverb.roomCoefficient * (input - line.roomLowpass));
            line.preDelay.Push(line.roomLowpass + reverb.roomHf * (input - line.roomLowpass));
            float early = 0.0f;
            if (reverb.earlyDelay != 0) {
                for (std::size_t tap = 0; tap < EARLY_TAPS.size(); tap++) {
                    early += line.preDelay.Tap(static_cast<std::size_t>(reverb.earlyDelay * EARLY_TAPS[tap])) * EARLY_GAINS[tap];
                }
            }
            const float lateInput = line.preDelay.Tap(reverb.lateDelay) * INPUT_GAIN;
            float late = 0.0f;
            for (auto& comb : line.combs) late += comb.Process(lateInput);
            for (auto& allpass : line.allpasses) late = allpass.Process(late, reverb.allpassFeedback);
            samples[i] = Flush(reverb.dry * input + reverb.wet * (reverb.earlyGain * early + reverb.lateGain * late * OUTPUT_GAIN));
            audible = audible || samples[i] != 0.0f;
        }
    }
    return audible;
}
