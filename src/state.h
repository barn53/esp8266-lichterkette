#pragma once
#include "color_corrector.h"
#include <NeoPixelBus.h>

class State {

public:
    enum class Mode {
        GRADIENT,
        ALTERNATING,
        RAINBOW,
        STROBO,
        BLINKY_BLINKY
    };

    State();

    void setColor(const RgbColor& c);
    void toggleGradientAlternating();
    void setColorMode();
    void setRainbowMode();
    void setStroboMode();
    void setBlinkyBlinkyMode();

    void brighter();
    void darker();
    void slower();
    void faster();

    Mode getMode() const { return m_mode; }
    bool isDirty() const { return m_dirty; }
    void setClean() { m_dirty = false; }

    RgbColor getCorrectedColor1();
    RgbColor getCorrectedColor2();
    RgbColor getColor1() const { return m_color_1; }
    RgbColor getColor2() const { return m_color_2; }
    int8_t getSpeed() const { return m_speed; }
    int8_t getStroboSpeed() const { return m_stroboSpeed; }

    void log() const;

    template <typename C>
    RgbColor correctColor(const C& c)
    {
        return m_colorCorrector.get(c);
    }

private:
    static const char* modeToString(Mode m);

    ColorCorrector m_colorCorrector;
    RgbColor m_color_1; // uncorrected colors
    RgbColor m_color_2; // uncorrected colors
    int8_t m_speed;
    int8_t m_stroboSpeed;
    Mode m_mode;
    bool m_dirty;
};
