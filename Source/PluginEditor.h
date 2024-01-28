/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "FxmeLookAndFeel.h"

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

    FxmeKnobLookAndFeel fxmeKnobLookAndFeel;

    juce::Slider gain;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
    juce::Label gainLabel{"gainLabel", "Gain"};

    juce::Slider level;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> levelAttachment;
    juce::Label levelLabel{"levelLabel", "Level"};

    juce::Slider porta;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> portaAttachment;
    juce::Label portaLabel{"portaLabel", "Porta"};

    juce::Slider stringsFBGainOn;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> stringsFBGainOnAttachment;
    juce::Label stringsFBGainOnLabel{"stringsFBGainOnLabel", "FB Gain"};
    juce::Slider stringsFBFreqOn;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> stringsFBFreqOnAttachment;
    juce::Label stringsFBFreqOnLabel{"stringsFBFreqOnLabel", "FB Freq"};
    juce::Slider stringsLevelOn;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> stringsLevelOnAttachment;
    juce::Label stringsLevelOnLabel{"stringsLevelOnLabel", "Level"};

    juce::Slider stringsFBGainOff;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> stringsFBGainOffAttachment;
    juce::Label stringsFBGainOffLabel{"stringsFBGainOffLabel", "FB Gain"};
    juce::Slider stringsFBFreqOff;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> stringsFBFreqOffAttachment;
    juce::Label stringsFBFreqOffLabel{"stringsFBFreqOffLabel", "FB Freq"};
    juce::Slider stringsLevelOff;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> stringsLevelOffAttachment;
    juce::Label stringsLevelOffLabel{"stringsLevelOffLabel", "Level"};

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

    juce::Slider samplerAttack;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> samplerAttackAttachment;
    juce::Label samplerAttackLabel{"samplerAttackLabel", "A"};
    juce::Slider samplerDecay;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> samplerDecayAttachment;
    juce::Label samplerDecayLabel{"samplerDecayLabel", "D"};
    juce::Slider samplerSustain;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> samplerSustainAttachment;
    juce::Label samplerSustainLabel{"samplerAttackLabel", "S"};
    juce::Slider samplerRelease;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> samplerReleaseAttachment;
    juce::Label samplerReleaseLabel{"samplerReleaseLabel", "R"};
    juce::Slider samplerFreq;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> samplerFreqAttachment;
    juce::Label samplerFreqLabel{"samplerFreqLabel", "Freq"};
    juce::Slider samplerLevel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> samplerLevelAttachment;
    juce::Label samplerLevelLabel{"samplerLevelLabel", "Level"};
    juce::ComboBox waveComboBox;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveComboBoxAttachment;
    juce::Label waveLabel{"waveLabel", "Wave type"};
    juce::ToggleButton loopButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> loopButtonAttachment;
    juce::Label loopLabel{"loopLabel", "Loop"};

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
    juce::Slider noiseLPFreq;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> noiseLPFreqAttachment;
    juce::Label noiseLPFreqLabel{"noiseLPFreqLabel", "LPFreq"};
    juce::Slider noiseHPFreq;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> noiseHPFreqAttachment;
    juce::Label noiseHPFreqLabel{"noiseHPFreqLabel", "HPFreq"};

    juce::Slider noiseLevel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> noiseLevelAttachment;
    juce::Label noiseLevelLabel{"noiseLevelLabel", "Level"};

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
    juce::Slider crackDensity;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> crackDensityAttachment;
    juce::Label crackDensityLabel{"crackDensityLabel", "Density"};
    juce::Slider crackFreq;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> crackFreqAttachment;
    juce::Label crackFreqLabel{"crackFreqLabel", "Freq"};
    juce::Slider crackLevel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> crackLevelAttachment;
    juce::Label crackLevelLabel{"crackLevelLabel", "Level"};
    
    juce::Slider stringsInPos;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> stringsInPosAttachment;
    juce::Label stringsInPosLabel{"inPosLabel", "Input Position"};
    juce::Slider stringsOutPos;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> stringsOutPosAttachment;
    juce::Label stringsOutPosLabel{"outPosLabel", "Output Position"};

    juce::Slider stringsCoupling;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> stringsCouplingAttachment;
    juce::Label stringsCouplingLabel{"couplingLabel", "Coupling"};

    juce::Slider string1Level;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> string1LevelAttachment;
    juce::Label string1LevelLabel{"string1LevelLabel", "Level"};
    juce::Slider string1FreqCoarse;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> string1FreqCoarseAttachment;
    juce::Label string1FreqCoarseLabel{"string1FreqCoarseLabel", "Coarse"};
    juce::Slider string1FreqFine;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> string1FreqFineAttachment;
    juce::Label string1FreqFineLabel{"string1FreqFineLabel", "Fine"};

    juce::Slider string2Level;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> string2LevelAttachment;
    juce::Label string2LevelLabel{"string2LevelLabel", "Level"};
    juce::Slider string2FreqCoarse;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> string2FreqCoarseAttachment;
    juce::Label string2FreqCoarseLabel{"string2FreqCoarseLabel", "Coarse"};
    juce::Slider string2FreqFine;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> string2FreqFineAttachment;
    juce::Label string2FreqFineLabel{"string2FreqFineLabel", "Fine"};

    juce::Slider velocitySampleLevel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> velocitySampleLevelAttachment;
    juce::Label velocitySampleLevelLabel{"velocitySampleLevelLabel", "Level"};
    juce::Slider velocitySampleFreq;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> velocitySampleFreqAttachment;
    juce::Label velocitySampleFreqLabel{"velocitySampleFreqLabel", "Freq"};

    juce::Slider velocityNoiseLevel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> velocityNoiseLevelAttachment;
    juce::Label velocityNoiseLevelLabel{"velocityNoiseLevelLabel", "Level"};
    juce::Slider velocityNoiseFreq;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> velocityNoiseFreqAttachment;
    juce::Label velocityNoiseFreqLabel{"velocityNoiseFreqLabel", "Freq"};

    juce::Slider velocityCrackLevel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> velocityCrackLevelAttachment;
    juce::Label velocityCrackLevelLabel{"velocityCrackLevelLabel", "Level"};
    juce::Slider velocityCrackFreq;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> velocityCrackFreqAttachment;
    juce::Label velocityCrackFreqLabel{"velocityCrackFreqLabel", "Freq"};


    // juce::Slider velocityLevel;
    // std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> velocityLevelAttachment;
    // juce::Label velocityLevelLabel{"string2LevelLabel", "Level"};
    // juce::Slider velocitySampleFreq;
    // std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> velocitySampleFreqAttachment;
    // juce::Label velocitySampleFreqLabel{"string2FreqCoarseLabel", "Sample freq"};
    // juce::Slider velocityNoiseFreq;
    // std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> velocityNoiseFreqAttachment;
    // juce::Label velocityNoiseFreqLabel{"string2FreqFineLabel", "Noise freq"};

    juce::Label emptyLabel{"emptyLabel", ""};
    
    void addKnob(juce::Slider &slider, juce::Label &label, juce::Colour fillCol, juce::Colour outlineCol);
    void addVSlider(juce::Slider &slider, juce::Label &label, juce::Colour fillCol, juce::Colour outlineCol);
    void addHSlider(juce::Slider &slider, juce::Label &label, juce::Colour fillCol, juce::Colour outlineCol);
    //void addAndConnectKnobLabel(juce::Slider &slider, juce::Label &label);
    void drawBoxWithTitleOnTop(juce::Graphics& g,
                                const juce::String title,
                                const juce::Colour baseColour,
                                const float x,
                                const float y,
                                const float wx,
                                const float wy,
                                const float bandHeight);

    void drawBoxWithTitleOnLeft(juce::Graphics& g,
                                const juce::String title,
                                const juce::Colour baseColour,
                                const float x,
                                const float y,
                                const float wx,
                                const float wy,
                                const float bandWidth);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MySynthAudioProcessorEditor)
};
