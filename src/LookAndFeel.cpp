#include "LookAndFeel.h"

GrapeLookAndFeel::GrapeLookAndFeel(bool controlled) : controlled(controlled)
{
    setColour (juce::ComboBox::textColourId, COLOUR_TEXT);
    setColour (juce::ComboBox::arrowColourId, COLOUR_TEXT);
    setColour (juce::ComboBox::outlineColourId, COLOUR_BORDER);
    setColour (juce::PopupMenu::backgroundColourId, COLOUR_BACKGROUND);
    setColour (juce::PopupMenu::highlightedBackgroundColourId, COLOUR_SELECT.withBrightness(0.4));
//    setColour (juce::TooltipWindow::outlineColourId, COLOUR_BORDER);
//    setColour (juce::TooltipWindow::backgroundColourId, COLOUR_BACKGROUND);
//    setColour (juce::Slider::textBoxOutlineColourId, COLOUR_BORDER);
//    setColour (juce::Slider::textBoxBackgroundColourId, COLOUR_BACKGROUND);
    setColour (juce::BubbleComponent::outlineColourId, COLOUR_BORDER);
    setColour (juce::BubbleComponent::backgroundColourId, COLOUR_PIT);
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
    if(isEnabled) {
        g.setColour (COLOUR_PIT);
        g.fillRect(tickBounds);
    }
    g.setColour (COLOUR_BORDER.withBrightness(isEnabled ? 0.1 : 0.14));
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
    auto knobWidth = juce::jmin(MAX_KNOB_WIDTH, juce::jmin(width, height));
    auto fromCentre = dynamic_cast<CentredSlider*> (&slider) != nullptr;
    auto radius = knobWidth / 2 - 4.0f;
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
        auto color = controlled ? COLOUR_CONTROLLED : COLOUR_SELECT;
        g.setColour (color.withAlpha(slider.isEnabled() ? 1.0f : 0.3f));
//        g.setColour (COLOUR_SELECT.withAlpha(slider.isEnabled() ? 1.0f : 0.3f));
        g.strokePath(p, strokeType);
    }
    {
        juce::Path p;
        p.addEllipse(-POINTER_RADIUS/2, -radius*0.8, POINTER_RADIUS, POINTER_RADIUS);
        p.applyTransform (juce::AffineTransform::rotation (angle).translated (centreX, centreY));
        auto color = controlled ? COLOUR_CONTROLLED : COLOUR_SELECT;
        g.setColour (color.withAlpha(slider.isEnabled() ? 1.0f : 0.3f));
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
void GrapeLookAndFeel::drawLabel (juce::Graphics& g, juce::Label& label)
{
    g.fillAll (label.findColour (juce::Label::backgroundColourId));

    if (! label.isBeingEdited())
    {
        auto alpha = label.isEnabled() ? 1.0f : 0.5f;
        const juce::Font font (getLabelFont (label));

        g.setColour (label.findColour (juce::Label::textColourId).withMultipliedAlpha (alpha));
        g.setFont (font);

        auto textArea = getLabelBorderSize (label).subtractedFrom (label.getLocalBounds());
        
        // MIDI CC の番号のみを表示するために...
        auto text = label.getText();
        auto idx = text.indexOfChar(':');
        if(idx >= 0) {
            text = text.substring(0, idx);
        }
        g.drawFittedText (text, textArea, label.getJustificationType(),
                          juce::jmax (1, (int) ((float) textArea.getHeight() / font.getHeight())),
                          label.getMinimumHorizontalScale());

        g.setColour (label.findColour (juce::Label::outlineColourId).withMultipliedAlpha (alpha));
    }
    else if (label.isEnabled())
    {
        g.setColour (label.findColour (juce::Label::outlineColourId));
    }

    g.drawRect (label.getLocalBounds());
}
void GrapeLookAndFeel::drawPopupMenuBackground (juce::Graphics& g, int width, int height)
{
    auto background = findColour (juce::PopupMenu::backgroundColourId);

    g.fillAll (background);
    g.setColour (background.overlaidWith (juce::Colour (0x2badd8e6)));

//    for (int i = 0; i < height; i += 3)
//        g.fillRect (0, i, width, 1);

   #if ! JUCE_MAC
    g.setColour (findColour (PopupMenu::textColourId).withAlpha (0.6f));
    g.drawRect (0, 0, width, height);
   #endif
}
void GrapeLookAndFeel::drawPopupMenuItem (juce::Graphics& g, const juce::Rectangle<int>& area,
                                        const bool isSeparator, const bool isActive,
                                        const bool isHighlighted, const bool isTicked,
                                        const bool hasSubMenu, const juce::String& text,
                                        const juce::String& shortcutKeyText,
                                        const juce::Drawable* icon, const juce::Colour* const textColourToUse)
{
    if (isSeparator)
    {
        auto r  = area.reduced (5, 0);
        r.removeFromTop (juce::roundToInt (((float) r.getHeight() * 0.5f) - 0.5f));

        g.setColour (findColour (juce::PopupMenu::textColourId).withAlpha (0.3f));
        g.fillRect (r.removeFromTop (1));
    }
    else
    {
        auto textColour = (textColourToUse == nullptr ? findColour (juce::PopupMenu::textColourId)
                                                      : *textColourToUse);

        auto r  = area.reduced (1);

        if (isHighlighted && isActive)
        {
            g.setColour (findColour (juce::PopupMenu::highlightedBackgroundColourId));
            g.fillRect (r);

            g.setColour (findColour (juce::PopupMenu::highlightedTextColourId));
        }
        else
        {
            g.setColour (textColour.withMultipliedAlpha (isActive ? 1.0f : 0.5f));
        }

        r.reduce (juce::jmin (5, area.getWidth() / 20), 0);

        auto font = getPopupMenuFont();

        auto maxFontHeight = (float) r.getHeight() / 1.3f;

        if (font.getHeight() > maxFontHeight)
            font.setHeight (maxFontHeight);

        g.setFont (font);

        auto iconArea = r.removeFromLeft (juce::roundToInt (maxFontHeight)).toFloat();

        if (icon != nullptr)
        {
            icon->drawWithin (g, iconArea, juce::RectanglePlacement::centred | juce::RectanglePlacement::onlyReduceInSize, 1.0f);
            r.removeFromLeft (juce::roundToInt (maxFontHeight * 0.5f));
        }
        else if (isTicked)
        {
            auto tick = getTickShape (1.0f);
//            g.fillPath (tick, tick.getTransformToScaleToFit (iconArea.reduced (iconArea.getWidth() / 5, 0).toFloat(), true));
            g.fillPath (tick, tick.getTransformToScaleToFit (iconArea.reduced (iconArea.getWidth() / 3, 0).toFloat(), true));
        }

        if (hasSubMenu)
        {
            auto arrowH = 0.6f * getPopupMenuFont().getAscent();

            auto x = static_cast<float> (r.removeFromRight ((int) arrowH).getX());
            auto halfH = static_cast<float> (r.getCentreY());

            juce::Path path;
            path.startNewSubPath (x, halfH - arrowH * 0.5f);
            path.lineTo (x + arrowH * 0.6f, halfH);
            path.lineTo (x, halfH + arrowH * 0.5f);

            g.strokePath (path, juce::PathStrokeType (2.0f));
        }

        r.removeFromRight (3);
        g.drawFittedText (text, r, juce::Justification::centredLeft, 1);

        if (shortcutKeyText.isNotEmpty())
        {
            auto f2 = font;
            f2.setHeight (f2.getHeight() * 0.75f);
            f2.setHorizontalScale (0.95f);
            g.setFont (f2);

            g.drawText (shortcutKeyText, r, juce::Justification::centredRight, true);
        }
    }
}

juce::Path GrapeLookAndFeel::getTickShape (float height)
{
    auto w = height / 4;
    auto h = height / 4;
    auto x = height / 2 - w / 2;
    auto y = height / 2 - h / 2;

    juce::Path path;
    path.addEllipse(x, y, w, h);

    return path;
}
