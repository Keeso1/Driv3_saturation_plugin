#include "driv3/PluginProcessor.h"
#include "driv3/PluginEditor.h"

namespace audio_plugin {
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
    : AudioProcessor(
          BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
)
      {
  apvts = std::make_unique<juce::AudioProcessorValueTreeState>(*this, nullptr, "Parameters", createParameters());
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor() {}

const juce::String AudioPluginAudioProcessor::getName() const {
  return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool AudioPluginAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const {
  return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms() {
  return 1;  // NB: some hosts don't cope very well if you tell them there are 0
             // programs, so this should be at least 1, even if you're not
             // really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram() {
  return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram(int index) {
  juce::ignoreUnused(index);
}

const juce::String AudioPluginAudioProcessor::getProgramName(int index) {
  juce::ignoreUnused(index);
  return {};
}

void AudioPluginAudioProcessor::changeProgramName(int index,
                                                  const juce::String& newName) {
  juce::ignoreUnused(index, newName);
}

void AudioPluginAudioProcessor::prepareToPlay(double sampleRate,
                                              int samplesPerBlock) {
  // Use this method as the place to do any pre-playback
  // initialisation that you need..
  analyserComponent.prepareToPlay(samplesPerBlock, sampleRate);
}

void AudioPluginAudioProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
  analyserComponent.releaseResources();
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported(
    const BusesLayout& layouts) const {
#if JucePlugin_IsMidiEffect
  juce::ignoreUnused(layouts);
  return true;
#else
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  // Some plugin hosts, such as certain GarageBand versions, will only
  // load plugins that support stereo bus layouts.
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

  // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif

  return true;
#endif
}

void AudioPluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                             juce::MidiBuffer& midiMessages) {
  juce::ignoreUnused(midiMessages);

  juce::ScopedNoDenormals noDenormals;
  auto totalNumInputChannels  = getTotalNumInputChannels();
  auto totalNumOutputChannels = getTotalNumOutputChannels();

  for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
      buffer.clear (i, 0, buffer.getNumSamples());


  // Get the drive parameter value from the APVTS
  auto driveDb = apvts->getRawParameterValue("DRIVE")->load();
  auto saturationType = apvts->getRawParameterValue("SAT_TYPE")->load();
  auto mixValue = apvts->getRawParameterValue("MIX")->load();
  auto mixLevel = mixValue / 100.0f;
  auto outputDb = apvts->getRawParameterValue("OUTPUT")->load();
  auto inputDb = apvts->getRawParameterValue("INPUT")->load();
  auto bypass = apvts->getRawParameterValue("BYPASS")->load();

  if (bypass < 0.5f)
  {
      // Convert the dB value to a linear gain multiplier
      auto driveGain = juce::Decibels::decibelsToGain(driveDb);
      auto outputGain = juce::Decibels::decibelsToGain(outputDb);
      auto inputGain = juce::Decibels::decibelsToGain(inputDb);


      for (int channel = 0; channel < totalNumInputChannels; ++channel)
      {
          buffer.applyGain(channel, 0, buffer.getNumSamples(), inputGain);

          if (channel == 0) {
              inputRmsLevelLeft = juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
          }
          if (channel == 1) {
              inputRmsLevelRight = juce::Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, buffer.getNumSamples()));
          }
          auto* channelData = buffer.getWritePointer (channel);

          for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
          {
              float drySample = channelData[sample];
              float wetSample = drySample; //Apply inputgain TODO: Check if this still updates the RMSlevelreader.

              switch (static_cast<int>(saturationType)) {
                  case 0: // Soft
                  {
                      wetSample = std::tanh(wetSample * driveGain);
                      break;
                  }
              
                  case 1: 
                  {
                      wetSample = juce::jlimit(-1.0f, 1.0f, wetSample * driveGain); // Hard
                      break;
                  }
              
                  case 2: 
                  {
                      float drive_var = driveGain;
                      if (drive_var == 0.00f) drive_var = 0.01f;
                      wetSample = (drive_var * wetSample) / (drive_var * std::abs(wetSample) + 1); // Custom fuzz: f(x) = (a * x) / (a * |x| + 1)
                      break;
                  }
              
              }
              channelData[sample] = ((drySample * (1.0f - mixLevel)) + (wetSample * mixLevel)) * outputGain;
          }
          if (channel == 0) {
              outputRmsLevelLeft = juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
          }
          if (channel == 1) {
              outputRmsLevelRight = juce::Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, buffer.getNumSamples()));
          }
      }
  }
  else
  {
      // Bypassed: just pass audio through and update meters
      for (int channel = 0; channel < totalNumInputChannels; ++channel)
      {
          if (channel == 0) {
              inputRmsLevelLeft = juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
              outputRmsLevelLeft = inputRmsLevelLeft;
          }
          if (channel == 1) {
              inputRmsLevelRight = juce::Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, buffer.getNumSamples()));
              outputRmsLevelRight = inputRmsLevelRight;
          }
      }
  }
  analyserComponent.getNextAudioBlock(juce::AudioSourceChannelInfo(buffer));
}

bool AudioPluginAudioProcessor::hasEditor() const {
  return true;  // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor() {
  return new AudioPluginAudioProcessorEditor(*this);
}

void AudioPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = apvts->copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void AudioPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (apvts->state.getType()))
            apvts->replaceState (juce::ValueTree::fromXml (*xmlState));
}

std::tuple<float,float> AudioPluginAudioProcessor::getRmsValue(const int channel) const {
    jassert(channel == 0 || channel == 1);
    if (channel == 0) {
        return std::make_tuple(inputRmsLevelLeft, outputRmsLevelLeft);
    }
    if (channel == 1) {
        return std::make_tuple(inputRmsLevelRight, outputRmsLevelRight);
    }
    return std::make_tuple(0.0f, 0.0f);
}

AnalyserComponent& AudioPluginAudioProcessor::getAnalyserComponent()
{
    return analyserComponent;
}

juce::AudioProcessorValueTreeState::ParameterLayout AudioPluginAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>("DRIVE", "Drive", juce::NormalisableRange<float>(0.0f, 24.0f), 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MIX", "Mix", juce::NormalisableRange<float>(0.0f, 100.0f), 100.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("INPUT", "Input", juce::NormalisableRange<float>(-24.0f, 24.0f), 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OUTPUT", "Output", juce::NormalisableRange<float>(-24.0f, 24.0f), 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("SAT_TYPE", "Saturation Type", juce::StringArray{ "a", "b", "c" }, 0));
    params.push_back(std::make_unique<juce::AudioParameterBool>("BYPASS", "Bypass", false));

    return { params.begin(), params.end() };
}

} // namespace audio_plugin

// This creates new instances of the plugin.
// This function definition must be in the global namespace.
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
  return new audio_plugin::AudioPluginAudioProcessor();
}
