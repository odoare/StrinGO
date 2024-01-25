#pragma once
#include <JuceHeader.h>

class FxmeLookAndFeel : public juce::LookAndFeel_V4
{
public:
  FxmeLookAndFeel()
  {
  }

  void drawRotarySlider(juce::Graphics &g, 
          int x, int y, 
          int width,
          int height, 
          float sliderPos, 
          float rotaryStartAngle, 
          float rotaryEndAngle, 
          juce::Slider &slider) override
  {
    float diameter = juce::jmin(width,height);
    float radius = diameter * 0.5;
    float centreX = x + width * 0.5;
    float centreY = y + height * 0.5;
    float rx = centreX - radius;
    float ry = centreY - radius;
    float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle-rotaryStartAngle));

    juce::Rectangle<float> dialArea(rx,ry,diameter,diameter);
    g.setColour(juce::Colours::black);
    // g.setColour(juce::Colour::fromFloatRGBA (0.15f, 0.15f, 0.2f, 1.0f));
    //g.drawRect(dialArea);
    g.fillEllipse(dialArea.reduced(8));
    g.setColour(slider.findColour(juce::Slider::thumbColourId).darker());
    g.drawEllipse(dialArea.reduced(10),2.0f);
    
    // g.drawText(slider.getValue());

    g.setColour(slider.findColour(juce::Slider::thumbColourId));
    juce::Path dialTick;
    juce::Rectangle<int> rect(0.f,-radius,5.0f,radius*0.4);
    dialTick.addRectangle(rect);
    g.fillPath(dialTick,juce::AffineTransform::rotation(angle).translated(centreX,centreY));

    g.setColour(slider.findColour(juce::Slider::thumbColourId).darker().darker().darker());
    juce::Path arc1;
    arc1.addArc(centreX-diameter/2, centreY-diameter/2, diameter, diameter, rotaryStartAngle, rotaryEndAngle, true);
    g.strokePath(arc1, juce::PathStrokeType(6.0f));
    g.setColour(slider.findColour(juce::Slider::thumbColourId));
    juce::Path arc2;
    arc2.addArc(centreX-diameter/2, centreY-diameter/2, diameter, diameter, rotaryStartAngle, angle, true);
    g.strokePath(arc2, juce::PathStrokeType(6.0f));
    // juce::DropShadow shadow(juce::Colours::white,30,point);
    // shadow.drawForRectangle(g,rect);
  };

  juce::Slider::SliderLayout getSliderLayout (juce::Slider& slider) override
  {
    auto localBounds = slider.getLocalBounds().reduced(5).toFloat();
    juce::Slider::SliderLayout layout;
    layout.sliderBounds = slider.getLocalBounds().reduced(5);
    layout.textBoxBounds = slider.getLocalBounds().reduced(5).withSizeKeepingCentre (localBounds.getWidth() * 0.7f,
                                                                localBounds.getHeight() * 0.25f);
    return layout;
  }

  void drawLabel(juce::Graphics &g, juce::Label &label) override
  {
    g.setColour(juce::Colours::white);
    
    auto labelArea{label.getLocalBounds().toFloat()};
    g.setFont(labelArea.getHeight() * 0.8f);
    auto center = labelArea.getCentre();
    auto bottom = labelArea.getBottom();

    juce::String labelToDisplay = juce::String(label.getText());
    //juce::String labelToDisplay = sliderText;
    
    g.drawSingleLineText(labelToDisplay,
                          juce::roundToInt(center.x + g.getCurrentFont().getHorizontalScale()),
                          juce::roundToInt(center.y + g.getCurrentFont().getDescent()),
                          juce::Justification::horizontallyCentred);

    // g.drawSingleLineText(labelToDisplay,
    //                       juce::roundToInt(center.x + g.getCurrentFont().getHorizontalScale()),
    //                       juce::roundToInt(bottom),
    //                       juce::Justification::horizontallyCentred);
        
    }



    void setColour(juce::Colour newCol)
    {
      sliderColour = newCol;
    }

    void setText(juce::String newText)
    {
      sliderText = newText;
    }

private:
  juce::Colour sliderColour;
  juce::String sliderText;
};
