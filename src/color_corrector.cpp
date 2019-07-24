#include "color_corrector.h"

ColorCorrector::ColorCorrector()
{
}

template RgbColor ColorCorrector::get<RgbColor>(const RgbColor& c, int16_t darkenBy);
template RgbColor ColorCorrector::get<HsbColor>(const HsbColor& c, int16_t darkenBy);

template <typename C>
RgbColor ColorCorrector::get(const C& c, int16_t darkenBy)
{
    RgbColor rgb(c);
    rgb.Darken(darkenBy);
    rgb = m_colorGamma.Correct(rgb);
    return rgb;
}
