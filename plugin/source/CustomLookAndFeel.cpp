/*
  ==============================================================================

    CustomLookAndFeel.cpp
    Created: 10 Nov 2025 2:00:00pm
    Author:  Your Name

  ==============================================================================

  TODO: Fix layer1 and 2 overlay
*/

#include "driv3/CustomLookAndFeel.h"
#include "BinaryData.h"

CustomLookAndFeel::CustomLookAndFeel()
{
    // Load the background SVG
    if (auto svgXml = juce::XmlDocument::parse(juce::String::createStringFromData(BinaryData::drive_slider_layer1_svg, BinaryData::drive_slider_layer1_svgSize))) {
        layer1 = juce::Drawable::createFromSVG(*svgXml);
    }

    // Load the fillLayer SVG
    if (auto svgXml = juce::XmlDocument::parse(juce::String::createStringFromData(BinaryData::drive_slider_layer2_svg, BinaryData::drive_slider_layer2_svgSize))) {
        fillLayer = juce::Drawable::createFromSVG(*svgXml);
    }
}

void CustomLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                                       float sliderPos, float minSliderPos, float maxSliderPos,
                                       const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    juce::ignoreUnused(sliderPos, minSliderPos, maxSliderPos, style);
    // 1. Draw the background layer first
    if (layer1)
    {
        layer1->drawWithin(g, juce::Rectangle<float>(static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height)),
                           juce::RectanglePlacement::fillDestination, 1.0f);
    }

    //2. If we have a fill layer, calculate its position and draw it clipped
    if (fillLayer)
    {
        // These relative values are CONSTANTS based on the original SVG dimensions.
        // This is the key to correct resizing.
        const float relativeX = 79.0f / 196.0f;
        const float relativeY = 44.0f / 515.0f;
        const float relativeWidth = 38.0f / 196.0f;
        const float relativeHeight = 458.0f / 515.0f;

        // Calculate the absolute bounds for the fill layer for THIS paint call
        auto fillBounds = juce::Rectangle<float>(static_cast<float>(x) + static_cast<float>(width) * relativeX,
                                                 static_cast<float>(y) + static_cast<float>(height) * relativeY,
                                                 static_cast<float>(width) * relativeWidth,
                                                 static_cast<float>(height) * relativeHeight);
        
        float proportion = 0.0f;
        const double range = slider.getMaximum() - slider.getMinimum();
        /*DBG("maximum= " << slider.getMaximum() << "minimum= " << slider.getMinimum());*/
        if (range > 0)
        {
            proportion = static_cast<float>((slider.getValue() - slider.getMinimum()) / range);
        }
        /*DBG("proportion= " << proportion);
        DBG("fillBounds " << "aspectratio: " << fillBounds.getAspectRatio() << "x:" << fillBounds.getX() << "y:" << fillBounds.getY() << "w:" << fillBounds.getWidth() << "h: " << fillBounds.getHeight());*/
        /*double proportion = slider.valueToProportionOfLength(sliderPos);*/
        // Calculate the height of the VISIBLE part of the fill
        auto visibleFillHeight = (fillBounds.getY() + fillBounds.getHeight()) * proportion;

        // The fill should grow from the bottom up
        auto fillTopY = fillBounds.getBottom() - visibleFillHeight;

        // Create the rectangle for the visible part of the fill
        auto clipRect = juce::Rectangle<float>(fillBounds.getX(), fillTopY, fillBounds.getWidth(), visibleFillHeight);

        // Save the graphics state, set the clipping region, and restore it automatically
        juce::Graphics::ScopedSaveState savedState(g);
        g.reduceClipRegion(clipRect.toNearestInt());

        // Draw the entire fill layer within its calculated bounds;
        // it will be clipped automatically by the region we just set.
        fillLayer->drawWithin(g, fillBounds, juce::RectanglePlacement::fillDestination, 1.0f);
    }
}
