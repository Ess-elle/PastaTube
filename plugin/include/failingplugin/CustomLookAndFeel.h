#pragma once

#include "juce_gui_basics/juce_gui_basics.h"

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    void drawLabel (juce::Graphics& g, juce::Label& label) override;
};
