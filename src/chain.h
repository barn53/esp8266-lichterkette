#pragma once

#include "state.h"
#include <NeoPixelAnimator.h>
#include <NeoPixelBus.h>

using Feature = NeoGrbFeature;
//using Feature = NeoRgbFeature;

class State;

class Chain {
public:
    Chain(uint16_t pixelCount);

    void begin();
    void loop();
    void first();

    void action(uint8_t id);

private:
    void init();
    void color(const RgbColor& c);

    void rotateAnimation(const AnimationParam& param);
    void stroboAnimation(const AnimationParam& param);

    void state2World();

    const uint16_t m_pixel_count;
    const uint16_t m_animations_count;

    NeoPixelBus<Feature, Neo800KbpsMethod> m_strip;
    NeoPixelAnimator m_animations;
    State m_state;
    bool randomSeeded;
};
