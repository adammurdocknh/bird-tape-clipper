#include "TapeBird.h"

void TapeBirdMono::prepare (double sampleRate)
{
    m_sr_scale = static_cast<float> (sampleRate) / 44100.0f;
    update();
}

float TapeBirdMono::processSample (float x)
{
    //      processing = this.processing * this.a3;

    //   x1 = this.hpf_k * x + (x - this.prev_x);
    //   x2 = x1 * this.f1 + x1;
    //   x3 = (!this.g0) ? x : x2;
    //   x4 = (this.model_type == 3) ? this.Phoenix_sat(x2 * processing) : this.Phoenix_sat(x2);
    //   x5 = this.Phoenix_sat(x4 * processing * this.p20 + x3);

    //   this.prev_x = x;

    //   this.s += (x5 - this.s) * this.lpf_k;

    //   y = processing * (this.s - x * this.p24);

    //   (this.model_type == 3) ? ( y *= 0.5 );

    //   y += x;

    //   this.use_auto_gain ? ( y *= this.auto_gain );

    //   y;
    const float processing = m_processing * m_a3;
    const float x1 = m_hpf_k * x + (x - m_prev_x);
    const float x2 = x1 * m_f1 + x1;
    const float x3 = (!m_g0) ? x : x2;
    const float x4 = m_mode_option == ModeOptions_DarkEssence ? processSat (x2 * (processing)) : processSat (x2);
    const float x5 = processSat (x4 * processing * m_p20 + x3);

    m_prev_x = x;
    m_s += (x5 - m_s) * m_lpf_k;
    float y = processing * (m_s - x * m_p24);
    if (m_mode_option == ModeOptions_DarkEssence)
        y *= 0.5f;

    y += x;
    return y;
}

float TapeBirdMono::processSat (float x)
{
    // Certain modes share clipping/saturation functions which is why we have three in here even though there are 5 modes.
    switch (m_sat_type)
    {
        case 0:
        {
            const float clipped = juce::jlimit (-1.0f, 1.0f, x);
            const float x2 = clipped * clipped;
            const float x4 = x2 * x2;
            const float x6 = x4 * x2;
            const float x8 = x4 * x4;

            return x * 2.827568855 + x2 * 0.0003903798913 + x2 * x * -4.17220229 + x4 * -0.0001107320401 + x4 * x * 0.523459874 + x6 * 0.0002768079893 + x6 * x * -0.423546883 + x8 * -0.001448632 + x8 * x * 3.224580615 + x8 * x2 * 0.002728704 + x8 * x2 * x * -5.495344862 + x8 * x4 * -0.002846356 + x8 * x4 * x * 5.449768693 + x8 * x6 * 0.001310366 + x8 * x6 * x * -2.414078731;
            break;
        }
        case 1:
        {
            const float clipped = juce::jlimit (-0.991184403f, 0.990821248f, x);
            const float x2 = clipped * clipped;
            const float x4 = x2 * x2;
            const float x6 = x4 * x2;
            const float x8 = x4 * x4;

            return x * 1.501040337 + x2 * -0.0002757478168 + x2 * x * -0.301802438 + x4 * 0.003273802 + x4 * x * 1.786333688 + x6 * -0.046104732 + x6 * x * -24.582679252 + x8 * 0.110553367 + x8 * x * 41.112226106 + x8 * x2 * -0.092987632 + x8 * x2 * x * -16.724196818 + x8 * x4 * 0.01857341 + x8 * x4 * x * -9.331919223 + x8 * x6 * 0.006696015 + x8 * x6 * x * 6.543207186;
            break;
        }
        case 2:
        {
            const float clipped = juce::jlimit (-0.991022224f, 0.990984424f, x);
            const float x2 = clipped * clipped;
            const float x4 = x2 * x2;
            const float x6 = x4 * x2;
            const float x8 = x4 * x4;

            return x * 2.063930806 + x2 * 0.0002008141989 + x2 * x * -0.414990906 + x4 * -0.003741183 + x4 * x * 2.456380956 + x6 * 0.03108163 + x6 * x * -33.802027499 + x8 * -0.092816819 + x8 * x * 56.531406839 + x8 * x2 * 0.134928028 + x8 * x2 * x * -22.998647073 + x8 * x4 * -0.098216457 + x8 * x4 * x * -12.829323005 + x8 * x6 * 0.028676158 + x8 * x6 * x * 8.996306767;
            break;
        }
        default:
        {
            jassertfalse; // Should not get here
            break;
            return x;
        }
    }
}

