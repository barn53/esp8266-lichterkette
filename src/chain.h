#pragma once

#include "state.h"
#include <NeoPixelAnimator.h>
#include <NeoPixelBus.h>
#include <vector>

//using Feature = NeoGrbFeature;
using Feature = NeoRgbFeature;

class State;

class Chain {
public:
    Chain(uint16_t pixelCount);

    void begin();
    void loop();
    void first();

    void off();

    void action(uint8_t id);

private:
    void init();
    void color(const RgbColor& c);

    void rotateAnimation(const AnimationParam& param);
    void stroboAnimation(const AnimationParam& param);
    void blinkyBlinkyAnimation(const AnimationParam& param);
    void rotationAnimation(const AnimationParam& param);
    void offAnimation(const AnimationParam& param);

    RgbColor getRandomColor() const;
    void state2World();
    void rainbow2World();
    void strobo2World();
    void clearColors();
    void blinkyBlinky2World();
    void initColorsForRotation();
    void rotation2World();
    void off2World();

    const uint16_t m_pixel_count;
    const uint16_t m_animations_count;

    std::vector<RgbColor> m_colors_1;
    std::vector<RgbColor> m_colors_2;
    std::vector<int16_t> m_counters;

    NeoPixelBus<Feature, Neo800KbpsMethod> m_strip;
    NeoPixelAnimator m_animations;
    State m_state;
    bool randomSeeded;
};
