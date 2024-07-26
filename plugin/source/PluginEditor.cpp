#include "pastaplugin/PluginProcessor.h"
#include "pastaplugin/PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    // Make sure that before the constructor has finished, you've set the editor's size to whatever you need it to be.
    setSize (400, 300);

    // Create an instance of the custom LookAndFeel
    customLookAndFeel = std::make_unique<CustomLookAndFeel>();

    // Initialise inSlider
    inSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    inSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 40);
    inSlider.setRange(-60.0, 12.0);
    inSlider.setValue(0.0);
    inSlider.setTextValueSuffix(" dB");
    inSlider.setNumDecimalPlacesToDisplay(1);
    inSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    inSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::indigo);
    inSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    addAndMakeVisible(inSlider);

    // Initialise driveSlider
    driveSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    driveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 40);
    driveSlider.setRange(0.0, 1.0);
    driveSlider.setValue(0.35);
    driveSlider.setNumDecimalPlacesToDisplay(2);
    driveSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    driveSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::indigo);
    driveSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    addAndMakeVisible(driveSlider);

    // Initialise outSlider
    outSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    outSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 40);
    outSlider.setRange(0.0, 1.0);
    outSlider.setValue(0.5);
    outSlider.setNumDecimalPlacesToDisplay(2);
    outSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    outSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::indigo);
    outSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    addAndMakeVisible(outSlider);

    //attach the slider to the parameter
    inSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.parameters, "input", inSlider);
    driveSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.parameters, "drive", driveSlider);
    outSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.parameters, "output", outSlider);

    // Initialize the labels
    inLabel.setText("input", juce::dontSendNotification);
    inLabel.setFont(juce::Font(15.0f, juce::Font::bold));
    inLabel.setColour(juce::Label::textColourId, juce::Colours::grey);
    inLabel.attachToComponent(&inSlider, false);
    inLabel.setJustificationType(juce::Justification::centred);
    inLabel.setColour(juce::Label::backgroundColourId, juce::Colours::pink);
    inLabel.setBorderSize(juce::BorderSize<int>(3)); // Set border size to create rounded edges
    addAndMakeVisible(inLabel);

    driveLabel.setText("drive", juce::dontSendNotification);
    driveLabel.setFont(juce::Font(15.0f, juce::Font::bold));
    driveLabel.setColour(juce::Label::textColourId, juce::Colours::grey);
    driveLabel.attachToComponent(&driveSlider, false);
    driveLabel.setJustificationType(juce::Justification::centred);
    driveLabel.setColour(juce::Label::backgroundColourId, juce::Colours::pink);
    driveLabel.setBorderSize(juce::BorderSize<int>(3)); // Set border size to create rounded edges
    addAndMakeVisible(driveLabel);

    outLabel.setText("output", juce::dontSendNotification);
    outLabel.setFont(juce::Font(15.0f, juce::Font::bold));
    outLabel.setColour(juce::Label::textColourId, juce::Colours::grey);
    outLabel.attachToComponent(&outSlider, false);
    outLabel.setJustificationType(juce::Justification::centred);
    outLabel.setColour(juce::Label::backgroundColourId, juce::Colours::pink);
    outLabel.setBorderSize(juce::BorderSize<int>(3)); // Set border size to create rounded edges
    addAndMakeVisible(outLabel);


    // Add listener to the slider
    inSlider.addListener (this);

    // Start the timer for animation - taken out to stop automatically starting
    //startTimerHz(60); // 60 frames per second
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
   // Clean up slider attachments
    inSliderAttachment = nullptr;
    driveSliderAttachment = nullptr;
    outSliderAttachment = nullptr;

    // Stop timer if running
    stopTimer();

    // Remove listener when destroyed
    inSlider.removeListener(this);
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

    // Retrieve the in value from the processor
    auto* inParameter = processorRef.parameters.getRawParameterValue("in"); //maybe change this
    float inInDecibels = inParameter->load();
    float inLinear = juce::Decibels::decibelsToGain(inInDecibels);

    // Use the in value to adjust the wave amplitude
    float waveAmplitude = 20.0f * inLinear;
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
    g.drawFittedText("Pastaaa", textArea, juce::Justification::centred, 1);
}

void AudioPluginAudioProcessorEditor::resized()
{
    // Lay out the positions of subcomponents
    // Define dimensions and positions for sliders
    int sliderWidth = 100;
    int sliderHeight = 100;
   
    // Calculate the center position for the drive slider
    int centerX = getWidth() / 2;
    int sliderY = 120;

    // Position the driveSlider in the center
    driveSlider.setBounds(centerX - (sliderWidth / 2), sliderY, sliderWidth, sliderHeight);

    // Calculate spacing between sliders
    int sliderSpacing = 30; // Adjust as needed

     // Position the inSlider to the left of the driveSlider
    int inSliderX = centerX - (sliderWidth / 2) - sliderWidth - sliderSpacing;
    inSlider.setBounds(inSliderX, sliderY, sliderWidth, sliderHeight);

    // Position the outSlider to the right of the driveSlider
    int outSliderX = centerX + (sliderWidth / 2) + sliderSpacing;
    outSlider.setBounds(outSliderX, sliderY, sliderWidth, sliderHeight);

    // Center the labels with the sliders
    int labelWidth = inSlider.getWidth();
    int labelHeight = 20; // Height of the label
    int labelY = sliderY - labelHeight - 5; // Position it above the slider with a 5 pixel p

    // Center the inLabel with the inSlider
    inLabel.setBounds(inSliderX, labelY, labelWidth, labelHeight);

    // Center the driveLabel with the driveSlider
    driveLabel.setBounds(centerX - (labelWidth / 2), labelY, labelWidth, labelHeight);

    // Center the outLabel with the outSlider
    outLabel.setBounds(outSliderX, labelY, labelWidth, labelHeight);
}

void AudioPluginAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &inSlider)
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