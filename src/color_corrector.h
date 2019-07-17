#pragma once
#include <NeoPixelBus.h>

class ColorCorrector {
public:
    ColorCorrector();

    template <typename C>
    RgbColor get(const C& c);

    void brighter();
    void darker();

    static const uint8_t brightnessStep = 20;

private:
    NeoGamma<NeoGammaTableMethod> m_colorGamma;
    int16_t m_darkenBy;
};