void TapeBirdMono::setProcessAmount (float processAmount)
{
    // Set process amount code here
    m_processing = processAmount;
}

void TapeBirdMono::setBrightnessOption (BrightnessOptions option)
{
    // Set brightness option code here
    if (m_brightness_option == option)
        return;
    m_brightness_option = option;
    update();
}

void TapeBirdMono::setModeOption (ModeOptions option)
{
    // Set mode option code here
    if (m_mode_option == option)
        return;
    m_mode_option = option;
    update();
}

void TapeBirdMono::reset()
{
    m_prev_x = 0.0f;
    m_s = 0.0f;
}

float TapeBirdMono::getLinearGainValue()
{
    return 1.f + m_processing * m_auto_gain_a1 + m_processing * m_processing * m_auto_gain_a2;
}

void TapeBirdMono::update()
{
    // Update internal parameters based on m_brightness_option and m_mode_option
    // Example:
    switch (m_mode_option)
    {
        case ModeOptions_Luminescent:
        {
            switch (m_brightness_option)
            {
                case Brightness_Opal:
                    m_hpf_k = .625f;
                    m_lpf_k = .1875f;
                    break;
                case Brightness_Gold:
                    m_hpf_k = .4375f;
                    m_lpf_k = .3125f;
                    break;
                case Brightness_Sapphire:
                    m_hpf_k = .1875f;
                    m_lpf_k = .375f;
                    break;
                default:
                    jassertfalse; // Should not get here
                    break;
            }

            m_a3 = .25f;
            m_f1 = .75f;
            m_p20 = .3125f;
            m_p24 = .0625f;
            m_g0 = true;
            m_sat_type = 0;
            m_auto_gain_a1 = -.416f;
            m_auto_gain_a2 = .092f;
            break;
        }

        case ModeOptions_Iridescent:
        {
            switch (m_brightness_option)
            {
                case Brightness_Opal:
                    m_hpf_k = .625f;
                    m_lpf_k = .1875f;
                    break;
                case Brightness_Gold:
                    m_hpf_k = .375f;
                    m_lpf_k = .3125f;
                    break;
                case Brightness_Sapphire:
                    m_hpf_k = .3125f;
                    m_lpf_k = .375f;
                    break;
                default:
                    jassertfalse; // Should not get here
                    break;
            }

            m_a3 = .25f;
            m_f1 = .875f;
            m_p20 = .3125f;
            m_p24 = .0625f;
            m_g0 = true;
            m_sat_type = 0;
            m_auto_gain_a1 = -.393f;
            m_auto_gain_a2 = .082f;
            break;
        }
        case ModeOptions_Radiant:
        {
            switch (m_brightness_option)
            {
                case Brightness_Opal:
                    m_hpf_k = .75f;
                    m_lpf_k = .125f;
                    break;
                case Brightness_Gold:
                    m_hpf_k = .45629901f;
                    m_lpf_k = .375;
                    break;
                case Brightness_Sapphire:
                    m_hpf_k = .375f;
                    m_lpf_k = .5f;
                    break;
                default:
                    jassertfalse; // Should not get here
                    break;
            }

            m_a3 = .375f;
            m_f1 = .75f;
            m_p20 = .1875f;
            m_p24 = .0125f;
            m_g0 = false;
            m_sat_type = 1;
            m_auto_gain_a1 = -.441f;
            m_auto_gain_a2 = .103f;
            break;
        }
        case ModeOptions_Luster:
        {
            switch (m_brightness_option)
            {
                case Brightness_Opal:
                    m_hpf_k = .75f;
                    m_lpf_k = .125f;
                    break;
                case Brightness_Gold:
                    m_hpf_k = .45629901f;
                    m_lpf_k = .375;
                    break;
                case Brightness_Sapphire:
                    m_hpf_k = .375f;
                    m_lpf_k = .5625f;
                    break;
                default:
                    jassertfalse; // Should not get here
                    break;
            }

            m_a3 = 1.f;
            m_f1 = .6875f;
            m_p20 = .27343899f;
            m_p24 = .1171875f;
            m_g0 = false;
            m_sat_type = 2;
            m_auto_gain_a1 = -.712f;
            m_auto_gain_a2 = .172f;
            break;
        }

        case ModeOptions_DarkEssence:
        {
            switch (m_brightness_option)
            {
                case Brightness_Opal:
                    m_hpf_k = .75f;
                    m_lpf_k = .125f;
                    break;
                case Brightness_Gold:
                    m_hpf_k = .45629901f;
                    m_lpf_k = .375;
                    break;
                case Brightness_Sapphire:
                    m_hpf_k = .375f;
                    m_lpf_k = .5625f;
                    break;
                default:
                    jassertfalse; // Should not get here
                    break;
            }

            m_a3 = .375f;
            m_f1 = .75f;
            m_p20 = .5625f;
            m_p24 = .0125f;
            m_g0 = false;
            m_sat_type = 2;
            m_auto_gain_a1 = -.636f;
            m_auto_gain_a2 = .17f;
            break;
        }
        // Add other cases as needed
        default:
            jassertfalse; // Should not get here
            break;
    }
    m_hpf_k *= m_sr_scale;
    m_lpf_k *= m_sr_scale;
}

