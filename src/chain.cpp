#include "chain.h"
#include "state.h"

Chain::Chain(uint16_t pixelCount)
    : m_pixel_count(pixelCount)
    , m_animations_count(pixelCount)
    , m_strip(pixelCount)
    , m_animations(m_animations_count)
    , randomSeeded(false)
{
}

void Chain::begin()
{
    m_strip.Begin();
    init();
    m_strip.Show();
}

void Chain::first()
{
    color(RgbColor(0x0, 0x0, 0xff));
    color(RgbColor(0x0, 0xff, 0xff));
    state2World();
}

void Chain::loop()
{
    m_animations.UpdateAnimations();
    m_strip.Show();
}

void Chain::action(uint8_t id)
{
    if (!randomSeeded) {
        randomSeeded = true;
        randomSeed(millis());
    }

    if (id == 1) {
        color(RgbColor(0xff, 0x0, 0x0));
    } else if (id == 2) {
        color(RgbColor(0x0, 0xff, 0x0));
    } else if (id == 3) {
        color(RgbColor(0x0, 0x0, 0xff));
    } else if (id == 4) {
        color(RgbColor(0xff, 0x0, 0xaa));
    } else if (id == 5) {
        color(RgbColor(0x0, 0xff, 0xdd));
    } else if (id == 6) {
        color(RgbColor(0xff, 0xff, 0x0));
    } else if (id == 7) {
        color(RgbColor(0xff, 0xcc, 0x0));
    } else if (id == 8) {
        color(RgbColor(0xaa, 0xcc, 0xff));
    } else if (id == 9) {
        color(RgbColor(random(0x100), random(0x100), random(0x100)));
    } else if (id == 10) {
        m_state.setRainbowMode();
    } else if (id == 11) {
        // blinky, blinky
    } else if (id == 12) {
        // strobo
    } else if (id == 13) {
        m_state.toggleGradientAlternating();
    } else if (id == 14) {
        m_state.faster();
    } else if (id == 15) {
        m_state.slower();
    } else if (id == 16) {
        m_state.brighter();
    } else if (id == 17) {
        m_state.darker();
    } else if (id == 18) {
        // aus
    }
    state2World();
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
    m_strip.ClearTo(RgbColor(0x0, 0x0, 0x0));
    m_strip.SetPixelColor(0, m_state.correctColor(RgbColor(0xff, 0x0, 0x0)));
    m_animations.StartAnimation(0, 100, std::bind(&Chain::rotateAnimation, this, std::placeholders::_1));
}

void Chain::color(const RgbColor& c)
{
    m_state.setColor(c);
    m_state.setColorMode();
    state2World();
}

void Chain::state2World()
{
    if (m_state.isDirty()) {
        m_animations.StopAll();
        switch (m_state.getMode()) {
        case State::Mode::GRADIENT: {
            for (uint16_t index = 0; index < m_pixel_count; ++index) {
                float progress(index / static_cast<float>(m_pixel_count));
                m_strip.SetPixelColor(index, m_state.correctColor(RgbColor::LinearBlend(m_state.getColor1(), m_state.getColor2(), progress)));
            }
            break;
        }
        case State::Mode::ALTERNATING: {
            for (uint16_t index = 0; index < m_pixel_count; ++index) {
                if (index % 2 == 0) {
                    m_strip.SetPixelColor(index, m_state.getCorrectedColor1());
                } else {
                    m_strip.SetPixelColor(index, m_state.getCorrectedColor2());
                }
            }
            break;
        }
        case State::Mode::RAINBOW: {
            for (uint16_t index = 0; index < m_pixel_count; ++index) {
                float hue(index / static_cast<float>(m_pixel_count));
                HsbColor c(hue, 1, 1);
                m_strip.SetPixelColor(index, m_state.correctColor(c));
            }
            break;
        }
        case State::Mode::STROBO:
            m_animations.StartAnimation(0, 80, std::bind(&Chain::stroboAnimation, this, std::placeholders::_1));
            break;
        default:
            break;
        }
        m_state.setClean();
    }
}
