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
#define VELOCITYCOLOR juce::Colours::magenta
#define STRINGCOLOR juce::Colours::cyan
#define TITLECOLOR juce::Colours::grey


//==============================================================================
MySynthAudioProcessorEditor::MySynthAudioProcessorEditor (MySynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addVSlider(gain, gainLabel, MASTERCOLOR,juce::Colours::black);
    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Output Gain",gain);

    addVSlider(level, levelLabel, MASTERCOLOR,juce::Colours::black);
    levelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Output Level",level);

    addKnob(porta, portaLabel, MASTERCOLOR,juce::Colours::black);
    portaAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Porta time",porta);

    // Waveguides
    addKnob(stringsFBGainOn, stringsFBGainOnLabel, STRINGCOLOR, juce::Colours::black);
    stringsFBGainOnAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Fb Gain on",stringsFBGainOn);
    addKnob(stringsFBFreqOn, stringsFBFreqOnLabel, STRINGCOLOR,juce::Colours::black);
    stringsFBFreqOnAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Fb Freq on",stringsFBFreqOn);
    addKnob(stringsLevelOn, stringsLevelOnLabel, STRINGCOLOR,juce::Colours::black);
    stringsLevelOnAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Level on",stringsLevelOn);

    addKnob(stringsFBGainOff, stringsFBGainOffLabel, STRINGCOLOR,juce::Colours::black);
    stringsFBGainOffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Fb Gain off",stringsFBGainOff);
    addKnob(stringsFBFreqOff, stringsFBFreqOffLabel, STRINGCOLOR,juce::Colours::black);
    stringsFBFreqOffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Fb Freq off",stringsFBFreqOff);
    addKnob(stringsLevelOff, stringsLevelOffLabel, STRINGCOLOR,juce::Colours::black);
    stringsLevelOffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Level off",stringsLevelOff);

    addHSlider(stringsInPos, stringsInPosLabel, STRINGCOLOR,juce::Colours::black);
    // addAndConnectLabel(stringsInPos, stringsInPosLabel);
    stringsInPosAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"In Pos",stringsInPos);
    //stringsInPos.setTextBoxStyle(juce::Slider::NoTextBox,true,50,20);

    addHSlider(stringsOutPos, stringsOutPosLabel, STRINGCOLOR,juce::Colours::black);
    // addAndConnectLabel(stringsOutPos, stringsOutPosLabel);
    stringsOutPosAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Out Pos",stringsOutPos);
    //stringsOutPos.setTextBoxStyle(juce::Slider::NoTextBox,true,50,20);

    addHSlider(stringsCoupling, stringsCouplingLabel, STRINGCOLOR,juce::Colours::black);
    // addAndConnectLabel(stringsCoupling, stringsCouplingLabel);
    stringsCouplingAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Coupling",stringsCoupling);

    addKnob(string1Level, string1LevelLabel, STRINGCOLOR,juce::Colours::black);
    string1LevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Level 1",string1Level);
    addKnob(string1FreqCoarse, string1FreqCoarseLabel, STRINGCOLOR,juce::Colours::black);
    string1FreqCoarseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Freq Coarse 1",string1FreqCoarse);
    addKnob(string1FreqFine, string1FreqFineLabel, STRINGCOLOR,juce::Colours::black);
    string1FreqFineAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Freq Fine 1",string1FreqFine);
       
    addKnob(string2Level, string2LevelLabel, STRINGCOLOR,juce::Colours::black);
    string2LevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Level 2",string2Level);
    addKnob(string2FreqCoarse, string2FreqCoarseLabel, STRINGCOLOR,juce::Colours::black);
    string2FreqCoarseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Freq Coarse 2",string2FreqCoarse);
    addKnob(string2FreqFine, string2FreqFineLabel, STRINGCOLOR,juce::Colours::black);
    string2FreqFineAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Freq Fine 2",string2FreqFine);

    // String waveguide enveloppe
    addKnob(stringsAttack, stringsAttackLabel, STRINGCOLOR,juce::Colours::black);
    stringsAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"A",stringsAttack);
    addKnob(stringsDecay, stringsDecayLabel, STRINGCOLOR,juce::Colours::black);
    stringsDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"D",stringsDecay);
    addKnob(stringsSustain, stringsSustainLabel, STRINGCOLOR,juce::Colours::black);
    stringsSustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"S",stringsSustain);
    addKnob(stringsRelease, stringsReleaseLabel, STRINGCOLOR,juce::Colours::black);
    stringsReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"R",stringsRelease);
    
    // Sample
    addKnob(samplerAttack, samplerAttackLabel, SAMPLERCOLOR,juce::Colours::black);
    samplerAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"AS",samplerAttack);
    addKnob(samplerDecay, samplerDecayLabel, SAMPLERCOLOR,juce::Colours::black);
    samplerDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"DS",samplerDecay);
    addKnob(samplerSustain, samplerSustainLabel, SAMPLERCOLOR,juce::Colours::black);
    samplerSustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"SS",samplerSustain);
    addKnob(samplerRelease, samplerReleaseLabel, SAMPLERCOLOR,juce::Colours::black);
    samplerReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"RS",samplerRelease);
    addKnob(samplerFreq, samplerFreqLabel, SAMPLERCOLOR,juce::Colours::black);
    samplerFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Sample Freq",samplerFreq);
    addKnob(samplerLevel, samplerLevelLabel, SAMPLERCOLOR,juce::Colours::black);
    samplerLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Sample Level",samplerLevel);

    // // Sample wave combo box
    juce::StringArray choices;
    choices.addArray(WAVECHOICES);
    waveComboBox.addItemList(choices,1);
    waveComboBoxAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts,"Attack Sample",waveComboBox);
    waveLabel.attachToComponent(&waveComboBox,false);
    addAndMakeVisible(waveComboBox);
    addAndMakeVisible(waveLabel);

    addAndMakeVisible(loopButton);
    loopButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,"Loop Sample",loopButton);
    addAndMakeVisible(loopLabel);
    loopLabel.attachToComponent(&loopButton,true);

    // Noise enveloppe
    addKnob(noiseAttack, noiseAttackLabel, NOISECOLOR,juce::Colours::black);
    noiseAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"AN",noiseAttack);
    addKnob(noiseDecay, noiseDecayLabel, NOISECOLOR,juce::Colours::black);
    noiseDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"DN",noiseDecay);
    addKnob(noiseSustain, noiseSustainLabel, NOISECOLOR,juce::Colours::black);
    noiseSustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"SN",noiseSustain);
    addKnob(noiseRelease, noiseReleaseLabel, NOISECOLOR,juce::Colours::black);
    noiseReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"RN",noiseRelease);
    addKnob(noiseLPFreq, noiseLPFreqLabel, NOISECOLOR,juce::Colours::black);
    noiseLPFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Noise LP Freq",noiseLPFreq);
    addKnob(noiseHPFreq, noiseHPFreqLabel, NOISECOLOR,juce::Colours::black);
    noiseHPFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Noise HP Freq",noiseHPFreq);
    addKnob(noiseLevel, noiseLevelLabel, NOISECOLOR,juce::Colours::black);
    noiseLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Noise Level",noiseLevel);

    // crack enveloppe
    addKnob(crackAttack, crackAttackLabel, CRACKSCOLOR,juce::Colours::black);
    crackAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"AC",crackAttack);
    addKnob(crackDecay, crackDecayLabel, CRACKSCOLOR,juce::Colours::black);
    crackDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"DC",crackDecay);
    addKnob(crackSustain, crackSustainLabel, CRACKSCOLOR,juce::Colours::black);
    crackSustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"SC",crackSustain);
    addKnob(crackRelease, crackReleaseLabel, CRACKSCOLOR,juce::Colours::black);
    crackReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"RC",crackRelease);
    addKnob(crackDensity, crackDensityLabel, CRACKSCOLOR,juce::Colours::black);
    crackDensityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Crack Density",crackDensity);
    addKnob(crackFreq, crackFreqLabel, CRACKSCOLOR,juce::Colours::black);
    crackFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Crack Freq",crackFreq);
    addKnob(crackLevel, crackLevelLabel, CRACKSCOLOR,juce::Colours::black);
    crackLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Crack Level",crackLevel);

    addKnob(velocitySampleLevel, velocitySampleLevelLabel, SAMPLERCOLOR,juce::Colours::black);
    velocitySampleLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Velocity Sample Level",velocitySampleLevel);
    addKnob(velocitySampleFreq, velocitySampleFreqLabel, SAMPLERCOLOR,juce::Colours::black);
    velocitySampleFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Velocity Sample Freq",velocitySampleFreq);

    addKnob(velocityNoiseLevel, velocityNoiseLevelLabel, NOISECOLOR,juce::Colours::black);
    velocityNoiseLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Velocity Noise Level",velocityNoiseLevel);
    addKnob(velocityNoiseFreq, velocityNoiseFreqLabel, NOISECOLOR,juce::Colours::black);
    velocityNoiseLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Velocity Noise Freq",velocityNoiseFreq);

    addKnob(velocityCrackLevel, velocityCrackLevelLabel, CRACKSCOLOR,juce::Colours::black);
    velocityCrackLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Velocity Crack Level",velocityCrackLevel);
    addKnob(velocityCrackFreq, velocityCrackFreqLabel, CRACKSCOLOR,juce::Colours::black);
    velocityCrackFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Velocity Crack Freq",velocityCrackFreq);

    // addKnob(velocityLevel, velocityLevelLabel, VELOCITYCOLOR,juce::Colours::black);
    // velocityLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Velocity Volume",velocityLevel);
    // addKnob(velocitySampleFreq, velocitySampleFreqLabel, VELOCITYCOLOR,juce::Colours::black);
    // velocitySampleFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Velocity Sample Filter",velocitySampleFreq);
    // addKnob(velocityNoiseFreq, velocityNoiseFreqLabel, VELOCITYCOLOR,juce::Colours::black);
    // velocityNoiseFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,"Velocity Noise Filter",velocityNoiseFreq);

    setSize (1000, 650);
    setResizable(true,true);
}

