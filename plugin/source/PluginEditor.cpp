#include "failingplugin/PluginProcessor.h"
#include "failingplugin/PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    // Make sure that before the constructor has finished, you've set the editor's size to whatever you need it to be.
    setSize (400, 300);

    gainSlider.setSliderStyle(juce::Slider::LinearVertical);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 40); //width, height
    gainSlider.setRange(-60.0, 12.0); // Set the range in decibels
    gainSlider.setSkewFactor(0.0); // ??
    gainSlider.setNumDecimalPlacesToDisplay(1); // Display one decimal place
    gainSlider.setTextValueSuffix(" dB"); // Add " dB" suffix to the value
    addAndMakeVisible(gainSlider);

        // Change slider colors
    gainSlider.setColour(juce::Slider::trackColourId, juce::Colours::palevioletred);
    gainSlider.setColour(juce::Slider::thumbColourId, juce::Colours::mediumvioletred);
    gainSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::grey); // Change text color
    gainSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::lightgrey); // Change text box background color

    // Initialize the gain label
    gainLabel.setText("Gain", juce::dontSendNotification);
    gainLabel.setFont(juce::Font(15.0f, juce::Font::bold));
    gainLabel.setColour(juce::Label::textColourId, juce::Colours::grey);
    gainLabel.attachToComponent(&gainSlider, false);
    gainLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(gainLabel);

    //attach the slider to the parameter
    gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.parameters, "gain", gainSlider);

    // Add listener to the slider
    gainSlider.addListener (this);

    

    // Start the timer for animation - taken out to stop automatically starting
    //startTimerHz(60); // 60 frames per second
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
   // Stop the timer when the editor is destroyed
    stopTimer();
   
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
    auto* gainParameter = processorRef.parameters.getRawParameterValue("gain"); //maybe change this
    float gainInDecibels = gainParameter->load();
    float gainLinear = juce::Decibels::decibelsToGain(gainInDecibels);

    // Use the gain value to adjust the wave amplitude
    float waveAmplitude = 20.0f * gainLinear;
    float waveFrequency = 0.05f;
    float lineThickness = 10.0f;

    float phaseShift = animationPhase;

     for (int i = 0; i < numWaves; ++i)
    {
        juce::Path wavePath;
        wavePath.startNewSubPath (backgroundRect.getX(), backgroundRect.getY() + i * waveAmplitude);

        for (float x = backgroundRect.getX(); x < backgroundRect.getRight(); x += 1.0f)
        {
            float y = backgroundRect.getY() + i * waveAmplitude + std::sin(x * waveFrequency + phaseShift) * waveAmplitude;
            wavePath.lineTo (x, y);
        }

        g.strokePath (wavePath, juce::PathStrokeType (lineThickness));
    }
    
    // Set the text area to a fixed size and position
    int textAreaWidth = 200;
    int textAreaX = (getWidth() - textAreaWidth) / 2;

    juce::Rectangle<int> textArea(textAreaX, 10, textAreaWidth, 50);
    g.setColour(backgroundColour);
    g.fillRoundedRectangle(textArea.toFloat(), 10.0f); // Fill rounded rectangle with 10 pixel corner radius
    
    juce::Font font;
    font.setHeight(24.0f); // Set the desired font size
    font.setBold(true);
    g.setFont(font);

    g.setColour(textColour);
    g.drawFittedText("esselleAudio", textArea, juce::Justification::centred, 1);
}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    // Lay out the positions of subcomponents
    int sliderWidth = 100;
    int sliderHeight = getHeight() - 140;
    int sliderX = 40;
    int sliderY = 120;

    gainSlider.setBounds(sliderX, sliderY, sliderWidth, sliderHeight);

    // Center the gainLabel with the gainSlider
    int labelWidth = gainSlider.getWidth();
    int labelHeight = 20; // Height of the label
    int labelX = sliderX;
    int labelY = sliderY - labelHeight - 5; // Position it above the slider with a 5 pixel gap

    gainLabel.setBounds(labelX, labelY, labelWidth, labelHeight);
}

void AudioPluginAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &gainSlider)
    {
        // Force a repaint whenever the slider value changes
        repaint();

        // Update the last slider change time and start the timer
        lastSliderChangeTime = juce::Time::getCurrentTime();
        startTimerHz(60); // 60 frames per second
    }
}

void AudioPluginAudioProcessorEditor::timerCallback()
{
    // Update the phase for the sine wave animation
    animationPhase += 0.1f;
    repaint();

        // Check if the inactivity duration has passed since the last slider change
    if (juce::Time::getCurrentTime() > lastSliderChangeTime + juce::RelativeTime::milliseconds(inactivityDurationMs))
    {
        stopTimer();
    }
}
