/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MySynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MySynthAudioProcessorEditor (MySynthAudioProcessor&);
    ~MySynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    MySynthAudioProcessor& audioProcessor;

    juce::Slider gain;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
    juce::Label gainLabel{"gainLabel", "Gain"};
    juce::Slider level;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> levelAttachment;
    juce::Label levelLabel{"levelLabel", "Level"};

    juce::Slider stringsAttack;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> stringsAttackAttachment;
    juce::Label stringsAttackLabel{"stringsAttackLabel", "A"};
    juce::Slider stringsDecay;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> stringsDecayAttachment;
    juce::Label stringsDecayLabel{"stringsDecayLabel", "D"};
    juce::Slider stringsSustain;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> stringsSustainAttachment;
    juce::Label stringsSustainLabel{"stringsAttackLabel", "S"};
    juce::Slider stringsRelease;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> stringsReleaseAttachment;
    juce::Label stringsReleaseLabel{"stringsReleaseLabel", "R"};

    juce::Slider samplerLevel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> samplerLevelAttachment;
    juce::Label samplerLevelLabel{"samplerLevelLabel", "Level"};
    juce::Slider samplerFreq;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> samplerFreqAttachment;
    juce::Label samplerFreqLabel{"samplerFreqLabel", "Freq"};
    juce::ComboBox waveComboBox;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveComboBoxAttachment;
    juce::Label waveLabel{"waveLabel", "Wave type"};
    juce::ToggleButton loopButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> loopButtonAttachment;
    juce::Label loopLabel{"loopLabel", "Loop"};

    juce::Slider noiseLevel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> noiseLevelAttachment;
    juce::Label noiseLevelLabel{"noiseLevelLabel", "Level"};
    juce::Slider noiseFreq;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> noiseFreqAttachment;
    juce::Label noiseFreqLabel{"noiseFreqLabel", "Freq"};
    juce::Slider noiseAttack;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> noiseAttackAttachment;
    juce::Label noiseAttackLabel{"noiseAttackLabel", "A"};
    juce::Slider noiseDecay;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> noiseDecayAttachment;
    juce::Label noiseDecayLabel{"noiseDecayLabel", "D"};
    juce::Slider noiseSustain;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> noiseSustainAttachment;
    juce::Label noiseSustainLabel{"noiseAttackLabel", "S"};
    juce::Slider noiseRelease;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> noiseReleaseAttachment;
    juce::Label noiseReleaseLabel{"noiseReleaseLabel", "R"};

    juce::Slider crackLevel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> crackLevelAttachment;
    juce::Label crackLevelLabel{"crackLevelLabel", "Level"};
    juce::Slider crackDensity;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> crackDensityAttachment;
    juce::Label crackDensityLabel{"crackDensityLabel", "Density"};
    juce::Slider crackFreq;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> crackFreqAttachment;
    juce::Label crackFreqLabel{"crackFreqLabel", "Freq"};
    juce::Slider crackAttack;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> crackAttackAttachment;
    juce::Label crackAttackLabel{"crackAttackLabel", "A"};
    juce::Slider crackDecay;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> crackDecayAttachment;
    juce::Label crackDecayLabel{"crackDecayLabel", "D"};
    juce::Slider crackSustain;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> crackSustainAttachment;
    juce::Label crackSustainLabel{"crackAttackLabel", "S"};
    juce::Slider crackRelease;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> crackReleaseAttachment;
    juce::Label crackReleaseLabel{"crackReleaseLabel", "R"};

    juce::Slider stringsFBGainOn;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> stringsFBGainOnAttachment;
    juce::Label stringsFBGainOnLabel{"stringsFBGainOnLabel", "Strings FBGain On"};
    juce::Slider stringsFBFreqOn;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> stringsFBFreqOnAttachment;
    juce::Label stringsFBFreqOnLabel{"stringsFBFreqOnLabel", "Strings FBFreq On"};

    juce::Slider stringsFBGainOff;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> stringsFBGainOffAttachment;
    juce::Label stringsFBGainOffLabel{"stringsFBGainOffLabel", "Strings FBGain Off"};
    juce::Slider stringsFBFreqOff;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> stringsFBFreqOffAttachment;
    juce::Label stringsFBFreqOffLabel{"stringsFBFreqOffLabel", "Strings FBFreq Off"};

    juce::Slider inPos;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inPosAttachment;
    juce::Label inPosLabel{"inPosLabel", "Input Position"};
    juce::Slider outPos;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outPosAttachment;
    juce::Label outPosLabel{"outPosLabel", "Output Position"};

    juce::Slider coupling;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> couplingAttachment;
    juce::Label couplingLabel{"couplingLabel", "Coupling Position"};

    juce::Slider string1Level;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> string1LevelAttachment;
    juce::Label string1LevelLabel{"string1LevelLabel", "String 1 Level"};
    juce::Slider string1FreqCoarse;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> string1FreqCoarseAttachment;
    juce::Label string1FreqCoarseLabel{"string1FreqCoarseLabel", "String 1 FreqCoarse"};
    juce::Slider string1FreqFine;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> string1FreqFineAttachment;
    juce::Label string1FreqFineLabel{"string1FreqFineLabel", "String 1 FreqFine"};

    juce::Slider string2Level;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> string2LevelAttachment;
    juce::Label string2LevelLabel{"string2LevelLabel", "String 1 Level"};
    juce::Slider string2FreqCoarse;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> string2FreqCoarseAttachment;
    juce::Label string2FreqCoarseLabel{"string2FreqCoarseLabel", "String 1 FreqCoarse"};
    juce::Slider string2FreqFine;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> string2FreqFineAttachment;
    juce::Label string2FreqFineLabel{"string2FreqFineLabel", "String 1 FreqFine"};



    void addController(juce::Slider &slider, juce::Slider::SliderStyle style, juce::Colour fillCol, juce::Colour outlineCol);
    void addAndConnectLabel(juce::Slider &slider, juce::Label &label);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MySynthAudioProcessorEditor)
};
