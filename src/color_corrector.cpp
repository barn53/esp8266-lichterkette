#include "color_corrector.h"

ColorCorrector::ColorCorrector()
{
    m_darkenBy = 0;
}

template RgbColor ColorCorrector::get<RgbColor>(const RgbColor& c);
template RgbColor ColorCorrector::get<HsbColor>(const HsbColor& c);

template <typename C>
RgbColor ColorCorrector::get(const C& c)
{
    RgbColor rgb(c);
    rgb.Darken(m_darkenBy);
    return m_colorGamma.Correct(rgb);
}

void ColorCorrector::brighter()
{
    m_darkenBy -= brightnessStep;
    ;
    if (m_darkenBy < 0) {
        m_darkenBy = 0;
    }
    Serial.printf("darken by: %u\n", m_darkenBy);
}
void ColorCorrector::darker()
{
    m_darkenBy += brightnessStep;
    if (m_darkenBy > 255) {
        m_darkenBy = 255;
    }
    Serial.printf("darken by: %u\n", m_darkenBy);
}
