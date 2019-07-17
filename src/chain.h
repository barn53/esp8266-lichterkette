#pragma once
#include "color_corrector.h"

#include <NeoPixelAnimator.h>
#include <NeoPixelBus.h>

using Feature = NeoGrbFeature;
//using Feature = NeoRgbFeature;

class Chain {
public:
    Chain(uint16_t pixelCount);

    void begin();
    void loop();
    void color();

    void action(uint8_t id);

private:
    void init();
    void color(const RgbColor& c);
    void rainbow();
    void strobo();

    void brighter();
    void darker();
    void off();

    void stopAnimations();

    void rotateAnimation(const AnimationParam& param);
    void stroboAnimation(const AnimationParam& param);

    const uint16_t m_pixel_count;
    const uint16_t m_animations_count;

    NeoPixelBus<Feature, Neo800KbpsMethod> m_strip;
    NeoPixelAnimator m_animations;
    ColorCorrector m_colorCorrector;
};
