#include "chain.h"

Chain::Chain(uint16_t pixelCount)
    : m_pixel_count(pixelCount)
    , m_animations_count(pixelCount)
    , m_strip(pixelCount)
    , m_animations(m_animations_count)
{
}

void Chain::begin()
{
    m_strip.Begin();
    init();
    m_strip.Show();
}

void Chain::loop()
{
    m_animations.UpdateAnimations();
    m_strip.Show();
}

void Chain::action(uint8_t id)
{
    stopAnimations();

    if (id == 1) {
        color(RgbColor(0xff, 0x0, 0x0));
    } else if (id == 2) {
        color(RgbColor(0x0, 0xff, 0xff));
    } else if (id == 3) {
        color(RgbColor(0x0, 0x0, 0xff));
    } else if (id == 4) {
        color(RgbColor(0xff, 0x0, 0xff));
    } else if (id == 5) {
        color(RgbColor(0xff, 0x0, 0xff));
    } else if (id == 6) {
        color(RgbColor(0xff, 0x0, 0xff));
    } else if (id == 7) {
        rainbow();
    } else if (id == 8) {
        // randomColor();
    } else if (id == 9) {
        // gradientAlternating();
    } else if (id == 10) {
        // rotateCurent();
    } else if (id == 11) {
        // randomAnimated();
    } else if (id == 12) {
        strobo();
    } else if (id == 13) {
        brighter();
    } else if (id == 14) {
        darker();
    } else if (id == 15) {
        off();
    }
}

void Chain::stopAnimations()
{
    for (uint16_t index = 0; index < m_animations_count; ++index) {
        m_animations.StopAnimation(index);
    }
}

void Chain::rotateAnimation(const AnimationParam& param)
{
    if (param.state == AnimationState_Completed) {
        m_strip.RotateRight(1);
        m_animations.RestartAnimation(param.index);
    }
}

void Chain::stroboAnimation(const AnimationParam& param)
{
    RgbColor color1(0xff, 0xff, 0xff);
    RgbColor color2(0x0, 0x0, 0x0);

    if (param.progress <= 0.1) {
        for (uint16_t index = 0; index < m_pixel_count; ++index) {
            m_strip.SetPixelColor(index, color1);
        }
    } else {
        for (uint16_t index = 0; index < m_pixel_count; ++index) {
            m_strip.SetPixelColor(index, color2);
        }
    }

    if (param.state == AnimationState_Completed) {
        m_animations.RestartAnimation(param.index);
    }
}

void Chain::init()
{
    for (uint16_t index = 0; index < m_pixel_count; ++index) {
        if (index == 0) {
            m_strip.SetPixelColor(index, m_colorCorrector.get(RgbColor(0xff, 0x0, 0x0)));
        } else {
            m_strip.SetPixelColor(index, RgbColor(0x0, 0x0, 0x0));
        }
    }
    m_animations.StartAnimation(0, 100, std::bind(&Chain::rotateAnimation, this, std::placeholders::_1));
}

void Chain::color()
{
    color(RgbColor(0x0, 0xff, 0x0));
}

void Chain::color(const RgbColor& c)
{
    for (uint16_t index = 0; index < m_pixel_count; ++index) {
        m_strip.SetPixelColor(index, m_colorCorrector.get(c));
    }
}
void Chain::rainbow()
{
    for (uint16_t index = 1; index < m_pixel_count + 1; ++index) {
        float hue(index / static_cast<float>(m_pixel_count));
        HsbColor c(hue, 1, 1);
        m_strip.SetPixelColor(index - 1, m_colorCorrector.get(c));
    }
}
void Chain::strobo()
{
    m_animations.StartAnimation(0, 80, std::bind(&Chain::stroboAnimation, this, std::placeholders::_1));
}

void Chain::brighter()
{
    for (uint16_t index = 0; index < m_pixel_count; ++index) {
        m_strip.SetPixelColor(index, m_strip.GetPixelColor(index).Lighten(m_colorCorrector.brightnessStep));
    }
    m_colorCorrector.brighter();
}
void Chain::darker()
{
    for (uint16_t index = 0; index < m_pixel_count; ++index) {
        m_strip.SetPixelColor(index, m_strip.GetPixelColor(index).Darken(m_colorCorrector.brightnessStep));
    }
    m_colorCorrector.darker();
}
void Chain::off()
{
    for (uint16_t index = 0; index < m_pixel_count; ++index) {
        m_strip.ClearTo(RgbColor(0x0, 0x0, 0x0));
    }
}
