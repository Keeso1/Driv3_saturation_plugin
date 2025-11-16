/*
  ==============================================================================

    MixSliderLookAndFeel.h
    Created: 11 Nov 2025 3:30:50pm
    Author:  Your Name

  ==============================================================================
*/

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>

class MixSliderLookAndFeel : public juce::LookAndFeel_V3
{
public:
    MixSliderLookAndFeel();

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float minSliderPos, float maxSliderPos,
                          const juce::Slider::SliderStyle style, juce::Slider& slider) override;

    void drawLinearSliderThumb(
        juce::Graphics& g,
        int	x,
        int	y,
        int	width,
        int	height,
        float	sliderPos,
        float	minSliderPos,
        float	maxSliderPos,
        juce::Slider::SliderStyle style,
        juce::Slider& slider) override;

private:
    std::unique_ptr<juce::Drawable> backgroundLayer;
    std::unique_ptr<juce::Drawable> fillLayer;
    std::unique_ptr<juce::Drawable> thumb;
};
