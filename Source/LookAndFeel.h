#pragma once
#include <JuceHeader.h>

class CentredSlider : public juce::Slider
{
public:
    CentredSlider() {}
    ~CentredSlider() {}
};
class GrapeLookAndFeel : public juce::LookAndFeel_V4
{
public:
    GrapeLookAndFeel();
    ~GrapeLookAndFeel();
private:
    void drawTickBox (juce::Graphics& g, juce::Component& component,
                      float x, float y, float w, float h,
                      const bool ticked,
                      const bool isEnabled,
                      const bool shouldDrawButtonAsHighlighted,
                      const bool shouldDrawButtonAsDown) override;
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                           const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override;
    void drawComboBox(juce::Graphics&, int width, int height, bool isButtonDown, int buttonX, int buttonY, int buttonW, int buttonH, juce::ComboBox&) override;
    void positionComboBoxText (juce::ComboBox& box, juce::Label& label) override;
};
