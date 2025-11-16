/*
  ==============================================================================

    GainSliderLookAndFeel.cpp
    Created: 13 Nov 2025 12:14:32pm
    Author:  isacl

  ==============================================================================
*/

#include "driv3/GainSliderLookAndFeel.h"
#include "BinaryData.h"

GainSliderLookAndFeel::GainSliderLookAndFeel()
{
    // Load the background SVG
    if (auto svgXml = juce::XmlDocument::parse(juce::String::createStringFromData(BinaryData::gain_slider_svg, BinaryData::gain_slider_svgSize))) {
        backgroundLayer = juce::Drawable::createFromSVG(*svgXml);
    }

    // Load the thumb SVG
    if (auto svgXml = juce::XmlDocument::parse(juce::String::createStringFromData(BinaryData::gain_slider_thumb_svg, BinaryData::gain_slider_thumb_svgSize))) {
        thumb = juce::Drawable::createFromSVG(*svgXml);
    }
}

void GainSliderLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPos, float minSliderPos, float maxSliderPos,
    const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    if (backgroundLayer)
    {
        backgroundLayer->drawWithin(g, slider.getLocalBounds().toFloat(), juce::RectanglePlacement(2 + 8), 1.0f);
    }

    const float relativeX = 31.0f / 71.0f;
    const float relativeY = 34.0f / 489.0f;
    const float relativeWidth = 40.0f / 71.0f;
    const float relativeHeight = 455.0f / 489.0f;

    auto fillBounds = juce::Rectangle<float>(x + width * relativeX,
        y + height * relativeY,
        width * relativeWidth,
        height * relativeHeight);

    drawLinearSliderThumb(g, static_cast<int>(fillBounds.getX()), slider.getLocalBounds().getY(), static_cast<int>(fillBounds.getWidth()), slider.getLocalBounds().getHeight(), sliderPos, minSliderPos, maxSliderPos, style, slider);
}

void GainSliderLookAndFeel::drawLinearSliderThumb(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPos, float minSliderPos, float maxSliderPos,
    const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    juce::ignoreUnused(x, y, width, height, minSliderPos, maxSliderPos, style);
    if (thumb)
    {
        auto bounds = slider.getLocalBounds().toFloat();

        const float thumbW = bounds.getHeight() * 0.2f;
        const float thumbH = thumbW * (static_cast<float>(thumb->getBounds().getHeight()) / static_cast<float>(thumb->getBounds().getWidth()));


        const float thumbX = sliderPos;
        const float thumbY = bounds.getBottom() - thumbH;

        juce::Rectangle<float> thumbRect(thumbX, thumbY, thumbW, thumbH);
        thumb->drawWithin(g, thumbRect, juce::RectanglePlacement::yBottom, 1.0f);
    }
}