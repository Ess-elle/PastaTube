#include "failingplugin/PluginProcessor.h"
#include "failingplugin/PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    // Make sure that before the constructor has finished, you've set the editor's size to whatever you need it to be.
    setSize (400, 300);

    // Create an instance of the custom LookAndFeel
    customLookAndFeel = std::make_unique<CustomLookAndFeel>();

    // Initialize gainSlider
    gainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 40);
    gainSlider.setRange(-60.0, 12.0);
    gainSlider.setValue(0.0);
    gainSlider.setTextValueSuffix(" dB");
    gainSlider.setNumDecimalPlacesToDisplay(1);
    gainSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    gainSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::indigo);
    gainSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    addAndMakeVisible(gainSlider);

    // Initialize feedbackSlider
    feedbackSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    feedbackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 40);
    feedbackSlider.setRange(0.0, 1.0);
    feedbackSlider.setValue(0.35);
    feedbackSlider.setNumDecimalPlacesToDisplay(2);
    feedbackSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    feedbackSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::indigo);
    feedbackSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    addAndMakeVisible(feedbackSlider);

    // Initialize mixSlider
    mixSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 40);
    mixSlider.setRange(0.0, 1.0);
    mixSlider.setValue(0.5);
    mixSlider.setNumDecimalPlacesToDisplay(2);
    mixSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    mixSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::indigo);
    mixSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    addAndMakeVisible(mixSlider);

    //attach the slider to the parameter
    gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.parameters, "gain", gainSlider);
    feedbackSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.parameters, "feedback", feedbackSlider);
    mixSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.parameters, "mix", mixSlider);

    // Initialize the labels
    gainLabel.setText("Gain", juce::dontSendNotification);
    gainLabel.setFont(juce::Font(15.0f, juce::Font::bold));
    gainLabel.setColour(juce::Label::textColourId, juce::Colours::grey);
    gainLabel.attachToComponent(&gainSlider, false);
    gainLabel.setJustificationType(juce::Justification::centred);
    gainLabel.setColour(juce::Label::backgroundColourId, juce::Colours::pink);
    gainLabel.setBorderSize(juce::BorderSize<int>(3)); // Set border size to create rounded edges
    addAndMakeVisible(gainLabel);

    feedbackLabel.setText("Feedback", juce::dontSendNotification);
    feedbackLabel.setFont(juce::Font(15.0f, juce::Font::bold));
    feedbackLabel.setColour(juce::Label::textColourId, juce::Colours::grey);
    feedbackLabel.attachToComponent(&feedbackSlider, false);
    feedbackLabel.setJustificationType(juce::Justification::centred);
    feedbackLabel.setColour(juce::Label::backgroundColourId, juce::Colours::pink);
    feedbackLabel.setBorderSize(juce::BorderSize<int>(3)); // Set border size to create rounded edges
    addAndMakeVisible(feedbackLabel);

    mixLabel.setText("Mix", juce::dontSendNotification);
    mixLabel.setFont(juce::Font(15.0f, juce::Font::bold));
    mixLabel.setColour(juce::Label::textColourId, juce::Colours::grey);
    mixLabel.attachToComponent(&mixSlider, false);
    mixLabel.setJustificationType(juce::Justification::centred);
    mixLabel.setColour(juce::Label::backgroundColourId, juce::Colours::pink);
    mixLabel.setBorderSize(juce::BorderSize<int>(3)); // Set border size to create rounded edges
    addAndMakeVisible(mixLabel);


    // Add listener to the slider
    gainSlider.addListener (this);

    // Start the timer for animation - taken out to stop automatically starting
    //startTimerHz(60); // 60 frames per second
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
   // Clean up slider attachments
    gainSliderAttachment = nullptr;
    feedbackSliderAttachment = nullptr;
    mixSliderAttachment = nullptr;

    // Stop timer if running
    stopTimer();

    // Remove listener when destroyed
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
    // Lay out the positions of subcomponents
    // Define dimensions and positions for sliders
    int sliderWidth = 100;
    int sliderHeight = 100;
   
    // Calculate the center position for the feedback slider
    int centerX = getWidth() / 2;
    int sliderY = 120;

    // Position the feedbackSlider in the center
    feedbackSlider.setBounds(centerX - (sliderWidth / 2), sliderY, sliderWidth, sliderHeight);

    // Calculate spacing between sliders
    int sliderSpacing = 30; // Adjust as needed

     // Position the gainSlider to the left of the feedbackSlider
    int gainSliderX = centerX - (sliderWidth / 2) - sliderWidth - sliderSpacing;
    gainSlider.setBounds(gainSliderX, sliderY, sliderWidth, sliderHeight);

    // Position the mixSlider to the right of the feedbackSlider
    int mixSliderX = centerX + (sliderWidth / 2) + sliderSpacing;
    mixSlider.setBounds(mixSliderX, sliderY, sliderWidth, sliderHeight);

    // Center the labels with the sliders
    int labelWidth = gainSlider.getWidth();
    int labelHeight = 20; // Height of the label
    int labelY = sliderY - labelHeight - 5; // Position it above the slider with a 5 pixel gap

    // Center the gainLabel with the gainSlider
    gainLabel.setBounds(gainSliderX, labelY, labelWidth, labelHeight);

    // Center the feedbackLabel with the feedbackSlider
    feedbackLabel.setBounds(centerX - (labelWidth / 2), labelY, labelWidth, labelHeight);

    // Center the mixLabel with the mixSlider
    mixLabel.setBounds(mixSliderX, labelY, labelWidth, labelHeight);
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