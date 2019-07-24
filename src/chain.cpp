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
    color(RgbColor(0x0, 0xff, 0x0));
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
        color(RgbColor(0xff, 0x0, 0xff));
    } else if (id == 9) {
        color(getRandomColor());
    } else if (id == 10) {
        m_state.setRainbowMode();
    } else if (id == 11) {
        m_state.setBlinkyBlinkyMode();
    } else if (id == 12) {
        m_state.setStroboMode();
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

RgbColor Chain::getRandomColor() const
{
    return RgbColor(random(0x100), random(0x100), random(0x100));
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

    if (param.progress <= 0.3) {
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

void Chain::blinkyBlinkyAnimation(const AnimationParam& param)
{
    RgbColor c;
    if (m_blinkyBlinkyCounters[param.index] % 2 == 0) {
        c = RgbColor::LinearBlend(m_blinkyBlinkyColors_1[param.index], m_blinkyBlinkyColors_2[param.index], param.progress);
    } else {
        c = RgbColor::LinearBlend(m_blinkyBlinkyColors_2[param.index], m_blinkyBlinkyColors_1[param.index], param.progress);
    }

    m_strip.SetPixelColor(param.index, m_state.correctColor(c));

    if (param.state == AnimationState_Completed) {
        if (m_blinkyBlinkyCounters[param.index] % 2 == 0) {
            m_blinkyBlinkyColors_1[param.index] = getRandomColor();
        } else {
            m_blinkyBlinkyColors_2[param.index] = getRandomColor();
        }
        ++m_blinkyBlinkyCounters[param.index];
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
    m_state.setColorMode();
    m_state.setColor(c);
    state2World();
}

void Chain::state2World()
{
    m_state.log();
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
        case State::Mode::RAINBOW:
            rainbow2World();
            break;
        case State::Mode::BLINKY_BLINKY:
            blinkyBlinky2World();
            break;
        case State::Mode::STROBO:
            strobo2World();
            break;
        default:
            break;
        }

        if (m_state.getMode() == State::Mode::GRADIENT
            || m_state.getMode() == State::Mode::ALTERNATING
            || m_state.getMode() == State::Mode::RAINBOW) {

            // speed
        }

        m_state.setClean();
    }
}

void Chain::rainbow2World()
{
    for (uint16_t index = 0; index < m_pixel_count; ++index) {
        float hue((index / static_cast<float>(m_pixel_count)) / 1.2);
        HsbColor c(hue, 1, 1);
        m_strip.SetPixelColor(index, m_state.correctColor(c));
    }
}
void Chain::strobo2World()
{
    uint16_t duration(220);
    switch (m_state.getStroboSpeed()) {
    case 1:
        duration = 180;
        break;
    case 2:
        duration = 140;
        break;
    case 3:
        duration = 100;
        break;
    case 4:
        duration = 60;
        break;
    }
    m_animations.StartAnimation(0, duration, std::bind(&Chain::stroboAnimation, this, std::placeholders::_1));
}
void Chain::blinkyBlinky2World()
{
    m_blinkyBlinkyColors_1.clear();
    m_blinkyBlinkyColors_2.clear();
    m_blinkyBlinkyCounters.clear();
    for (uint16_t index = 0; index < m_pixel_count; ++index) {
        m_blinkyBlinkyColors_1.push_back(getRandomColor());
        m_blinkyBlinkyColors_2.push_back(getRandomColor());
        m_blinkyBlinkyCounters.push_back(0);
        m_animations.StartAnimation(index, random(2000) + 300, std::bind(&Chain::blinkyBlinkyAnimation, this, std::placeholders::_1));
    }
}
