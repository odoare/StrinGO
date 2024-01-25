/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define SAMPLERCOLOR juce::Colours::red
#define NOISECOLOR juce::Colours::green
#define CRACKSCOLOR juce::Colours::blue
#define MASTERCOLOR juce::Colours::yellow

//==============================================================================
MySynthAudioProcessorEditor::MySynthAudioProcessorEditor (MySynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addController(gain, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, MASTERCOLOR,juce::Colours::black);
    addAndConnectLabel(gain, gainLabel);
    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Output Gain",gain);
    // gain.setPopupDisplayEnabled(true,true,nullptr,1000);

    addController(level, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, MASTERCOLOR,juce::Colours::black);
    // addAndConnectLabel(level, levelLabel);
    levelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Output Level",level);

    addController(porta, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, MASTERCOLOR,juce::Colours::black);
    addAndConnectLabel(porta, portaLabel);
    portaAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Porta time",porta);

    // Waveguides
    addController(stringsFBGainOn, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(stringsFBGainOn, stringsFBGainOnLabel);
    stringsFBGainOnAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Fb Gain on",stringsFBGainOn);
    addController(stringsFBFreqOn, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(stringsFBFreqOn, stringsFBFreqOnLabel);
    stringsFBFreqOnAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Fb Freq on",stringsFBFreqOn);
    addController(stringsLevelOn, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(stringsLevelOn, stringsLevelOnLabel);
    stringsLevelOnAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Level on",stringsLevelOn);

    addController(stringsFBGainOff, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(stringsFBGainOff, stringsFBGainOffLabel);
    stringsFBGainOffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Fb Gain off",stringsFBGainOff);
    addController(stringsFBFreqOff, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(stringsFBFreqOff, stringsFBFreqOffLabel);
    stringsFBFreqOffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Fb Freq off",stringsFBFreqOff);
    addController(stringsLevelOff, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(stringsLevelOff, stringsLevelOffLabel);
    stringsLevelOffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Level off",stringsLevelOff);

    addController(stringsInPos, juce::Slider::SliderStyle::LinearHorizontal, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(stringsInPos, stringsInPosLabel);
    stringsInPosAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"In Pos",stringsInPos);
    stringsInPos.setTextBoxStyle(juce::Slider::TextBoxLeft,true,50,20);
    stringsInPos.setTextBoxIsEditable(true);

    addController(stringsOutPos, juce::Slider::SliderStyle::LinearHorizontal, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(stringsOutPos, stringsOutPosLabel);
    stringsOutPosAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Out Pos",stringsOutPos);
    addController(stringsCoupling, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(stringsCoupling, stringsCouplingLabel);
    stringsCouplingAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Coupling",stringsCoupling);
    stringsOutPos.setTextBoxStyle(juce::Slider::TextBoxLeft,true,50,20);
    stringsOutPos.setTextBoxIsEditable(true);

    addController(string1Level, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(string1Level, string1LevelLabel);
    string1LevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Level 1",string1Level);
    addController(string1FreqCoarse, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(string1FreqCoarse, string1FreqCoarseLabel);
    string1FreqCoarseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Freq Coarse 1",string1FreqCoarse);
    addController(string1FreqFine, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(string1FreqFine, string1FreqFineLabel);
    string1FreqFineAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Freq Fine 1",string1FreqFine);
       
    addController(string2Level, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(string2Level, string2LevelLabel);
    string2LevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Level 2",string2Level);
    addController(string2FreqCoarse, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(string2FreqCoarse, string2FreqCoarseLabel);
    string2FreqCoarseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Freq Coarse 2",string2FreqCoarse);
    addController(string2FreqFine, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(string2FreqFine, string2FreqFineLabel);
    string2FreqFineAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Freq Fine 2",string2FreqFine);

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
    
    // Sample
    addController(samplerAttack, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, SAMPLERCOLOR,juce::Colours::black);
    addAndConnectLabel(samplerAttack, samplerAttackLabel);
    samplerAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"AS",samplerAttack);
    addController(samplerDecay, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, SAMPLERCOLOR,juce::Colours::black);
    addAndConnectLabel(samplerDecay, samplerDecayLabel);
    samplerDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"DS",samplerDecay);
    addController(samplerSustain, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, SAMPLERCOLOR,juce::Colours::black);
    addAndConnectLabel(samplerSustain, samplerSustainLabel);
    samplerSustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"SS",samplerSustain);
    addController(samplerRelease, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, SAMPLERCOLOR,juce::Colours::black);
    addAndConnectLabel(samplerRelease, samplerReleaseLabel);
    samplerReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"RS",samplerRelease);
    addController(samplerFreq, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, SAMPLERCOLOR,juce::Colours::black);
    addAndConnectLabel(samplerFreq, samplerFreqLabel);
    samplerFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Sampler Lowpass",samplerFreq);
    addController(samplerLevel, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, SAMPLERCOLOR,juce::Colours::black);
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
    loopButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"Loop Sample",loopButton);
    addAndMakeVisible(loopLabel);
    loopLabel.attachToComponent(&loopButton,false);

    // Noise enveloppe
    addController(noiseAttack, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, NOISECOLOR,juce::Colours::black);
    addAndConnectLabel(noiseAttack, noiseAttackLabel);
    noiseAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"AN",noiseAttack);
    addController(noiseDecay, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, NOISECOLOR,juce::Colours::black);
    addAndConnectLabel(noiseDecay, noiseDecayLabel);
    noiseDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"DN",noiseDecay);
    addController(noiseSustain, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, NOISECOLOR,juce::Colours::black);
    addAndConnectLabel(noiseSustain, noiseSustainLabel);
    noiseSustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"SN",noiseSustain);
    addController(noiseRelease, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, NOISECOLOR,juce::Colours::black);
    addAndConnectLabel(noiseRelease, noiseReleaseLabel);
    noiseReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"RN",noiseRelease);
    addController(noiseFreq, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, NOISECOLOR,juce::Colours::black);
    addAndConnectLabel(noiseFreq, noiseFreqLabel);
    noiseFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Noise Freq",noiseFreq);
    addController(noiseLevel, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, NOISECOLOR,juce::Colours::black);
    addAndConnectLabel(noiseLevel, noiseLevelLabel);
    noiseLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Noise Level",noiseLevel);

    // crack enveloppe
    addController(crackAttack, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, CRACKSCOLOR,juce::Colours::black);
    addAndConnectLabel(crackAttack, crackAttackLabel);
    crackAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"AC",crackAttack);
    addController(crackDecay, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, CRACKSCOLOR,juce::Colours::black);
    addAndConnectLabel(crackDecay, crackDecayLabel);
    crackDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"DC",crackDecay);
    addController(crackSustain, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, CRACKSCOLOR,juce::Colours::black);
    addAndConnectLabel(crackSustain, crackSustainLabel);
    crackSustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"SC",crackSustain);
    addController(crackRelease, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, CRACKSCOLOR,juce::Colours::black);
    addAndConnectLabel(crackRelease, crackReleaseLabel);
    crackReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"RC",crackRelease);
    addController(crackDensity, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, CRACKSCOLOR,juce::Colours::black);
    addAndConnectLabel(crackDensity, crackDensityLabel);
    crackDensityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Crack Density",crackDensity);
    addController(crackFreq, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, CRACKSCOLOR,juce::Colours::black);
    addAndConnectLabel(crackFreq, crackFreqLabel);
    crackFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Crack Freq",crackFreq);
    addController(crackLevel, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, CRACKSCOLOR,juce::Colours::black);
    addAndConnectLabel(crackLevel, crackLevelLabel);
    crackLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Crack Level",crackLevel);

    addController(velocityLevel, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(velocityLevel, velocityLevelLabel);
    velocityLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Velocity Volume",velocityLevel);
    addController(velocitySampleFreq, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(velocitySampleFreq, velocitySampleFreqLabel);
    velocitySampleFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Velocity Sample Filter",velocitySampleFreq);
    addController(velocityNoiseFreq, juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Colours::teal,juce::Colours::black);
    addAndConnectLabel(velocityNoiseFreq, velocityNoiseFreqLabel);
    velocityNoiseFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Velocity Noise Filter",velocityNoiseFreq);

    setSize (900, 600);
}

MySynthAudioProcessorEditor::~MySynthAudioProcessorEditor()
{
}

//========================================  ======================================
void MySynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    static const float border = 0.01;
    float uxb = border*getWidth();
    float uyb = border*getHeight();
    auto ux = (1-2*border)*getWidth()/46;
    auto uy = (1-2*border)*getHeight()/24;
    const float step = 4.f;

    auto diagonale = (getLocalBounds().getTopLeft() - getLocalBounds().getBottomRight()).toFloat();
    auto length = diagonale.getDistanceFromOrigin();
    auto perpendicular = diagonale.rotatedAboutOrigin (juce::degreesToRadians (90.0f)) / length;
    auto height = float (getWidth() * getHeight()) / length;
    // auto bluegreengrey = juce::Colour::fromFloatRGBA(0.17f,0.22f,0.27f,1.0f);
    auto bluegreengrey = juce::Colour::fromFloatRGBA (0.15f, 0.15f, 0.25f, 1.0f);
    juce::ColourGradient grad (bluegreengrey.darker().darker().darker().darker(), perpendicular * height,
                           bluegreengrey.darker(), perpendicular * -height, false);
    g.setGradientFill(grad);
    g.fillAll();

    g.setColour(SAMPLERCOLOR.darker().darker());
    juce::Rectangle<int> samplerRect(uxb+0.75*step*ux,uyb+9*uy,7.25*step*ux,1.2*step*uy);
    g.drawRect(samplerRect);
    juce::Rectangle<int> samplerBand(uxb+0.75*step*ux,uyb+9*uy,0.25*step*ux,1.2*step*uy);
    g.fillRect(samplerBand);

    g.setColour(CRACKSCOLOR.darker().darker());
    juce::Rectangle<int> cracksRect(uxb+0.75*step*ux,uyb+14*uy,7.25*step*ux,1.2*step*uy);
    g.drawRect(cracksRect);
    juce::Rectangle<int> cracksBand(uxb+0.75*step*ux,uyb+14*uy,0.25*step*ux,1.2*step*uy);
    g.fillRect(cracksBand);

    g.setColour(NOISECOLOR.darker().darker());
    juce::Rectangle<int> noiseRect(uxb+0.75*step*ux,uyb+19*uy,7.25*step*ux,1.2*step*uy);
    g.drawRect(noiseRect);
    juce::Rectangle<int> noiseBand(uxb+0.75*step*ux,uyb+19*uy,0.25*step*ux,1.2*step*uy);
    g.fillRect(noiseBand);

}

void MySynthAudioProcessorEditor::resized()
{
    static const float border = 0.01;
    const float uxb = border*getWidth();
    const float uyb = border*getHeight();
    const auto ux = (1-2*border)*getWidth()/46;
    const auto uy = (1-2*border)*getHeight()/24;
    const float step = 4.f;
    const float size = 3.5f;

    float ex = uxb+8.5*step*ux;
    float ey = uyb+20*uy;    
    gain.setBounds(ex+step*ux,ey,size*ux,size*uy);
    level.setBounds(ex+2*step*ux,ey,size*ux,size*uy);
    porta.setBounds(ex,ey,size*ux,size*uy);

    ex = uxb+5*step*ux;
    ey = uyb+5*uy;
    stringsInPos.setBounds(ex,ey+uy,2*size*ux,uy);
    stringsOutPos.setBounds(ex,ey+3*uy,2*size*ux,uy);
    stringsCoupling.setBounds(ex+8*ux,ey,size*ux,size*uy);

    ex = uxb+8.5*step*ux;
    ey = uyb+1*uy;
    string1Level.setBounds(ex,ey,size*ux,size*uy);
    string1FreqCoarse.setBounds(ex+step*ux,ey,size*ux,size*uy);
    string1FreqFine.setBounds(ex+2*step*ux,ey,size*ux,size*uy);

    ex = uxb+8.5*step*ux;
    ey = uyb+(step+2)*uy;
    string2Level.setBounds(ex,ey,size*ux,size*uy);
    string2FreqCoarse.setBounds(ex+step*ux,ey,size*ux,size*uy);
    string2FreqFine.setBounds(ex+2*step*ux,ey,size*ux,size*uy);

    ex = uxb+8.5*step*ux;
    ey = uyb+(2.5*step+1)*uy;
    velocityLevel.setBounds(ex,ey,size*ux,size*uy);
    velocitySampleFreq.setBounds(ex+step*ux,ey,size*ux,size*uy);
    velocityNoiseFreq.setBounds(ex+2*step*ux,ey,size*ux,size*uy);

    ex = uxb+step*ux;
    ey = uyb+1*uy;
    stringsFBFreqOff.setBounds(ex,ey,size*ux,size*uy);
    stringsFBGainOff.setBounds(ex+step*ux,ey,size*ux,size*uy);
    stringsLevelOff.setBounds(ex+2*step*ux,ey,size*ux,size*uy);
    stringsFBFreqOn.setBounds(ex+3*step*ux,ey,size*ux,size*uy);
    stringsFBGainOn.setBounds(ex+4*step*ux,ey,size*ux,size*uy);
    stringsLevelOn.setBounds(ex+5*step*ux,ey,size*ux,size*uy);

    ex = uxb+step*ux;
    ey = uyb+5*uy;
    stringsAttack.setBounds(ex,ey,size*ux,size*uy);
    stringsDecay.setBounds(ex+step*ux,ey,size*ux,size*uy);
    stringsSustain.setBounds(ex+2*step*ux,ey,size*ux,size*uy);
    stringsRelease.setBounds(ex+3*step*ux,ey,size*ux,size*uy);

    ex = uxb+step*ux;
    ey = uyb+10*uy;
    samplerAttack.setBounds(ex,ey,size*ux,size*uy);
    samplerDecay.setBounds(ex+step*ux,ey,size*ux,size*uy);
    samplerSustain.setBounds(ex+2*step*ux,ey,size*ux,size*uy);
    samplerRelease.setBounds(ex+3*step*ux,ey,size*ux,size*uy);
    samplerLevel.setBounds(ex+4*step*ux,ey,size*ux,size*uy);
    samplerFreq.setBounds(ex+5*step*ux,ey,size*ux,size*uy);
    waveComboBox.setBounds(ex+6*step*ux,ey,size*ux,1*uy);
    loopButton.setBounds(ex+6.25*step*ux,ey+2.5*uy,size*ux,1*uy);
    
    ex = uxb+step*ux;
    ey = uyb+15*uy;
    crackAttack.setBounds(ex,ey,size*ux,size*uy);
    crackDecay.setBounds(ex+step*ux,ey,size*ux,size*uy);
    crackSustain.setBounds(ex+2*step*ux,ey,size*ux,size*uy);
    crackRelease.setBounds(ex+3*step*ux,ey,size*ux,size*uy);
    crackLevel.setBounds(ex+4*step*ux,ey,size*ux,size*uy);
    crackFreq.setBounds(ex+5*step*ux,ey,size*ux,size*uy);
    crackDensity.setBounds(ex+6*step*ux,ey,size*ux,size*uy);

    ex = uxb+step*ux;
    ey = uyb+20*uy;
    noiseAttack.setBounds(ex,ey,size*ux,size*uy);
    noiseDecay.setBounds(ex+step*ux,ey,size*ux,size*uy);
    noiseSustain.setBounds(ex+2*step*ux,ey,size*ux,size*uy);
    noiseRelease.setBounds(ex+3*step*ux,ey,size*ux,size*uy);
    noiseLevel.setBounds(ex+4*step*ux,ey,size*ux,size*uy);
    noiseFreq.setBounds(ex+6*step*ux,ey,size*ux,size*uy);
    
}

void MySynthAudioProcessorEditor::addController(juce::Slider& slider,
                                                    juce::Slider::SliderStyle style,
                                                    juce::Colour fillCol,
                                                    juce::Colour outlineCol)
{
  slider.setSliderStyle(style);
  slider.setTextBoxStyle(juce::Slider::TextBoxBelow,true,80,15);
  slider.setTextBoxIsEditable(true);
  slider.setColour(juce::Slider::thumbColourId, fillCol);
  slider.setColour(juce::Slider::rotarySliderFillColourId, fillCol);
  slider.setColour(juce::Slider::trackColourId, outlineCol);
  slider.setColour(juce::Slider::backgroundColourId, outlineCol);
  slider.setColour(juce::Slider::rotarySliderOutlineColourId, outlineCol);
  // fxmeLookAndFeel.setColour(fillCol);
  // fxmeLookAndFeel.setText(text);
  slider.setLookAndFeel(&fxmeLookAndFeel);
  addAndMakeVisible(slider);
}

void MySynthAudioProcessorEditor::addAndConnectLabel(juce::Slider& slider,
                                                juce::Label& label)
{
  addAndMakeVisible(label);
  label.setJustificationType(juce::Justification::centred);
  label.attachToComponent(&slider,false);
}

void MySynthAudioProcessorEditor::addController2(juce::Slider &slider, juce::Slider::SliderStyle style, juce::Colour fillCol, juce::Colour outlineCol, juce::String text)
{
  slider.setSliderStyle(style);
  slider.setTextBoxStyle(juce::Slider::TextBoxBelow,true,100,20);
  slider.setPopupDisplayEnabled(true,true,nullptr,2000);
  fxmeLookAndFeel.setColour(fillCol);
  fxmeLookAndFeel.setText(text);
  slider.setLookAndFeel(&fxmeLookAndFeel);
  addAndMakeVisible(slider);
}
