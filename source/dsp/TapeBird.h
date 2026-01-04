#pragma once

#include "../StandardIncludes.h"
#include <juce_dsp/juce_dsp.h>
class TapeBirdMono
{
public:
    TapeBirdMono() = default;
    ~TapeBirdMono() = default;
    void prepare (double sampleRate);
    float processSample (float x);
    float processSat (float x);
    void setProcessAmount (float processAmount);
    void setBrightnessOption (BrightnessOptions option);
    void setModeOption (ModeOptions option);

    void reset();

    float getLinearGainValue();

private:
    void update();

    BrightnessOptions m_brightness_option = Brightness_Gold;
    ModeOptions m_mode_option = ModeOptions_Radiant;

    int m_sat_type;

    bool m_g0;

    float m_sr_scale = 1.f;

    float m_a3, m_f1, m_p20, m_p24, m_lpf_k, m_hpf_k, m_auto_gain_a1, m_auto_gain_a2, m_s, m_prev_x, m_processing = { 0.0f };
};

class TapeBird
{
public:
    TapeBird();
    ~TapeBird() = default;

    void prepare (juce::dsp::ProcessSpec& spec);

    void processBlock (juce::dsp::ProcessContextReplacing<float>& context);

    void setInputTrim (float trimInDecibels);
    void setProcessAmount (float processAmount);
    void setOutputTrim (float trimInDecibels);
    void setBrightnessOption (BrightnessOptions option);
    void setModeOption (ModeOptions option);
    void setMix (float mixPercent);
    void setAutoGainEnabled (bool isEnabled);

    void reset();

private:
    void update();

    juce::dsp::Gain<float> m_inputGainProcessor, m_outputGainProcessor, m_autoGainProcessor;

    juce::dsp::DryWetMixer<float> m_dryWetMixer;

    std::vector<TapeBirdMono> m_monoProcessors;
};