#include "failingplugin/PluginProcessor.h"
#include "failingplugin/PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    // Make sure that before the constructor has finished, you've set the editor's size to whatever you need it to be.
   
    gainSlider.setSliderStyle(juce::Slider::LinearVertical);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60,20);
    addAndMakeVisible(gainSlider);

    //attach the slider to the parameter
    gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.parameters, "gain", gainSlider);

    // Add listener to the slider
    gainSlider.addListener (this);

    setSize (400, 300);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
   // Remove the listener when the editor is destroyed
    gainSlider.removeListener(this);
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    juce::Colour palePinkPeach = juce::Colour::fromRGB(250, 220, 231); //waveycolours
    juce::Colour duskyLilac = juce::Colour::fromRGB(196, 169, 220);

    juce::Colour backgroundColour = juce::Colour::fromRGB(255, 201, 213); // Background colour for the text
    juce::Colour textColour = juce::Colour::fromRGB(164, 143, 183); // Colour for the text

    g.fillAll (duskyLilac);

    g.setColour (palePinkPeach);
    juce::Rectangle<float> backgroundRect = getLocalBounds().toFloat();
    int numWaves = 60;

    // Retrieve the gain value from the processor
    auto* gainParameter = processorRef.parameters.getRawParameterValue("gain");
    float gainInDecibels = gainParameter->load();
    float gainLinear = juce::Decibels::decibelsToGain(gainInDecibels);

    // Use the gain value to adjust the wave amplitude
    float waveAmplitude = 20.0f * gainLinear;

    float waveFrequency = 0.05f;
    float lineThickness = 10.0f;


     for (int i = 0; i < numWaves; ++i)
    {
        juce::Path wavePath;
        wavePath.startNewSubPath (backgroundRect.getX(), backgroundRect.getY() + i * waveAmplitude);

        for (float x = backgroundRect.getX(); x < backgroundRect.getRight(); x += 1.0f)
        {
            float y = backgroundRect.getY() + i * waveAmplitude + std::sin(x * waveFrequency) * waveAmplitude;
            wavePath.lineTo (x, y);
        }

        g.strokePath (wavePath, juce::PathStrokeType (lineThickness));
    }

     // Determine the text area
    juce::Rectangle<int> textArea = getLocalBounds();
    textArea = textArea.withSizeKeepingCentre(textArea.getWidth() / 2, textArea.getHeight() / 4);
    textArea.setY(10); // Align to the top

    // Fill the background for the text
    g.setColour(backgroundColour);
    g.fillRect(textArea);

    // Draw the text
    g.setColour(textColour);
    juce::Font font;
    font.setHeight(24.0f); // Set the desired font size
    font.setBold(true);
    g.setFont(font);
    textArea = textArea.withSizeKeepingCentre(textArea.getWidth() / 2, textArea.getHeight() / 4);
    textArea.setY(20); // Align to the top

    g.drawFittedText("esselleAudio", textArea, juce::Justification::centred, 1);
}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    gainSlider.setBounds (40, 30, 20, getHeight() - 60);
}

void AudioPluginAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &gainSlider)
    {
        // Force a repaint whenever the slider value changes
        repaint();
    }
}
