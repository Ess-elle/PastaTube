#pragma once

#include "PluginEditor.h"
#include "PluginProcessor.h"
#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_gui_basics/juce_gui_basics.h"
#include "pastaplugin/CustomLookAndFeel.h"

//==============================================================================
class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor, 
                                        public juce::Slider::Listener,
                                        private juce::Timer
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

  
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    
    AudioPluginAudioProcessor& processorRef;
    juce::Slider inSlider;
    juce::Slider driveSlider;
    juce::Slider outSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outSliderAttachment;
    juce::Label inLabel;
    juce::Label driveLabel;
    juce::Label outLabel;


    //juce::ComboBox delayComboBox;
    //juce::TextEditor delayTextEditor;

    // Declare custom LookAndFeel
    std::unique_ptr<CustomLookAndFeel> customLookAndFeel;
    
    
    // Implement the slider listener method
    void sliderValueChanged(juce::Slider* slider) override;
    void timerCallback() override;
    float animationPhase = 0.0f;
    juce::Time lastSliderChangeTime;
    int inactivityDurationMs = 1000; // Stop the animation after 1 second of inactivity

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
