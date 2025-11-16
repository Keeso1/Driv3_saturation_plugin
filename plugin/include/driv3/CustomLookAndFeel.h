/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 10 Nov 2025 2:00:00pm
    Author:  Your Name

  ==============================================================================
*/

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel();

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float minSliderPos, float maxSliderPos,
                          const juce::Slider::SliderStyle style, juce::Slider& slider) override;

private:
    std::unique_ptr<juce::Drawable> layer1;
    std::unique_ptr<juce::Drawable> fillLayer;
};
