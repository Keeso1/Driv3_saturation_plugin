/*
  ==============================================================================

    HorizontalMeter.h
    Created: 12 Nov 2025 8:30:58am
    Author:  isacl

  ==============================================================================
*/

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace Gui {
    class HorizontalMeter : public juce::Component 
    {
    public:
        void paint(juce::Graphics& g) override
        {
            auto bounds = getLocalBounds().toFloat();

            g.setColour(juce::Colours::green.withBrightness(0.6f));
            g.fillRect(bounds);

            g.setColour(juce::Colours::green.withBrightness(0.4f));
            const auto scaledX = juce::jmap(level, -60.0f, 6.0f, 0.0f, static_cast<float>(getWidth()));
            g.fillRect(bounds.removeFromLeft(scaledX));

        }
        void setLevel(const float value) { level = value; }
    private:
        float level = -60.0f;
    };
}