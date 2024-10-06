/**
 * @file imu.h
 * @author BBc
 * @brief
 * @version 0.1
 * @date 2024-10-02
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include <Arduino.h>
#include <M5GFX.h>
#include <lgfx/v1/panel/Panel_ST7789.hpp>

#include <Button.h>


#define display Disbuff
#define displayUpdate Displaybuff

#define LCD_MOSI_PIN 15
#define LCD_MISO_PIN -1
#define LCD_SCLK_PIN 13
#define LCD_DC_PIN 14
#define LCD_CS_PIN 5
#define LCD_RST_PIN 12
#define LCD_BUSY_PIN -1
#define LCD_BL_PIN 27


#define BUZZ_PIN 2


class CLite_GFX2 : public lgfx::LGFX_Device
{
    lgfx::Panel_ST7789 _panel_instance;
    lgfx::Bus_SPI _bus_instance;
    lgfx::Light_PWM _light_instance;

public:
    CLite_GFX2(void)
    {
        {
            auto cfg = _bus_instance.config();

            cfg.pin_mosi = LCD_MOSI_PIN;
            cfg.pin_miso = LCD_MISO_PIN;
            cfg.pin_sclk = LCD_SCLK_PIN;
            cfg.pin_dc = LCD_DC_PIN;
            cfg.freq_write = 40000000;

            _bus_instance.config(cfg);
            _panel_instance.setBus(&_bus_instance);
        }
        {
            auto cfg = _panel_instance.config();

            cfg.invert = true;
            cfg.pin_cs = LCD_CS_PIN;
            cfg.pin_rst = LCD_RST_PIN;
            cfg.pin_busy = LCD_BUSY_PIN;
            cfg.panel_width = 135;
            cfg.panel_height = 240;
            cfg.offset_x = 52;
            cfg.offset_y = 40;
            // cfg.offset_x     = 0;
            // cfg.offset_y     = 0;

            _panel_instance.config(cfg);
        }
        {                                        // バックライト制御の設定を行います。（必要なければ削除）
            auto cfg = _light_instance.config(); // バックライト設定用の構造体を取得します。

            cfg.pin_bl = 27;    // バックライトが接続されているピン番号
            cfg.invert = false; // バックライトの輝度を反転させる場合 true
            // cfg.freq   = 44100;           // バックライトのPWM周波数
            cfg.freq = 200;      // バックライトのPWM周波数
            cfg.pwm_channel = 7; // 使用するPWMのチャンネル番号

            _light_instance.config(cfg);
            _panel_instance.setLight(&_light_instance); // バックライトをパネルにセットします。
        }

        setPanel(&_panel_instance);
    }
};



namespace imuStream
{

    class LCD
    {
    private:

        /* Display */
        CLite_GFX2 lcd;
        LGFX_Sprite *Disbuff;

        void Displaybuff();
        void ColorBar();

        void frame_test();
        void color_test();
        void lcd_init();
        void lcd_test();

        inline void _tone(unsigned int frequency, unsigned long duration = 0UL) { tone(BUZZ_PIN, frequency, duration); }
        inline void _noTone() { noTone(BUZZ_PIN); }

        /* Keys */
        void key_init();
        //void key_test();
        Button btnA = Button(37, 20);
        Button btnB = Button(39, 20);
        Button btnPWR = Button(35, 20);

        void waitNext();
        bool checkNext();
        void checkReboot();

        double theta, last_theta;
        double phi, last_phi;

        // damping factor
        double alpha;

    public:
        LCD()  {}
        ~LCD() {}

        void setup();
        void loop();
        void draw_accel(float accX, float accY, float accZ);
    };

}
