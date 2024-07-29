#include "pastaplugin/PluginProcessor.h"
#include "pastaplugin/PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    // Make sure that before the constructor has fmixished, you've set the editor's size to whatever you need it to be.
    setSize (400, 300);

    // Create an mixstance of the custom LookAndFeel
    customLookAndFeel = std::make_unique<CustomLookAndFeel>();

   // mixitialise driveSlider
    driveSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    driveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 40);
    driveSlider.setRange(0.0, 10.0);
    driveSlider.setValue(0.0);
    driveSlider.setNumDecimalPlacesToDisplay(2);
    driveSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    driveSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::indigo);
    driveSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    addAndMakeVisible(driveSlider);

     // initialise mixSlider
    mixSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 40);
    mixSlider.setRange(0.0, 1.0);
    mixSlider.setValue(0.0);
    mixSlider.setTextValueSuffix(" dB");
    mixSlider.setNumDecimalPlacesToDisplay(1);
    mixSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    mixSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::indigo);
    mixSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    addAndMakeVisible(mixSlider);

    // initialise outSlider
    outSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    outSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 40);
    outSlider.setRange(-6, 20.0);
    outSlider.setValue(0.0);
    outSlider.setNumDecimalPlacesToDisplay(2);
    outSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    outSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::indigo);
    outSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    addAndMakeVisible(outSlider);

    //attach the slider to the parameter
    driveSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.parameters, "drive", driveSlider);
    mixSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.parameters, "mix", mixSlider);
    outSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.parameters, "output", outSlider);

    // initialise the labels
    driveLabel.setText("drive", juce::dontSendNotification);
    driveLabel.setFont(juce::Font(15.0f, juce::Font::bold));
    driveLabel.setColour(juce::Label::textColourId, juce::Colours::grey);
    driveLabel.attachToComponent(&driveSlider, false);
    driveLabel.setJustificationType(juce::Justification::centred);
    driveLabel.setColour(juce::Label::backgroundColourId, juce::Colours::pink);
    driveLabel.setBorderSize(juce::BorderSize<int>(3)); // Set border size to create rounded edges
    addAndMakeVisible(driveLabel);

    mixLabel.setText("mix", juce::dontSendNotification);
    mixLabel.setFont(juce::Font(15.0f, juce::Font::bold));
    mixLabel.setColour(juce::Label::textColourId, juce::Colours::grey);
    mixLabel.attachToComponent(&mixSlider, false);
    mixLabel.setJustificationType(juce::Justification::centred);
    mixLabel.setColour(juce::Label::backgroundColourId, juce::Colours::pink);
    mixLabel.setBorderSize(juce::BorderSize<int>(3)); // Set border size to create rounded edges
    addAndMakeVisible(mixLabel);

    outLabel.setText("output", juce::dontSendNotification);
    outLabel.setFont(juce::Font(15.0f, juce::Font::bold));
    outLabel.setColour(juce::Label::textColourId, juce::Colours::grey);
    outLabel.attachToComponent(&outSlider, false);
    outLabel.setJustificationType(juce::Justification::centred);
    outLabel.setColour(juce::Label::backgroundColourId, juce::Colours::pink);
    outLabel.setBorderSize(juce::BorderSize<int>(3)); // Set border size to create rounded edges
    addAndMakeVisible(outLabel);

    // Add listener to the slider
    mixSlider.addListener (this);

    // Start the timer for animation - taken out to stop automatically startmixg
    //startTimerHz(60); // 60 frames per second
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
   // Clean up slider attachments
    driveSliderAttachment = nullptr;
    mixSliderAttachment = nullptr;
    outSliderAttachment = nullptr;

    // Stop timer if runnmixg
    stopTimer();

    // Remove listener when destroyed
    mixSlider.removeListener(this);
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

    // Retrieve the mix value from the processor
    auto* mixParameter = processorRef.parameters.getRawParameterValue("mix"); //maybe change this
    float mix = mixParameter->load();

    // Use the mix value to adjust the wave amplitude
    float waveAmplitude = 20.0f * mix;
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
    // Defmixe dimensions and positions for sliders
    int sliderWidth = 100;
    int sliderHeight = 100;
   
    // Calculate the center position for the drive slider
    int centerX = getWidth() / 2;
    int sliderY = 120;

    // Position the driveSlider mix the center
    driveSlider.setBounds(centerX - (sliderWidth / 2), sliderY, sliderWidth, sliderHeight);

    // Calculate spacmixg between sliders
    int sliderSpacmixg = 30; // Adjust as needed

     // Position the mixSlider to the left of the driveSlider
    int mixSliderX = centerX - (sliderWidth / 2) - sliderWidth - sliderSpacmixg;
    mixSlider.setBounds(mixSliderX, sliderY, sliderWidth, sliderHeight);

    // Position the outSlider to the right of the driveSlider
    int outSliderX = centerX + (sliderWidth / 2) + sliderSpacmixg;
    outSlider.setBounds(outSliderX, sliderY, sliderWidth, sliderHeight);

    // Center the labels with the sliders
    int labelWidth = mixSlider.getWidth();
    int labelHeight = 20; // Height of the label
    int labelY = sliderY - labelHeight - 5; // Position it above the slider with a 5 pixel p

    // Center the mixLabel with the mixSlider
    mixLabel.setBounds(mixSliderX, labelY, labelWidth, labelHeight);

    // Center the driveLabel with the driveSlider
    driveLabel.setBounds(centerX - (labelWidth / 2), labelY, labelWidth, labelHeight);

    // Center the outLabel with the outSlider
    outLabel.setBounds(outSliderX, labelY, labelWidth, labelHeight);
}

void AudioPluginAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &mixSlider)
    {
        // Force a repamixt whenever the slider value changes
        repaint();

        // Update the last slider change time and start the timer
        lastSliderChangeTime = juce::Time::getCurrentTime();
        startTimerHz(60); // 60 frames per second
    }
}

void AudioPluginAudioProcessorEditor::timerCallback()
{
    // Update the phase for the smixe wave animation
    animationPhase += 0.1f;
    // Ensure this runs on the message thread
    juce::MessageManager::callAsync([this] { repaint(); });

    // Check if the mixactivity duration has passed smixce the last slider change
    if (juce::Time::getCurrentTime() > lastSliderChangeTime + juce::RelativeTime::milliseconds(inactivityDurationMs))
    {
        stopTimer();
    }
}