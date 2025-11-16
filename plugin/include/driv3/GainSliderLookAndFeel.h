/*
  ==============================================================================

    GainSliderLookAndFeel.h
    Created: 13 Nov 2025 12:14:32pm
    Author:  isacl

  ==============================================================================
*/

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>

class GainSliderLookAndFeel : public juce::LookAndFeel_V3
{
public:
    GainSliderLookAndFeel();

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
    std::unique_ptr<juce::Drawable> thumb;
};