MySynthAudioProcessorEditor::~MySynthAudioProcessorEditor()
{
}

#define NX 12.5
#define NY 6.5
#define SX 0.8
#define SY 0.8
#define DELTAX 0.1f
#define DELTAY 0.1f



//========================================  ======================================
void MySynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    static const float border = 0.01;
    float uxb = border*getWidth();
    float uyb = border*getHeight();
    auto ux = (1-2*border)*getWidth()/NX;
    auto uy = (1-2*border)*getHeight()/NY;
    const float step = 1.f;

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

    drawBoxWithTitleOnLeft(g,"Wave",SAMPLERCOLOR,uxb+0.5*ux,uyb,10*ux,uy,0.5*ux);
    drawBoxWithTitleOnLeft(g,"Noise",NOISECOLOR,uxb+0.5*ux,uyb+uy,10*ux,uy,0.5*ux);
    drawBoxWithTitleOnLeft(g,"Cracks",CRACKSCOLOR,uxb+0.5*ux,uyb+2*uy,10*ux,uy,0.5*ux);

    drawBoxWithTitleOnLeft(g,"Exciters",TITLECOLOR,uxb,uyb,0.5*ux,3*uy, 0.5*ux);
    drawBoxWithTitleOnLeft(g,"Strings",TITLECOLOR,uxb,uyb+3.25*uy,0.5*ux,3.5*uy, 0.5*ux);

    drawBoxWithTitleOnTop(g,"Properties when note off",STRINGCOLOR,uxb+0.5*ux,uyb+5.25*uy,3*ux,1.25*uy,0.25*uy);
    drawBoxWithTitleOnTop(g,"Properties when note on",STRINGCOLOR,uxb+3.5*ux,uyb+5.25*uy,3*ux,1.25*uy,0.25*uy);
    drawBoxWithTitleOnTop(g,"Enveloppe -> Properties",STRINGCOLOR,uxb+6.5*ux,uyb+5.25*uy,4*ux,1.25*uy,0.25*uy);
    
    drawBoxWithTitleOnTop(g,"",STRINGCOLOR,uxb+0.5*ux,uyb+3.25*uy,6*ux,1.85*uy,0.f);
    drawBoxWithTitleOnTop(g,"Waveguide 1",STRINGCOLOR,uxb+0.5*ux,uyb+3.25*uy,3*ux,1.25*uy,0.25*uy);
    drawBoxWithTitleOnTop(g,"Waveguide 2",STRINGCOLOR,uxb+3.5*ux,uyb+3.25*uy,3*ux,1.25*uy,0.25*uy);
    drawBoxWithTitleOnTop(g,"In/Out Positions",STRINGCOLOR,uxb+6.5*ux,uyb+3.25*uy,4*ux,1.85*uy,0.25*uy);

    drawBoxWithTitleOnLeft(g,"Velocity sensitivity",TITLECOLOR,uxb+8*ux,uyb,2.5*ux,3*uy,0.5*ux);
    drawBoxWithTitleOnTop(g,"Midi",MASTERCOLOR,uxb+10.5*ux,uyb,2*ux,1.25*uy,0.25*uy);
    drawBoxWithTitleOnTop(g,"Master",MASTERCOLOR,uxb+10.5*ux,uyb+1.25*uy,2*ux,5.25*uy,0.25*uy);
}

