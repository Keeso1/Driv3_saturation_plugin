/*
  ==============================================================================

    MixSliderLookAndFeel.cpp
    Created: 11 Nov 2025 3:30:50pm
    Author:  Your Name

  ==============================================================================
*/

#include "driv3/MixSliderLookAndFeel.h"
#include "BinaryData.h"

MixSliderLookAndFeel::MixSliderLookAndFeel()
{
    // Load the background SVG
    if (auto svgXml = juce::XmlDocument::parse(juce::String::createStringFromData(BinaryData::mix_slider_backgroundlayer_svg, BinaryData::mix_slider_backgroundlayer_svgSize))) {
        backgroundLayer = juce::Drawable::createFromSVG(*svgXml);
    }

    // Load the filllayer SVG
    if (auto svgXml = juce::XmlDocument::parse(juce::String::createStringFromData(BinaryData::mix_slider_filllayer_svg, BinaryData::mix_slider_filllayer_svgSize))) {
        fillLayer = juce::Drawable::createFromSVG(*svgXml);
    }

    // Load the thumb SVG
    if (auto svgXml = juce::XmlDocument::parse(juce::String::createStringFromData(BinaryData::mix_slider_thumb_svg, BinaryData::mix_slider_thumb_svgSize))) {
        thumb = juce::Drawable::createFromSVG(*svgXml);
    }
}

void MixSliderLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                                          float sliderPos, float minSliderPos, float maxSliderPos,
                                          const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    if (backgroundLayer)
    {
        backgroundLayer->drawWithin(g, slider.getLocalBounds().toFloat(), juce::RectanglePlacement(2 + 8), 1.0f);

        DBG("x: " << x << " " << "y: " << y << " " << "width: " << width << " " << "height: " << height); // result: x: 0 y: 9 width: 35 height: 177

    }

    const float relativeX = 31.0f / 71.0f;
    const float relativeY = 34.0f / 489.0f;
    const float relativeWidth = 40.0f / 71.0f;
    const float relativeHeight = 455.0f / 489.0f;

    auto fillBounds = juce::Rectangle<float>(x + width * relativeX,
        y + height * relativeY,
        width * relativeWidth,
        height * relativeHeight);

    if (fillLayer)
    {


        float proportion = 0.0f;
        const double range = slider.getMaximum() - slider.getMinimum();
        /*DBG("maximum= " << slider.getMaximum() << "minimum= " << slider.getMinimum());*/
        if (range > 0)
        {
            proportion = static_cast<float>((slider.getValue() - slider.getMinimum()) / range);
        }

        float visibleFillHeight = (fillBounds.getY() + fillBounds.getHeight()) * proportion;

        float fillTopY = fillBounds.getBottom() - visibleFillHeight;

        auto clipRect = juce::Rectangle<float>(fillBounds.getX(), fillTopY, fillBounds.getWidth(), visibleFillHeight);

        juce::Graphics::ScopedSaveState savedState(g);
        g.reduceClipRegion(clipRect.toNearestInt());

        fillLayer->drawWithin(g, fillBounds, juce::RectanglePlacement(2 + 8), 1.0f);

    }

    drawLinearSliderThumb(g, slider.getLocalBounds().getX(), static_cast<int>(fillBounds.getY()), slider.getLocalBounds().getWidth(), static_cast<int>(fillBounds.getHeight()), sliderPos, minSliderPos, maxSliderPos, style, slider);
}

void MixSliderLookAndFeel::drawLinearSliderThumb(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPos, float minSliderPos, float maxSliderPos,
    const juce::Slider::SliderStyle style, juce::Slider& slider) 
{
    juce::ignoreUnused(x, y, width, height, minSliderPos, maxSliderPos, style);
    if (thumb)
    {
        // Get the slider's full component bounds
        auto bounds = slider.getLocalBounds().toFloat();
        DBG("bounds: " << bounds.toString());

        // Define the thumb's width as a proportion of the component's width. Adjust as needed.
        const float thumbW = bounds.getWidth() * 0.2f;
        const float thumbH = thumbW * (thumb->getBounds().getHeight() / thumb->getBounds().getWidth()); // Maintain aspect ratio

        // Calculate the thumb's position.
        // Align its left side with the start of the fill layer's visual track.
        const float thumbX = bounds.getX();
        // Center it vertically on the slider's value position (sliderPos).
        const float thumbY = sliderPos;

        juce::Rectangle<float> thumbRect(thumbX, thumbY, thumbW, thumbH);
        thumb->drawWithin(g, thumbRect, juce::RectanglePlacement::stretchToFit, 1.0f);
    }
}