TapeBird::TapeBird()
{
    m_inputGainProcessor.setGainLinear (1.0f);
    m_inputGainProcessor.setRampDurationSeconds (0.01);

    m_outputGainProcessor.setGainLinear (1.0f);
    m_outputGainProcessor.setRampDurationSeconds (0.01);

    m_autoGainProcessor.setGainLinear (1.0f);
    m_autoGainProcessor.setRampDurationSeconds (0.01);
}

void TapeBird::prepare (juce::dsp::ProcessSpec& spec)
{
    m_inputGainProcessor.prepare (spec);
    m_outputGainProcessor.prepare (spec);
    m_autoGainProcessor.prepare (spec);

    if (spec.numChannels > m_monoProcessors.size())
    {
        const size_t numToAdd = spec.numChannels - m_monoProcessors.size();
        for (size_t i = 0; i < numToAdd; ++i)
            m_monoProcessors.emplace_back();
    }
    else if (spec.numChannels < m_monoProcessors.size())
    {
        m_monoProcessors.resize (spec.numChannels);
    }

    for (auto& monoProcessor : m_monoProcessors)
        monoProcessor.prepare (spec.sampleRate);
}

void TapeBird::processBlock (juce::dsp::ProcessContextReplacing<float>& context)
{
    const auto inputBlock = context.getInputBlock();
    auto outputBlock = context.getOutputBlock();
    const auto numChannels = inputBlock.getNumChannels();
    const auto numSamples = inputBlock.getNumSamples();

    m_inputGainProcessor.process (context);

    for (size_t channel = 0; channel < numChannels; ++channel)
    {
        float* wetData = outputBlock.getChannelPointer (static_cast<int> (channel));
        for (size_t i = 0; i < numSamples; ++i)
        {
            wetData[i] = m_monoProcessors[channel].processSample (wetData[i]);
        }
    }

    m_outputGainProcessor.process (context);
    m_autoGainProcessor.process (context);
}

void TapeBird::setInputTrim (float trimInDecibels)
{
    m_inputGainProcessor.setGainDecibels (trimInDecibels);
}

void TapeBird::setProcessAmount (float processAmount)
{
    for (auto& monoProcessor : m_monoProcessors)
        monoProcessor.setProcessAmount (processAmount / 100.f);
}

void TapeBird::setOutputTrim (float trimInDecibels)
{
    m_outputGainProcessor.setGainDecibels (trimInDecibels);
}

void TapeBird::setBrightnessOption (BrightnessOptions option)
{
    for (auto& monoProcessor : m_monoProcessors)
        monoProcessor.setBrightnessOption (option);
}

void TapeBird::setModeOption (ModeOptions option)
{
    for (auto& monoProcessor : m_monoProcessors)
        monoProcessor.setModeOption (option);
}

void TapeBird::reset()
{
    m_inputGainProcessor.reset();
    m_outputGainProcessor.reset();
    m_autoGainProcessor.reset();

    for (auto& monoProcessor : m_monoProcessors)
        monoProcessor.reset();
}

void TapeBird::setAutoGainEnabled (bool isEnabled)
{
    if (isEnabled)
        m_autoGainProcessor.setGainLinear (m_monoProcessors[0].getLinearGainValue());
    else
        m_autoGainProcessor.setGainLinear (1.0f);
}