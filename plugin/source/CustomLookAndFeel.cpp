// CustomLookAndFeel.cpp
#include "pastaplugin/CustomLookAndFeel.h"
#include "juce_gui_basics/juce_gui_basics.h"

void CustomLookAndFeel::drawLabel (juce::Graphics& g, juce::Label& label)
{
    g.fillAll (label.findColour (juce::Label::backgroundColourId));

    auto alpha = label.isEnabled() ? 1.0f : 0.5f;
    auto cornerSize = 6.0f; // Adjust this for the roundness of the corners
    juce::Rectangle<int> textArea (label.getBorderSize().subtractedFrom (label.getLocalBounds()));

    g.setColour (label.findColour (juce::Label::outlineColourId).withMultipliedAlpha (alpha));
    g.drawRoundedRectangle (label.getLocalBounds().toFloat(), cornerSize, 1.0f);

    g.setColour (label.findColour (juce::Label::textColourId).withMultipliedAlpha (alpha));
    g.drawFittedText (label.getText(), textArea, label.getJustificationType(),
                      juce::jmin (static_cast<int>(textArea.getHeight() / label.getFont().getHeight()), 1),
                      label.getMinimumHorizontalScale());
}