// ----------------------------------------------------------------------------
void MySynthAudioProcessorEditor::resized()
{
    static const float border = 0.01;
    const float uxb = border*getWidth();
    const float uyb = border*getHeight();
    const auto ux = (1-2*border)*getWidth()/NX;
    const auto uy = (1-2*border)*getHeight()/NY;
    const float step = 1.f;
    const float stepy = 1.f;
    const float size = 0.8f;

    float ex = uxb+11*ux;
    float ey = uyb+0.25*uy;
    porta.setBounds(juce::Rectangle<int>(ex,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));

    ex = uxb+10.5*ux;
    ey = uyb+1.75*uy;
    gain.setBounds(juce::Rectangle<int>(ex,ey,ux,2*uy).reduced(DELTAX*ux,DELTAY*uy));
    level.setBounds(juce::Rectangle<int>(ex+ux,ey,ux,2*uy).reduced(DELTAX*ux,DELTAY*uy));

    ex = uxb+8.5*ux;
    ey = uyb;
    velocitySampleFreq.setBounds(juce::Rectangle<int>(ex,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    velocitySampleLevel.setBounds(juce::Rectangle<int>(ex+ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    velocityNoiseFreq.setBounds(juce::Rectangle<int>(ex,ey+uy,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    velocityNoiseLevel.setBounds(juce::Rectangle<int>(ex+ux,ey+uy,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    velocityCrackFreq.setBounds(juce::Rectangle<int>(ex,ey+2*uy,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    velocityCrackLevel.setBounds(juce::Rectangle<int>(ex+ux,ey+2*uy,ux,uy).reduced(DELTAX*ux,DELTAY*uy));

    ex = uxb+6.75*ux;
    ey = uyb+3.75*uy;
    stringsInPos.setBounds(ex,ey+0*uy,3.5*ux,0.5*uy);
    stringsOutPos.setBounds(ex,ey+0.75*uy,3.5*ux,0.5*uy);

    ex = uxb+ux;
    ey = uyb+4.6*uy;
    stringsCoupling.setBounds(ex,ey,5*ux,0.5*uy);

    ex = uxb+0.5*ux;
    ey = uyb+3.5*uy;
    string1Level.setBounds(juce::Rectangle<int>(ex,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    string1FreqCoarse.setBounds(juce::Rectangle<int>(ex+ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    string1FreqFine.setBounds(juce::Rectangle<int>(ex+2*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));

    ex = uxb+3.5*ux;
    ey = uyb+3.5*uy;
    string2Level.setBounds(juce::Rectangle<int>(ex,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    string2FreqCoarse.setBounds(juce::Rectangle<int>(ex+ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    string2FreqFine.setBounds(juce::Rectangle<int>(ex+2*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));


    ex = uxb+0.5*ux;
    ey = uyb+5.5*uy;
    stringsFBFreqOff.setBounds(juce::Rectangle<int>(ex,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    stringsFBGainOff.setBounds(juce::Rectangle<int>(ex+1*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    stringsLevelOff.setBounds(juce::Rectangle<int>(ex+2*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));

    ex = uxb+3.5*ux;
    ey = uyb+5.5*uy;
    stringsFBFreqOn.setBounds(juce::Rectangle<int>(ex,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    stringsFBGainOn.setBounds(juce::Rectangle<int>(ex+1*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    stringsLevelOn.setBounds(juce::Rectangle<int>(ex+2*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));

    ex = uxb+6.5*ux;
    ey = uyb+5.5*uy;
    stringsAttack.setBounds(juce::Rectangle<int>(ex,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    stringsDecay.setBounds(juce::Rectangle<int>(ex+ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    stringsSustain.setBounds(juce::Rectangle<int>(ex+2*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    stringsRelease.setBounds(juce::Rectangle<int>(ex+3*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));

    ex = uxb+ux;
    ey = uyb;
    samplerAttack.setBounds(juce::Rectangle<int>(ex,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    samplerDecay.setBounds(juce::Rectangle<int>(ex+ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    samplerSustain.setBounds(juce::Rectangle<int>(ex+2*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    samplerRelease.setBounds(juce::Rectangle<int>(ex+3*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    samplerFreq.setBounds(juce::Rectangle<int>(ex+5*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    samplerLevel.setBounds(juce::Rectangle<int>(ex+6*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    waveComboBox.setBounds(ex+4*ux,ey+0.25*uy,ux,0.25*uy);
    loopButton.setBounds(ex+4.75*ux,ey+0.6*uy,size*ux,0.25*uy);

    ex = uxb+ux;
    ey = uyb+uy;
    noiseAttack.setBounds(juce::Rectangle<int>(ex,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    noiseDecay.setBounds(juce::Rectangle<int>(ex+1*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    noiseSustain.setBounds(juce::Rectangle<int>(ex+2*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    noiseRelease.setBounds(juce::Rectangle<int>(ex+3*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    noiseHPFreq.setBounds(juce::Rectangle<int>(ex+4*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    noiseLPFreq.setBounds(juce::Rectangle<int>(ex+5*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    noiseLevel.setBounds(juce::Rectangle<int>(ex+6*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));

    ex = uxb+ux;
    ey = uyb+2*uy;
    crackAttack.setBounds(juce::Rectangle<int>(ex,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    crackDecay.setBounds(juce::Rectangle<int>(ex+1*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    crackSustain.setBounds(juce::Rectangle<int>(ex+2*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    crackRelease.setBounds(juce::Rectangle<int>(ex+3*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    crackDensity.setBounds(juce::Rectangle<int>(ex+4*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    crackFreq.setBounds(juce::Rectangle<int>(ex+5*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
    crackLevel.setBounds(juce::Rectangle<int>(ex+6*ux,ey,ux,uy).reduced(DELTAX*ux,DELTAY*uy));
}

// -------------------------------------------------------------------------
void MySynthAudioProcessorEditor::addKnob(juce::Slider& slider,
                                                    juce::Label &label,
                                                    juce::Colour fillCol,
                                                    juce::Colour outlineCol)
{
  slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
  slider.setTextBoxStyle(juce::Slider::TextBoxBelow,true,80,15);
  slider.setTextBoxIsEditable(true);
  slider.setColour(juce::Slider::thumbColourId, fillCol);
  slider.setColour(juce::Slider::rotarySliderFillColourId, fillCol);
  slider.setColour(juce::Slider::trackColourId, outlineCol);
  slider.setColour(juce::Slider::backgroundColourId, outlineCol);
  slider.setColour(juce::Slider::rotarySliderOutlineColourId, outlineCol);
  slider.setLookAndFeel(&fxmeKnobLookAndFeel);
  addAndMakeVisible(slider);

  label.setJustificationType(juce::Justification::centred);
  label.attachToComponent(&slider,false);
  juce::AffineTransform t;
  auto labelArea{label.getLocalBounds().toFloat()};
  label.setTransform(t.translated(0,labelArea.getHeight()*0.65f));
  addAndMakeVisible(label);  
}

// -------------------------------------------------------------------------
void MySynthAudioProcessorEditor::addVSlider(juce::Slider& slider,
                                                    juce::Label &label,
                                                    juce::Colour fillCol,
                                                    juce::Colour outlineCol)
{
  slider.setSliderStyle(juce::Slider::LinearVertical);
  slider.setTextBoxStyle(juce::Slider::TextBoxBelow,true,80,15);
  slider.setTextBoxIsEditable(true);
  slider.setColour(juce::Slider::thumbColourId, fillCol);
  slider.setColour(juce::Slider::rotarySliderFillColourId, fillCol);
  slider.setColour(juce::Slider::trackColourId, outlineCol);
  slider.setColour(juce::Slider::backgroundColourId, outlineCol);
  slider.setColour(juce::Slider::rotarySliderOutlineColourId, outlineCol);
  addAndMakeVisible(slider);

  label.setJustificationType(juce::Justification::centred);
  label.attachToComponent(&slider,false);
  // juce::AffineTransform t;
  // auto labelArea{label.getLocalBounds().toFloat()};
  // label.setTransform(t.translated(0,labelArea.getHeight()*0.75f));
  addAndMakeVisible(label);
}


// -------------------------------------------------------------------------
void MySynthAudioProcessorEditor::addHSlider(juce::Slider& slider,
                                                    juce::Label &label,
                                                    juce::Colour fillCol,
                                                    juce::Colour outlineCol)
{
  slider.setSliderStyle(juce::Slider::LinearHorizontal);
  slider.setTextBoxStyle(juce::Slider::NoTextBox,true,120,15);
  slider.setTextBoxIsEditable(true);
  slider.setColour(juce::Slider::thumbColourId, fillCol);
  slider.setColour(juce::Slider::rotarySliderFillColourId, fillCol);
  slider.setColour(juce::Slider::trackColourId, outlineCol);
  slider.setColour(juce::Slider::backgroundColourId, outlineCol);
  slider.setColour(juce::Slider::rotarySliderOutlineColourId, outlineCol);
  addAndMakeVisible(slider);

  label.setJustificationType(juce::Justification::left);
  label.attachToComponent(&slider,false);
  // label.setBoundsRelative(0.2,0.2,0.2,0.2);
  juce::AffineTransform t;
  auto labelArea{label.getLocalBounds().toFloat()};
  label.setTransform(t.translated(-labelArea.getWidth(),labelArea.getHeight()*.75f));
  addAndMakeVisible(label);
}

// -------------------------------------------------------------------------------------
 void MySynthAudioProcessorEditor::drawBoxWithTitleOnTop(juce::Graphics &g,
                                                          const juce::String title,
                                                          const juce::Colour baseColour,
                                                          const float x,
                                                          const float y,
                                                          const float wx,
                                                          const float wy,
                                                          const float bandHeight)
 {
  g.setColour(baseColour.darker(1.5f));
  juce::Rectangle<int> stringRect2(x,y,wx,wy);
  g.drawRect(stringRect2);
  juce::Rectangle<int> stringBand2(x,y,wx,bandHeight);
  g.fillRect(stringBand2);
  g.setColour(juce::Colours::white);    
  g.drawText(title, stringBand2, juce::Justification::centred);
 }


// -------------------------------------------------------------------------------------
void MySynthAudioProcessorEditor::drawBoxWithTitleOnLeft(juce::Graphics &g,
                                                          const juce::String title,
                                                          const juce::Colour baseColour,
                                                          const float x,
                                                          const float y,
                                                          const float wx,
                                                          const float wy,
                                                          const float bandWidth)
 {
  float fontSize=18;
  g.setColour(baseColour.darker(1.5f));
  juce::Rectangle<int> stringRect2(x,y,wx,wy);
  g.drawRect(stringRect2);
  juce::Rectangle<int> stringBand2(x,y,bandWidth,wy);
  g.fillRect(stringBand2);
  g.setColour(juce::Colours::white);
  g.setFont (fontSize);
    
  juce::GlyphArrangement ga;
  ga.addLineOfText(juce::Font(fontSize), title, 0, 0);
  juce::Path p;
  ga.createPath(p);

  auto pathBounds = p.getBounds();
  p.applyTransform(juce::AffineTransform()
                    .translated(x, y+wy*0.5)
                    .rotated(-1.57079632f,x+bandWidth/2,y+wy/2)
                    );

  g.setColour (juce::Colours::white);
  g.fillPath(p);
 }
