#include "driv3/PluginEditor.h"
#include "driv3/PluginProcessor.h"
#include "BinaryData.h"

namespace audio_plugin {
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(
    AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    softClipButton("SOFTCLIPBUTTON", juce::DrawableButton::ButtonStyle::ImageStretched),
    hardClipButton("HARDCLIPBUTTON", juce::DrawableButton::ButtonStyle::ImageStretched),
    fuzzClipButton("FUZZCLIPBUTTON", juce::DrawableButton::ButtonStyle::ImageStretched),
    bypassButton("BYPASS", juce::DrawableButton::ButtonStyle::ImageFitted)
{
    // Helper lambda to load SVG data
    auto createDrawableFromSVG = [](const char* data, int size) -> std::unique_ptr<juce::Drawable> {
        if (auto svgXml = juce::XmlDocument::parse(juce::String::createStringFromData(data, size))) {
            return juce::Drawable::createFromSVG(*svgXml);
        }
        return nullptr;
    };

    // Start timer
    startTimerHz(24);

    // Levelmeter input
    addAndMakeVisible(inputHorizontalMeterL);
    addAndMakeVisible(inputHorizontalMeterR);

    // Levelmeter output
    addAndMakeVisible(outputHorizontalMeterL);
    addAndMakeVisible(outputHorizontalMeterR);

    // Drive Knob
    driveKnob.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    driveKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    driveKnob.setLookAndFeel(&customLookAndFeel);
    addAndMakeVisible(driveKnob);
    driveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(*audioProcessor.apvts, "DRIVE", driveKnob);

    // Mix Knob
    mixKnob.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    mixKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    mixKnob.setLookAndFeel(&mixSliderLookAndFeel);
    addAndMakeVisible(mixKnob);
    mixKnob.setVisible(true);
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(*audioProcessor.apvts, "MIX", mixKnob);

    // Input Knob
    inputKnob.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    inputKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    inputKnob.setLookAndFeel(&gainSliderLookAndFeel);
    addAndMakeVisible(inputKnob);
    inputKnob.setVisible(true);
    inputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(*audioProcessor.apvts, "INPUT", inputKnob);

    // Output Knob
    outputKnob.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    outputKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    outputKnob.setLookAndFeel(&gainSliderLookAndFeel);
    addAndMakeVisible(outputKnob);
    outputKnob.setVisible(true);
    outputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(*audioProcessor.apvts, "OUTPUT", outputKnob);

    // Saturation Buttons
    auto* satParam = audioProcessor.apvts->getParameter("SAT_TYPE");

    // Create Drawables
    softClipOff = createDrawableFromSVG(BinaryData::softclipOff_svg, BinaryData::softclipOff_svgSize);
    hardClipOff = createDrawableFromSVG(BinaryData::hardclipOff_svg, BinaryData::hardclipOff_svgSize);
    fuzzClipOff = createDrawableFromSVG(BinaryData::fuzzclipOff_svg, BinaryData::fuzzclipOff_svgSize);
    softClipOn = createDrawableFromSVG(BinaryData::softclipOn_svg, BinaryData::softclipOn_svgSize);
    hardClipOn = createDrawableFromSVG(BinaryData::hardclipOn_svg, BinaryData::hardclipOn_svgSize);
    fuzzClipOn = createDrawableFromSVG(BinaryData::fuzzclipOn_svg, BinaryData::fuzzclipOn_svgSize);

    bypassOff = createDrawableFromSVG(BinaryData::bypass_off_svg, BinaryData::bypass_off_svgSize);
    bypassOff_Hover = createDrawableFromSVG(BinaryData::bypass_off_hover_svg, BinaryData::bypass_off_hover_svgSize);
    bypassOn = createDrawableFromSVG(BinaryData::bypass_on_svg, BinaryData::bypass_on_svgSize);
    bypassOn_Hover = createDrawableFromSVG(BinaryData::bypass_on_hover_svg, BinaryData::bypass_on_hover_svgSize);

    // Softclip Button
    softClipButton.setImages(softClipOff.get(), nullptr, nullptr, nullptr, softClipOn.get());
    addAndMakeVisible(softClipButton);
    softClipButton.setClickingTogglesState(true);
    softClipButton.setRadioGroupId(1);
    softClipButton.setToggleState(satParam->getValue() == 0.0f, juce::dontSendNotification);
    softClipButton.onClick = [this, satParam] { satParam->setValueNotifyingHost(0.0f); };

    // Hardclip Button
    hardClipButton.setImages(hardClipOff.get(), nullptr, nullptr, nullptr, hardClipOn.get());
    addAndMakeVisible(hardClipButton);
    hardClipButton.setRadioGroupId(1);
    hardClipButton.setClickingTogglesState(true);
    hardClipButton.setToggleState(satParam->getValue() == 0.5f, juce::dontSendNotification);
    hardClipButton.onClick = [this, satParam] { satParam->setValueNotifyingHost(0.5f); };

    // Fuzzclip Button
    fuzzClipButton.setImages(fuzzClipOff.get(), nullptr, nullptr, nullptr, fuzzClipOn.get());
    addAndMakeVisible(fuzzClipButton);
    fuzzClipButton.setRadioGroupId(1);
    fuzzClipButton.setClickingTogglesState(true);
    fuzzClipButton.setToggleState(satParam->getValue() == 1.0f, juce::dontSendNotification);
    fuzzClipButton.onClick = [this, satParam] { satParam->setValueNotifyingHost(1.0f); };

    // Bypass Button
    bypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(*audioProcessor.apvts, "BYPASS", bypassButton);
    bypassButton.setImages(bypassOff.get(), bypassOff_Hover.get(), nullptr, nullptr, bypassOn.get(), bypassOn_Hover.get());
    addAndMakeVisible(bypassButton);
    bypassButton.setClickingTogglesState(true);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (static_cast<int>(figmaWidth * 0.45f), static_cast<int>(figmaHeight * 0.45f));

    setResizable(true, true);
    constrainer = std::make_unique<juce::ComponentBoundsConstrainer>();
    constrainer->setFixedAspectRatio(2.0);
    constrainer->setMinimumSize(static_cast<int>(figmaWidth * 0.2), static_cast<int>(figmaHeight * 0.2));
    constrainer->setMaximumSize(figmaWidth, figmaHeight);

    resizableCorner = std::make_unique<juce::ResizableCornerComponent>(this, constrainer.get());
    addAndMakeVisible(*resizableCorner);

    // Load SVG from binary data
    background = createDrawableFromSVG(BinaryData::background_blur_filter_svg, BinaryData::background_blur_filter_svgSize);
    uiForeground = createDrawableFromSVG(BinaryData::foreground_ui_svg, BinaryData::foreground_ui_svgSize);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {}

void AudioPluginAudioProcessorEditor::timerCallback() // Gets called everytime the Timer gets used. 24hz refreshrate
{
    // Input rms values
    inputHorizontalMeterL.setLevel(std::get<0>(audioProcessor.getRmsValue(0)));
    inputHorizontalMeterR.setLevel(std::get<0>(audioProcessor.getRmsValue(1)));
    inputHorizontalMeterL.repaint();
    inputHorizontalMeterR.repaint();

    // Output rms values
    outputHorizontalMeterL.setLevel(std::get<1>(audioProcessor.getRmsValue(0)));
    outputHorizontalMeterR.setLevel(std::get<1>(audioProcessor.getRmsValue(1)));
    outputHorizontalMeterL.repaint();
    outputHorizontalMeterR.repaint();
}

void AudioPluginAudioProcessorEditor::paint(juce::Graphics& g) {
  g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

  if (background)
  {
      background->drawWithin(g, getLocalBounds().toFloat(), juce::RectanglePlacement::stretchToFit, 1.0f);
  }
  else
  {
      // (Our component is opaque, so we must completely fill the background with a solid colour)
      g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
  }

  if (uiForeground)
  {
      uiForeground->drawWithin(g, getLocalBounds().toFloat(), juce::RectanglePlacement::stretchToFit, 0.5f);
  }
}

void AudioPluginAudioProcessorEditor::resized() {
  if (constrainer)
      constrainer->checkComponentBounds(this);

  if(resizableCorner)
      resizableCorner->setBounds(getWidth() - 16, getHeight() - 16, 16, 16);
  

  float scaleFactor = (float)getWidth() / figmaWidth; // Result: 0.4

  // Input rmsmeter
  inputHorizontalMeterL.setBounds(static_cast<int>(64 * scaleFactor), static_cast<int>(799 * scaleFactor), static_cast<int>(497 * scaleFactor), static_cast<int>(15 * scaleFactor));
  inputHorizontalMeterR.setBounds(static_cast<int>(64 * scaleFactor), static_cast<int>(819 * scaleFactor), static_cast<int>(497 * scaleFactor), static_cast<int>(15 * scaleFactor));

  // Output rmsmeter
  outputHorizontalMeterL.setBounds(static_cast<int>(1397 * scaleFactor), static_cast<int>(799 * scaleFactor), static_cast<int>(497 * scaleFactor), static_cast<int>(15 * scaleFactor));
  outputHorizontalMeterR.setBounds(static_cast<int>(1397 * scaleFactor), static_cast<int>(819 * scaleFactor), static_cast<int>(497 * scaleFactor), static_cast<int>(15 * scaleFactor));

  driveKnob.setBounds(static_cast<int>(50 * scaleFactor), static_cast<int>(237 * scaleFactor), static_cast<int>(196 * scaleFactor), static_cast<int>(515 * scaleFactor)); // Vertical slider bounds

  mixKnob.setBounds(static_cast<int>(1862 * scaleFactor), static_cast<int>(249 * scaleFactor), static_cast<int>(98 * scaleFactor), static_cast<int>(489 * scaleFactor));

  inputKnob.setBounds(static_cast<int>(70.5f * scaleFactor), static_cast<int>(852 * scaleFactor), static_cast<int>(495 * scaleFactor), static_cast<int>((63 + 30) * scaleFactor));

  outputKnob.setBounds(static_cast<int>(1403.5f * scaleFactor), static_cast<int>(852 * scaleFactor), static_cast<int>(495 * scaleFactor), static_cast<int>((63 + 30) * scaleFactor));



  const float buttonWidth = 100.67f * scaleFactor;

  const float buttonHeight = 82.0f * scaleFactor;

  softClipButton.setBounds(static_cast<int>(839.66f * scaleFactor), static_cast<int>(833.0f * scaleFactor), static_cast<int>(buttonWidth), static_cast<int>(buttonHeight));

  hardClipButton.setBounds(static_cast<int>(950.34f * scaleFactor), static_cast<int>(833.0f * scaleFactor), static_cast<int>(buttonWidth), static_cast<int>(buttonHeight));

  fuzzClipButton.setBounds(static_cast<int>(1060.34f * scaleFactor), static_cast<int>(833.0f * scaleFactor), static_cast<int>(buttonWidth), static_cast<int>(buttonHeight));

  bypassButton.setBounds(static_cast<int>(1826.0f * scaleFactor), static_cast<int>(92.0f * scaleFactor), static_cast<int>(124.0f * scaleFactor), static_cast<int>(40.0f * scaleFactor));
}
}  // namespace audio_plugin
