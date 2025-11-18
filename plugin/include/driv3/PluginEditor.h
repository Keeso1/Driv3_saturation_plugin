#pragma once

#include "PluginProcessor.h"
#include <memory>
#include "CustomLookAndFeel.h"
#include "MixSliderLookAndFeel.h"
#include "HorizontalMeter.h"
#include "AnalyserComponent.h"
#include "GainSliderLookAndFeel.h"

namespace audio_plugin {

class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Timer {
public:
  explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor&);
  ~AudioPluginAudioProcessorEditor() override;

  void paint(juce::Graphics&) override;
  void resized() override;
  void timerCallback() override;

private:
  std::unique_ptr<juce::ResizableCornerComponent> resizableCorner;
  std::unique_ptr<juce::ComponentBoundsConstrainer> constrainer;

  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  AudioPluginAudioProcessor& audioProcessor;
  AnalyserComponent& analyserComponent;

  const int figmaWidth = 2000;
  const int figmaHeight = 1000;

  CustomLookAndFeel customLookAndFeel;
  MixSliderLookAndFeel mixSliderLookAndFeel;
  GainSliderLookAndFeel gainSliderLookAndFeel;

  juce::Slider driveKnob;
  juce::Slider mixKnob;
  juce::Slider inputKnob;
  juce::Slider outputKnob;
  juce::DrawableButton softClipButton;
  juce::DrawableButton hardClipButton;
  juce::DrawableButton fuzzClipButton;
  juce::DrawableButton bypassButton;

  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> bypassAttachment;

  std::unique_ptr<juce::Drawable> softClipOff;
  std::unique_ptr<juce::Drawable> softClipOn;
  std::unique_ptr<juce::Drawable> hardClipOff;
  std::unique_ptr<juce::Drawable> hardClipOn;
  std::unique_ptr<juce::Drawable> fuzzClipOff;
  std::unique_ptr<juce::Drawable> fuzzClipOn;

  std::unique_ptr<juce::Drawable> bypassOff;
  std::unique_ptr<juce::Drawable> bypassOff_Hover;
  std::unique_ptr<juce::Drawable> bypassOn;
  std::unique_ptr<juce::Drawable> bypassOn_Hover;


  std::unique_ptr<juce::Drawable> background;
  std::unique_ptr<juce::Drawable> uiForeground;
  Gui::HorizontalMeter inputHorizontalMeterL, inputHorizontalMeterR, outputHorizontalMeterL, outputHorizontalMeterR;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};
}  // namespace audio_plugin
