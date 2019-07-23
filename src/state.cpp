#include "state.h"

State::State()
    : m_colorSwitch(0)
    , m_speed(0)
    , m_mode(Mode::GRADIENT)
    , m_dirty(false)
{
}

void State::setColor(const RgbColor& c)
{
    if (m_mode == Mode::GRADIENT
        || m_mode == Mode::ALTERNATING) {
        m_dirty = true;
        if (m_colorSwitch == 0) {
            m_colorSwitch = 1;
            m_color_1 = c;
        } else {
            m_colorSwitch = 0;
            m_color_2 = c;
        }
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

void State::slower()
{
    if (m_mode == Mode::GRADIENT
        || m_mode == Mode::ALTERNATING
        || m_mode == Mode::RAINBOW) {
        if (m_speed > 0) {
            --m_speed;
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
    }
}

void State::brighter()
{
    m_dirty = m_colorCorrector.brighter() || m_dirty;
}
void State::darker()
{
    m_dirty = m_colorCorrector.darker() || m_dirty;
}

RgbColor State::getCorrectedColor1()
{
    return correctColor(m_color_1);
}
RgbColor State::getCorrectedColor2()
{
    return correctColor(m_color_2);
}
