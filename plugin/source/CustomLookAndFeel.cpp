// CustomLookAndFeel.cpp
#include "pastaplugin/CustomLookAndFeel.h"
#include "juce_gui_basics/juce_gui_basics.h"

void CustomLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label)
{
    auto bounds = label.getLocalBounds().toFloat();
    auto cornerSize = 6.0f; // Adjust this for the roundness of the corners

    // Fill the background with the label's background colour
    g.setColour(label.findColour(juce::Label::backgroundColourId));
    g.fillRoundedRectangle(bounds, cornerSize);

    auto alpha = label.isEnabled() ? 1.0f : 1.0f;

    // Draw the outline with the label's outline colour
    g.setColour(label.findColour(juce::Label::outlineColourId).withMultipliedAlpha(alpha));
    g.drawRoundedRectangle(bounds, cornerSize, 1.0f);

    // Set the text colour and draw the text
    g.setColour(label.findColour(juce::Label::textColourId).withMultipliedAlpha(alpha));
    juce::Rectangle<int> textArea(label.getBorderSize().subtractedFrom(label.getLocalBounds()));
    g.drawFittedText(label.getText(), textArea, label.getJustificationType(),
                     juce::jmin(static_cast<int>(textArea.getHeight() / label.getFont().getHeight()), 1),
                     label.getMinimumHorizontalScale());
}

void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
    {
        if (!knobImage.isValid())
        {
            // Handle the case where the image is not valid
            // This could be a fallback drawing method
            juce::LookAndFeel_V4::drawRotarySlider(g, x, y, width, height, sliderPosProportional, rotaryStartAngle, rotaryEndAngle, slider);
            return;
        }

        // Calculate the rotation angle based on the slider value
        float rotation = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

        // Get the center point of the slider
        float cx = x + width * 0.5f;
        float cy = y + height * 0.5f;

        // Draw the image centered and rotated
        juce::AffineTransform transform = juce::AffineTransform::rotation(rotation, cx, cy)
                                            .translated(cx - knobImage.getWidth() * 0.5f, cy - knobImage.getHeight() * 0.5f);

        g.drawImageTransformed(knobImage, transform, false);
    }