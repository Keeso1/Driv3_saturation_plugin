#pragma once
#include <juce_dsp/juce_dsp.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace audio_plugin {
    class AnalyserComponent : public juce::Component, public juce::AudioSource {
    public:
        enum {
            fftOrder = 11,
            fftSize = 1 << fftOrder,
            scopeSize = 512
        };

        AnalyserComponent();

        void paint(juce::Graphics& g) override;

        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        void releaseResources() override;
        void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;

        void pushNextSampleIntoFifo (float sample) noexcept;
        void drawNextFrameOfSpectrum();
        void drawFrame (juce::Graphics& g);

        std::atomic<bool> nextFFTBlockReady { false };

    private:
        juce::dsp::FFT forwardFFT;
        juce::dsp::WindowingFunction<float> window;
        float fifo[fftSize];
        float fftData[2 * fftSize];
        int fifoIndex = 0;
        float scopeData[scopeSize];
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AnalyserComponent)
    };
}
