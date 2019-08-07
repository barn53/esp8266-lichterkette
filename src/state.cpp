#include "state.h"

State::State()
    : m_speed(0)
    , m_stroboSpeed(0)
    , m_darkenBy(128)
    , m_mode(Mode::GRADIENT)
    , m_dirty(false)
{
}

void State::setColor(const RgbColor& c)
{
    if (m_mode == Mode::GRADIENT
        || m_mode == Mode::ALTERNATING) {
        m_dirty = true;
        m_color_1 = m_color_2;
        m_color_2 = c;
    }
}

void State::toggleGradientAlternating()
{
    if (m_mode == Mode::GRADIENT) {
        m_dirty = true;
        m_mode = Mode::ALTERNATING;
    } else if (m_mode == Mode::ALTERNATING) {
        m_dirty = true;
        m_mode = Mode::GRADIENT;
    }
}
void State::setColorMode()
{
    if (m_mode != Mode::GRADIENT
        && m_mode != Mode::ALTERNATING) {
        m_dirty = true;
        m_mode = Mode::GRADIENT;
    }
}
void State::setRainbowMode()
{
    if (m_mode != Mode::RAINBOW) {
        m_dirty = true;
        m_mode = Mode::RAINBOW;
    }
}
void State::setStroboMode()
{
    if (m_mode != Mode::STROBO) {
        m_dirty = true;
        m_mode = Mode::STROBO;
    }
}
void State::setBlinkyBlinkyMode()
{
    if (m_mode != Mode::BLINKY_BLINKY) {
        m_dirty = true;
        m_mode = Mode::BLINKY_BLINKY;
    }
}

void State::setOffMode()
{
    if (m_mode != Mode::OFF) {
        m_dirty = true;
        m_mode = Mode::OFF;
    }
}

void State::slower()
{
    if (m_mode == Mode::GRADIENT
        || m_mode == Mode::ALTERNATING
        || m_mode == Mode::RAINBOW) {
        if (m_speed > 0) {
            --m_speed;
            m_dirty = true;
        }
    } else if (m_mode == Mode::STROBO) {
        if (m_stroboSpeed > 0) {
            --m_stroboSpeed;
            m_dirty = true;
        }
    }
}
void State::faster()
{
    if (m_mode == Mode::GRADIENT
        || m_mode == Mode::ALTERNATING
        || m_mode == Mode::RAINBOW) {
        if (m_speed < 5) {
            ++m_speed;
            m_dirty = true;
        }
    } else if (m_mode == Mode::STROBO) {
        if (m_stroboSpeed < 4) {
            ++m_stroboSpeed;
            m_dirty = true;
        }
    }
}

void State::brighter()
{
    auto was(m_darkenBy);
    m_darkenBy -= 30;
    if (m_darkenBy < 0) {
        m_darkenBy = 0;
    }
    m_dirty = (was != m_darkenBy);
}
void State::darker()
{
    auto was(m_darkenBy);
    m_darkenBy += 30;
    if (m_darkenBy > 255) {
        m_darkenBy = 255;
    }
    m_dirty = (was != m_darkenBy);
}

const char* State::modeToString(Mode m)
{
    switch (m) {
    case Mode::GRADIENT:
        return "Mode::GRADIENT";
        break;
    case Mode::ALTERNATING:
        return "Mode::ALTERNATING";
        break;
    case Mode::RAINBOW:
        return "Mode::RAINBOW";
        break;
    case Mode::STROBO:
        return "Mode::STROBO";
        break;
    case Mode::BLINKY_BLINKY:
        return "Mode::BLINKY_BLINKY";
    case Mode::OFF:
        return "Mode::OFF";
        break;
    }
    return "";
}

void State::log() const
{
    Serial.printf("---------------------------------\n");
    Serial.printf("Dirty: %s\n", (m_dirty ? "true" : "false"));
    Serial.printf("Mode: %s\n", modeToString(m_mode));
    Serial.printf("Color 1: R: %u G: %u B: %u\n", m_color_1.R, m_color_1.G, m_color_1.B);
    Serial.printf("Color 2: R: %u G: %u B: %u\n", m_color_2.R, m_color_2.G, m_color_2.B);
    Serial.printf("Darken by: %u\n", m_darkenBy);
    Serial.printf("Speed: %u - Strobo Speed: %u\n", m_speed, m_stroboSpeed);
}

RgbColor State::getCorrectedColor1()
{
    return correctColor(m_color_1);
}
RgbColor State::getCorrectedColor2()
{
    return correctColor(m_color_2);
}
