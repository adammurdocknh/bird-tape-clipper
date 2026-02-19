// PluginEditor.h
#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_extra/juce_gui_extra.h>
#include "PluginProcessor.h"

class PluginEditor : public juce::AudioProcessorEditor,
                     private juce::Timer  // Add for parameter sync

{
public:
    PluginEditor(PluginProcessor&);
    ~PluginEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:

    void timerCallback() override;  // For parameter sync

    PluginProcessor& processor;

    // WebView browser component
    juce::WebBrowserComponent browser {
        juce::WebBrowserComponent::Options()
            .withBackend(juce::WebBrowserComponent::Options::Backend::webview2)
            .withBackend(juce::WebBrowserComponent::Options::Backend::webview2)
            .withResourceProvider([this](const auto& url) {
                return getResource(url);
            })
            .withNativeFunction("setParameter", [this](auto& args, auto complete) {
                handleSetParameter(args, complete);
            })
            .withNativeFunction("getParameter", [this](auto& args, auto complete) {
                handleGetParameter(args, complete);
            })
            .withNativeFunction("beginGesture", [this](auto& args, auto complete) {
                handleBeginGesture(args, complete);
            })
            .withNativeFunction("endGesture", [this](auto& args, auto complete) {
                handleEndGesture(args, complete);
            })
            .withNativeFunction("requestParamSync", [this](auto& args, auto complete) {
                syncAllParametersToWebView();
                complete({});
            })
    };

    // Resource provider for loading HTML/CSS/JS
    std::optional<juce::WebBrowserComponent::Resource> getResource(const juce::String& url);

    // Native function handlers
    void handleSetParameter(const juce::Array<juce::var>& args,
        juce::WebBrowserComponent::NativeFunctionCompletion complete) const;
    void handleGetParameter(const juce::Array<juce::var>& args,
        juce::WebBrowserComponent::NativeFunctionCompletion complete) const;
    void handleBeginGesture(const juce::Array<juce::var>& args,
        juce::WebBrowserComponent::NativeFunctionCompletion complete) const;
    void handleEndGesture(const juce::Array<juce::var>& args,
        juce::WebBrowserComponent::NativeFunctionCompletion complete) const;

    // Parameter sync system
    void syncAllParametersToWebView();
    std::map<juce::String, float> lastSentValues;  // Track last sent values


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};