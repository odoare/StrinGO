/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MySynthAudioProcessorEditor::MySynthAudioProcessorEditor (MySynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addController(level, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(level, levelLabel);
    levelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Output Level",level);

    addController(gain, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(gain, gainLabel);
    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Output Gain",gain);
    
    // String waveguide enveloppe
    addController(stringsAttack, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(stringsAttack, stringsAttackLabel);
    stringsAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"A",stringsAttack);
    addController(stringsDecay, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(stringsDecay, stringsDecayLabel);
    stringsDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"D",stringsDecay);
    addController(stringsSustain, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(stringsSustain, stringsSustainLabel);
    stringsSustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"S",stringsSustain);
    addController(stringsRelease, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(stringsRelease, stringsReleaseLabel);
    stringsReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"R",stringsRelease);
    
    addController(samplerFreq, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(samplerFreq, samplerFreqLabel);
    samplerFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Sampler Lowpass",samplerFreq);
    addController(samplerLevel, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(samplerLevel, samplerLevelLabel);
    samplerLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Sampler Level",samplerLevel);

    // // Sample wave combo box
    juce::StringArray choices;
    choices.addArray(WAVECHOICES);
    waveComboBox.addItemList(choices,1);
    waveComboBoxAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts,"Attack Sample",waveComboBox);
    waveLabel.attachToComponent(&waveComboBox,false);
    addAndMakeVisible(waveComboBox);
    addAndMakeVisible(waveLabel);
    // waveComboBox.onChange = updateFunc;

    addAndMakeVisible(loopButton);
    loopButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"Loop",loopButton);
    addAndMakeVisible(loopLabel);
    loopLabel.attachToComponent(&loopButton,false);

    
    // Noise enveloppe
    addController(noiseAttack, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(noiseAttack, noiseAttackLabel);
    noiseAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"AN",noiseAttack);
    addController(noiseDecay, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(noiseDecay, noiseDecayLabel);
    noiseDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"DN",noiseDecay);
    addController(noiseSustain, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(noiseSustain, noiseSustainLabel);
    noiseSustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"SN",noiseSustain);
    addController(noiseRelease, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(noiseRelease, noiseReleaseLabel);
    noiseReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"RN",noiseRelease);
    addController(noiseFreq, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(noiseFreq, noiseFreqLabel);
    noiseFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Noise Freq",noiseFreq);
    addController(noiseLevel, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(noiseLevel, noiseLevelLabel);
    noiseLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Noise Level",noiseLevel);

    // crack enveloppe
    addController(crackAttack, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(crackAttack, crackAttackLabel);
    crackAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"AC",crackAttack);
    addController(crackDecay, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(crackDecay, crackDecayLabel);
    crackDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"DC",crackDecay);
    addController(crackSustain, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(crackSustain, crackSustainLabel);
    crackSustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"SC",crackSustain);
    addController(crackRelease, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(crackRelease, crackReleaseLabel);
    crackReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"RC",crackRelease);
    addController(crackFreq, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(crackFreq, crackFreqLabel);
    crackFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Crack Freq",crackFreq);
    addController(crackLevel, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(crackLevel, crackLevelLabel);
    crackLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Crack Level",crackLevel);
    addController(crackDensity, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(crackDensity, crackDensityLabel);
    crackDensityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Crack Density",crackDensity);

    // Waveguides feedback
    addController(stringsFBGainOn, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(stringsFBGainOn, stringsFBGainOnLabel);
    stringsFBGainOnAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Fb Gain On",stringsFBGainOn);
    addController(stringsFBFreqOn, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(stringsFBFreqOn, stringsFBFreqOnLabel);
    stringsFBFreqOnAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Fb Freq On",stringsFBFreqOn);

    addController(stringsFBGainOff, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(stringsFBGainOff, stringsFBGainOffLabel);
    stringsFBGainOffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Fb Gain Off",stringsFBGainOff);
    addController(stringsFBFreqOff, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(stringsFBFreqOff, stringsFBFreqOffLabel);
    stringsFBFreqOffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Fb Freq Off",stringsFBFreqOff);


    setSize (900, 600);
}

MySynthAudioProcessorEditor::~MySynthAudioProcessorEditor()
{
}

//==============================================================================
void MySynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    static const float border = 0.01;
    float uxb = border*getWidth();
    float uyb = border*getHeight();
    auto ux = (1-2*border)*getWidth()/36;
    auto uy = (1-2*border)*getHeight()/22;

    auto diagonale = (getLocalBounds().getTopLeft() - getLocalBounds().getBottomRight()).toFloat();
    auto length = diagonale.getDistanceFromOrigin();
    auto perpendicular = diagonale.rotatedAboutOrigin (juce::degreesToRadians (90.0f)) / length;
    auto height = float (getWidth() * getHeight()) / length;
    // auto bluegreengrey = juce::Colour::fromFloatRGBA(0.17f,0.22f,0.27f,1.0f);
    auto bluegreengrey = juce::Colour::fromFloatRGBA (0.15f, 0.15f, 0.25f, 1.0f);
    juce::ColourGradient grad (bluegreengrey.darker().darker().darker(), perpendicular * height,
                           bluegreengrey, perpendicular * -height, false);
    g.setGradientFill(grad);
    g.fillAll();

}

void MySynthAudioProcessorEditor::resized()
{
    static const float border = 0.01;
    float uxb = border*getWidth();
    float uyb = border*getHeight();
    auto ux = (1-2*border)*getWidth()/36;
    auto uy = (1-2*border)*getHeight()/22;
    
    gain.setBounds(uxb+28*ux,uyb+16*uy,4*ux,4*uy);
    level.setBounds(uxb+32*ux,uyb+16*uy,4*ux,4*uy);

    float ex = uxb+16*ux;
    float ey = uyb+6*uy;
    samplerLevel.setBounds(ex,ey,4*ux,4*uy);
    samplerFreq.setBounds(ex+4*ux,ey,4*ux,4*uy);
    waveComboBox.setBounds(ex+8*ux,ey,4*ux,1*uy);
    loopButton.setBounds(ex+9*ux,ey+3*uy,4*ux,1*uy);
    
    ex = uxb;
    ey = uyb+6*uy;
    stringsAttack.setBounds(ex,ey,4*ux,4*uy);
    stringsDecay.setBounds(ex+4*ux,ey,4*ux,4*uy);
    stringsSustain.setBounds(ex+8*ux,ey,4*ux,4*uy);
    stringsRelease.setBounds(ex+12*ux,ey,4*ux,4*uy);

    ex = uxb;
    ey = uyb+11*uy;
    crackAttack.setBounds(ex,ey,4*ux,4*uy);
    crackDecay.setBounds(ex+4*ux,ey,4*ux,4*uy);
    crackSustain.setBounds(ex+8*ux,ey,4*ux,4*uy);
    crackRelease.setBounds(ex+12*ux,ey,4*ux,4*uy);
    crackLevel.setBounds(ex+16*ux,ey,4*ux,4*uy);
    crackFreq.setBounds(ex+20*ux,ey,4*ux,4*uy);
    crackDensity.setBounds(ex+24*ux,ey,4*ux,4*uy);

    ex = uxb;
    ey = uyb+16*uy;
    noiseAttack.setBounds(ex,ey,4*ux,4*uy);
    noiseDecay.setBounds(ex+4*ux,ey,4*ux,4*uy);
    noiseSustain.setBounds(ex+8*ux,ey,4*ux,4*uy);
    noiseRelease.setBounds(ex+12*ux,ey,4*ux,4*uy);
    noiseLevel.setBounds(ex+16*ux,ey,4*ux,4*uy);
    noiseFreq.setBounds(ex+20*ux,ey,4*ux,4*uy);
    
}

void MySynthAudioProcessorEditor::addController(juce::Slider& slider,
                                                    juce::Slider::SliderStyle style,
                                                    juce::Colour fillCol,
                                                    juce::Colour outlineCol)
{
  slider.setSliderStyle(style);
  slider.setTextBoxStyle(juce::Slider::TextBoxBelow,true,100,20);
  slider.setTextBoxIsEditable(true);
  slider.setColour(juce::Slider::thumbColourId, fillCol);
  slider.setColour(juce::Slider::rotarySliderFillColourId, fillCol);
  slider.setColour(juce::Slider::trackColourId, outlineCol);
  slider.setColour(juce::Slider::backgroundColourId, outlineCol);
  slider.setColour(juce::Slider::rotarySliderOutlineColourId, outlineCol);
  addAndMakeVisible(slider);
}

void MySynthAudioProcessorEditor::addAndConnectLabel(juce::Slider& slider,
                                                juce::Label& label)
{
  addAndMakeVisible(label);
  label.setJustificationType(juce::Justification::centred);
  label.attachToComponent(&slider,false);
}
