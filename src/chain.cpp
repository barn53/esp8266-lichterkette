#include "chain.h"

Chain::Chain(uint16_t pixelCount)
    : m_pixel_count(pixelCount)
    , m_strip(pixelCount)
    , m_animations(m_animations_count)
{
}

void Chain::begin()
{
    m_strip.Begin();
    red();
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
        red();
    } else if (id == 2) {
        rainbow();
    } else if (id == 3) {
        strobo();
    } else if (id == 4) {
        blue();
    } else if (id == 5) {
        rainbow();
    } else if (id == 6) {
        rainbow();
    } else if (id == 7) {
        rainbow();
    } else if (id == 8) {
        rainbow();
    }
}

void Chain::red()
{
    for (uint16_t index = 0; index < m_pixel_count; ++index) {
        m_strip.SetPixelColor(index, RgbColor(0x0a, 0x0, 0x0));
    }
}
void Chain::rainbow()
{
    for (uint16_t index = 0; index < m_pixel_count; ++index) {
        float hue(index / static_cast<float>(m_pixel_count));
        HsbColor c(hue, 1, 0.4);
        m_strip.SetPixelColor(index, c);
    }
}

void Chain::stopAnimations()
{
    for (uint16_t index = 0; index < m_animations_count; ++index) {
        m_animations.StopAnimation(index);
    }
}

void Chain::stroboAnimation(const AnimationParam& param)
{
    static bool toggler(true);
    RgbColor color1(0x0a, 0x0a, 0x0a);
    RgbColor color2(0x0, 0x0a, 0x0);

    if (param.state == AnimationState_Completed) {
        toggler = !toggler;
        for (uint16_t index = 0; index < m_pixel_count; ++index) {
            if (toggler) {
                m_strip.SetPixelColor(index, color1);
            } else {
                m_strip.SetPixelColor(index, color2);
            }
        }
        m_animations.RestartAnimation(param.index);
    }
}

void Chain::strobo()
{
    m_animations.StartAnimation(0, 100, std::bind(&Chain::stroboAnimation, this, std::placeholders::_1));
}

void Chain::blue()
{
    for (uint16_t index = 0; index < m_pixel_count; ++index) {
        m_strip.SetPixelColor(index, RgbColor(0x0, 0x0, 0x99));
    }
}
