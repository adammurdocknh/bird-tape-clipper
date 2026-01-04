#pragma once

#include <juce_core/juce_core.h>

#if (MSVC)
    #include "ipps.h"
#endif

static constexpr int kParameterVersionHint = 1;

static const juce::String g_inputTrimID = "INPUT-TRIM";
static const juce::String g_processID = "PROCESS";
static const juce::String g_outputTrimID = "OUTPUT-TRIM";
static const juce::String g_brightnessID = "BRIGHTNESS";
static const juce::String g_modeID = "MODE";
static const juce::String g_mixID = "MIX";
static const juce::String g_autoGainID = "AUTO-GAIN";

static const juce::StringArray g_brightnessChoices = { "Opal", "Gold", "Sapphire" };

enum BrightnessOptions {
    Brightness_Opal = 0,
    Brightness_Gold,
    Brightness_Sapphire
};

static const juce::StringArray g_modeChoices = { "Luminescent", "Iridescent", "Radiant", "Dark Essence", "Luster" };

enum ModeOptions {
    ModeOptions_Luminescent = 0,
    ModeOptions_Iridescent,
    ModeOptions_Radiant,
    ModeOptions_DarkEssence,
    ModeOptions_Luster
};