#pragma once

#include "juce_gui_basics/juce_gui_basics.h"

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel()
    {
        // Load the image for the knob in the constructor
        knobImage = juce::ImageCache::getFromFile(juce::File("C:/Users/siobh/Jucing/RigatoniTubeSaturation/plugin/assets/images/knobs/rigatoniKnob.png"));
    }

    void drawLabel(juce::Graphics& g, juce::Label& label) override;
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;

private:
    juce::Image knobImage;
};