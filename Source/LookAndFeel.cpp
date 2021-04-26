#include "LookAndFeel.h"

namespace {
const juce::Colour COLOUR_SELECT = juce::Colour(170,100,210);
const juce::Colour COLOUR_PIT = juce::Colour(0,0,0);
const juce::Colour COLOUR_BORDER = juce::Colour(30,30,30);
const juce::Colour COLOUR_TEXT = juce::Colour(200,200,200);
const int KNOB_WIDTH = 40;
const float SLIT_WIDTH = 2.0f;
const float POINTER_RADIUS = 4.0f;
const float BORDER_WIDTH = 1.0f;
const float ARROW_ZONE_WIDTH = 18.0f;
}

GrapeLookAndFeel::GrapeLookAndFeel()
{
    setColour (juce::ComboBox::textColourId, COLOUR_TEXT);
    setColour (juce::ComboBox::arrowColourId, COLOUR_TEXT);
    setColour (juce::ComboBox::outlineColourId, COLOUR_BORDER);
}
GrapeLookAndFeel::~GrapeLookAndFeel()
{}
void GrapeLookAndFeel::drawTickBox (juce::Graphics& g, juce::Component& component,
                                  float x, float y, float w, float h,
                                  const bool ticked,
                                  const bool isEnabled,
                                  const bool shouldDrawButtonAsHighlighted,
                                  const bool shouldDrawButtonAsDown)
{
    juce::ignoreUnused (isEnabled, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
    juce::Rectangle<float> tickBounds (x, y, w, h);
    tickBounds = tickBounds.reduced(1);
    auto reduced = tickBounds.reduced(0.5);
    
    g.setColour (COLOUR_PIT);
    g.fillRect(tickBounds);
    g.setColour (COLOUR_BORDER.withBrightness(0.1));
    g.drawRect(reduced, 1.0f);
    if (ticked)
    {
        g.setColour (COLOUR_SELECT);
        g.fillRect(tickBounds.reduced(1.5f));
    }
}
void GrapeLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
    auto fromCentre = dynamic_cast<CentredSlider*> (&slider) != nullptr;
    auto radius = KNOB_WIDTH / 2 - 4.0f;
    auto centreX = (float) x + (float) width  * 0.5f;
    auto centreY = (float) y + (float) height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto fromAngle = fromCentre ? (rotaryStartAngle + rotaryEndAngle) / 2 :rotaryStartAngle;
    {
        juce::Path p;
        p.addArc(rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, true);
        juce::PathStrokeType strokeType(SLIT_WIDTH);
        g.setColour (COLOUR_PIT.withAlpha(slider.isEnabled() ? 1.0f : 0.3f));
        g.strokePath(p, strokeType);
    }
    {
        juce::Path p;
        p.addArc(rx, ry, rw, rw, fromAngle, angle, true);
        juce::PathStrokeType strokeType(SLIT_WIDTH);
        g.setColour (COLOUR_SELECT.withAlpha(slider.isEnabled() ? 1.0f : 0.3f));
        g.strokePath(p, strokeType);
    }
    {
        juce::Path p;
        p.addEllipse(-POINTER_RADIUS/2, -radius*0.8, POINTER_RADIUS, POINTER_RADIUS);
        p.applyTransform (juce::AffineTransform::rotation (angle).translated (centreX, centreY));
        g.setColour (COLOUR_SELECT.withAlpha(slider.isEnabled() ? 1.0f : 0.3f));
        g.fillPath(p);
    }
}
void GrapeLookAndFeel::drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown, int buttonX, int buttonY, int buttonW, int buttonH, juce::ComboBox& box)
{
//    {
//        juce::Path p;
//        juce::PathStrokeType strokeType(BORDER_WIDTH);
//        juce::Rectangle<float> boxBounds (0, 0, width, height);
//        p.addRectangle(boxBounds.reduced(BORDER_WIDTH / 2));
//        g.setColour (COLOUR_BORDER);
//        g.strokePath(p, strokeType);
//    }
    {
        juce::Rectangle<float> boxBounds (0, 0, width, height);
        auto reduced = boxBounds.reduced(BORDER_WIDTH / 2);
        auto outlineColour = box.findColour(juce::ComboBox::outlineColourId);
        {
            juce::Path p;
            p.addLineSegment(juce::Line(reduced.getTopLeft(), reduced.getTopRight()), 1);
            g.setColour (outlineColour.withBrightness(0.25));
            g.strokePath(p, juce::PathStrokeType(0.5));
        }
        {
            juce::Path p;
            p.addLineSegment(juce::Line(reduced.getTopRight(), reduced.getBottomRight()), 1);
            g.setColour (outlineColour.withBrightness(0.08));
            g.strokePath(p, juce::PathStrokeType(0.5));
        }
        {
            juce::Path p;
            p.addLineSegment(juce::Line(reduced.getBottomRight(), reduced.getBottomLeft()), 1);
            g.setColour (outlineColour.withBrightness(0.08));
            g.strokePath(p, juce::PathStrokeType(1));
        }
        {
            juce::Path p;
            p.addLineSegment(juce::Line(reduced.getBottomLeft(), reduced.getTopLeft()), 1);
            g.setColour (outlineColour.withBrightness(0.25));
            g.strokePath(p, juce::PathStrokeType(0.5));
        }
    }
    {
        juce::Rectangle<float> arrowZone (width - ARROW_ZONE_WIDTH, 0, ARROW_ZONE_WIDTH - 6.0, height);
        juce::Path p;
        p.addTriangle(arrowZone.getCentreX() - 4, arrowZone.getCentreY() - 2,
                      arrowZone.getCentreX() + 4, arrowZone.getCentreY() - 2,
                      arrowZone.getCentreX(),     arrowZone.getCentreY() + 2);
        g.setColour(box.findColour(juce::ComboBox::arrowColourId).withAlpha(box.isEnabled() ? 1.0f : 0.3f));
        g.fillPath(p);
    }

//    auto cornerSize = box.findParentComponentOfClass<ChoicePropertyComponent>() != nullptr ? 0.0f : 3.0f;
//    Rectangle<int> boxBounds (0, 0, width, height);
//
//    g.setColour (box.findColour (ComboBox::backgroundColourId));
//    g.fillRoundedRectangle (boxBounds.toFloat(), cornerSize);
//
//    g.setColour (box.findColour (ComboBox::outlineColourId));
//    g.drawRoundedRectangle (boxBounds.toFloat().reduced (0.5f, 0.5f), cornerSize, 1.0f);
//
    
//    path.startNewSubPath ((float) arrowZone.getX() + 3.0f, (float) arrowZone.getCentreY() - 2.0f);
//    path.lineTo ((float) arrowZone.getCentreX(), (float) arrowZone.getCentreY() + 3.0f);
//    path.lineTo ((float) arrowZone.getRight() - 3.0f, (float) arrowZone.getCentreY() - 2.0f);
//
//    g.setColour (box.findColour (ComboBox::arrowColourId).withAlpha ((box.isEnabled() ? 0.9f : 0.2f)));
//    g.strokePath (path, PathStrokeType (2.0f));
}
void GrapeLookAndFeel::positionComboBoxText (juce::ComboBox& box, juce::Label& label)
{
    label.setBounds (1, 1, box.getWidth() - ARROW_ZONE_WIDTH, box.getHeight() - 2);
    label.setFont (getComboBoxFont (box));
}
