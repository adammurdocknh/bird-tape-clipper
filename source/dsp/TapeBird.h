#pragma once

#include "../StandardIncludes.h"
#include <juce_dsp/juce_dsp.h>
class TapeBirdMono
{
public:
    TapeBirdMono() = default;
    ~TapeBirdMono() = default;
    void prepare (double sampleRate);
    double processSample (double x);
    double processSat (double x);
    void setProcessAmount (double processAmount);
    void setBrightnessOption (BrightnessOptions option);
    void setModeOption (ModeOptions option);

    void reset();

    double getLinearGainValue();

private:
    void update();

    BrightnessOptions m_brightness_option = Brightness_Gold;
    ModeOptions m_mode_option = ModeOptions_Radiant;

    int m_sat_type{};

    bool m_g0{};

    double m_sr_scale = 1.f;

    double m_a3 = 0.0, m_f1 = 0.0, m_p20 = 0.0, m_p24 = 0.0,
           m_lpf_k = 0.0, m_hpf_k = 0.0,
           m_auto_gain_a1 = 0.0, m_auto_gain_a2 = 0.0,
           m_s = 0.0, m_prev_x = 0.0, m_processing = 0.0;
};

class TapeBird
{
public:
    TapeBird();
    ~TapeBird() = default;

    void prepare (juce::dsp::ProcessSpec& spec);

    void processBlock (juce::dsp::ProcessContextReplacing<float>& context);

    void setInputTrim (double trimInDecibels);
    void setProcessAmount (double processAmount);
    void setOutputTrim (double trimInDecibels);
    void setBrightnessOption (BrightnessOptions option);
    void setModeOption (ModeOptions option);
    void setAutoGainEnabled (bool isEnabled);

    void reset();

private:
    juce::dsp::Gain<float> m_inputGainProcessor, m_outputGainProcessor, m_autoGainProcessor;

    std::vector<TapeBirdMono> m_monoProcessors;
};
