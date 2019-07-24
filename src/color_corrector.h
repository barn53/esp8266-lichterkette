#pragma once
#include <NeoPixelBus.h>

class ColorCorrector {
public:
    ColorCorrector();

    template <typename C>
    RgbColor get(const C& c);

    bool brighter();
    bool darker();
    int16_t getDarkenBy() const { return m_darkenBy; }

    static const uint8_t brightnessStep = 20;

private:
    NeoGamma<NeoGammaTableMethod> m_colorGamma;
    int16_t m_darkenBy;
};
