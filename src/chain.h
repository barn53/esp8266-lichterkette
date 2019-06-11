#pragma once
#include <NeoPixelAnimator.h>
#include <NeoPixelBus.h>

class Chain {
public:
    Chain(uint16_t pixelCount);

    void begin();
    void loop();

    void action(uint8_t id);

private:
    void red();
    void rainbow();
    void strobo();
    void blue();

    void stopAnimations();
    void stroboAnimation(const AnimationParam& param);

    const uint16_t m_pixel_count;
    const uint16_t m_animations_count { 8 };

    NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> m_strip;
    NeoPixelAnimator m_animations;
    static NeoGamma<NeoGammaTableMethod> m_colorGamma;
};
