#pragma once
#include <NeoPixelBus.h>

class ColorCorrector {
public:
    ColorCorrector();

    template <typename C>
    RgbColor get(const C& c, int16_t darkenBy);

private:
    NeoGamma<NeoGammaTableMethod> m_colorGamma;
};
