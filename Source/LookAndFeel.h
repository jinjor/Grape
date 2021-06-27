#pragma once
#include <JuceHeader.h>

namespace {
const juce::Colour COLOUR_CONTROLLED = juce::Colour(170,170,170);
const juce::Colour COLOUR_SELECT = juce::Colour(170,100,210);
const juce::Colour COLOUR_PIT = juce::Colour(0,0,0);
const juce::Colour COLOUR_BORDER = juce::Colour(30,30,30);
const juce::Colour COLOUR_BACKGROUND = juce::Colour(30,30,30);
const juce::Colour COLOUR_TEXT = juce::Colour(200,200,200);
const int MAX_KNOB_WIDTH = 40;
const float SLIT_WIDTH = 2.0f;
const float POINTER_RADIUS = 4.0f;
const float BORDER_WIDTH = 1.0f;
const float ARROW_ZONE_WIDTH = 18.0f;
}

class CentredSlider : public juce::Slider
{
public:
    CentredSlider() {}
    ~CentredSlider() {}
};
class GrapeLookAndFeel : public juce::LookAndFeel_V4
{
public:
    GrapeLookAndFeel(bool controlled = false);
    ~GrapeLookAndFeel();
private:
    bool controlled;
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
    void drawLabel (juce::Graphics& g, juce::Label& label) override;
    void drawPopupMenuBackground (juce::Graphics&, int width, int height) override;
    void drawPopupMenuItem (juce::Graphics& g, const juce::Rectangle<int>& area,
                                            const bool isSeparator, const bool isActive,
                                            const bool isHighlighted, const bool isTicked,
                                            const bool hasSubMenu, const juce::String& text,
                                            const juce::String& shortcutKeyText,
                                              const juce::Drawable* icon, const juce::Colour* const textColourToUse) override;
    juce::Path getTickShape (float height) override;
};
