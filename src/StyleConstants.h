#pragma once

#include <JuceHeader.h>

namespace styles {
namespace colour {
// text
const juce::Colour TEXT = juce::Colour(200, 200, 200);
const juce::Colour WARNING = juce::Colour(190, 190, 80);
const juce::Colour ERROR = juce::Colour(190, 40, 80);
// background
const juce::Colour BORDER = juce::Colour(30, 30, 30);
const juce::Colour BACKGROUND = juce::Colour(40, 40, 40);
const juce::Colour PANEL_NAME_BACKGROUND = juce::Colour(50, 50, 50);
const juce::Colour POPUP_BACKGROUND = juce::Colour(30, 30, 30);
// components
const juce::Colour SELECT = juce::Colour(170, 100, 210);
const juce::Colour CONTROLLED = juce::Colour(170, 170, 170);
const juce::Colour ANALYSER_LINE = juce::Colour(100, 190, 140);
const juce::Colour ANALYSER_LINE2 = juce::Colour(60, 100, 150);
const juce::Colour PIT = juce::Colour(0, 0, 0);
} // namespace colour
// font
constexpr float PANEL_NAME_FONT_SIZE = 15.0f;
constexpr float PARAM_LABEL_FONT_SIZE = 14.0f;
constexpr float PARAM_VALUE_LABEL_FONT_SIZE = 14.0f;
// knob
constexpr float MAX_KNOB_WIDTH = 40.0f;
constexpr float SLIT_WIDTH = 2.0f;
constexpr float POINTER_RADIUS = 4.0f;
// combo box
constexpr float BORDER_WIDTH = 1.0f;
constexpr float ARROW_ZONE_WIDTH = 18.0f;
// layout
constexpr float PANEL_NAME_HEIGHT = 26.0f;
constexpr float PANEL_MARGIN = 2.0f;
constexpr float LOCAL_MARGIN = 2.0f;
constexpr float LABEL_HEIGHT = 20.0f;
constexpr float COMBO_BOX_HEIGHT = 28.0f;
constexpr float SLIDER_WIDTH = 60.0f;
}