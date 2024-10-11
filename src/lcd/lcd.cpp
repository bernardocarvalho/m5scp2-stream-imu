/**
 * @file test.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2023-05-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "lcd.h"

typedef struct
{
    double x;
    double y;
    double z;
} point_3d_t;

typedef struct
{
    point_3d_t start_point;
    point_3d_t end_point;
} line_3d_t;

typedef struct
{
    double x;
    double y;
} point_2d_t;

const double r_rand = PI / 180.0;

const double r_alpha = 19.47 * PI / 180;
const double r_gamma = 20.7 * PI / 180.0;

const double sin_alpha = sin(19.47 * PI / 180.0);
const double cos_alpha = cos(19.47 * PI / 180.0);
const double sin_gamma = sin(20.7 * PI / 180.0);
const double cos_gamma = cos(20.7 * PI / 180.0);

line_3d_t rect[12] = {
    {.start_point = {-1, -1, 1}, .end_point = {1, -1, 1}},
    {.start_point = {1, -1, 1}, .end_point = {1, 1, 1}},
    {.start_point = {1, 1, 1}, .end_point = {-1, 1, 1}},
    {.start_point = {-1, 1, 1}, .end_point = {-1, -1, 1}},
    {
        .start_point = {-1, -1, 1},
        .end_point = {-1, -1, -1},
    },
    {
        .start_point = {1, -1, 1},
        .end_point = {1, -1, -1},
    },
    {
        .start_point = {1, 1, 1},
        .end_point = {1, 1, -1},
    },
    {
        .start_point = {-1, 1, 1},
        .end_point = {-1, 1, -1},
    },
    {.start_point = {-1, -1, -1}, .end_point = {1, -1, -1}},
    {.start_point = {1, -1, -1}, .end_point = {1, 1, -1}},
    {.start_point = {1, 1, -1}, .end_point = {-1, 1, -1}},
    {.start_point = {-1, 1, -1}, .end_point = {-1, -1, -1}},
};

line_3d_t x = {.start_point = {0, 0, 0}, .end_point = {0, 0, 0}};
line_3d_t y = {.start_point = {0, 0, 0}, .end_point = {0, 0, 0}};
line_3d_t z = {.start_point = {0, 0, 0}, .end_point = {0, 0, 30}};

line_3d_t rect_source[12];

bool point3Dto2D(point_3d_t *source, point_2d_t *point)
{
    point->x = (source->x * cos_gamma) - (source->y * sin_gamma);
    point->y = -(source->x * sin_gamma * sin_alpha) -
               (source->y * cos_gamma * sin_alpha) + (source->z * cos_alpha);
    return true;
}

bool point2DToDisPoint(point_2d_t *point, uint8_t *x, uint8_t *y)
{
    *x = point->x + 120;
    *y = 67 - point->y;
    return true;
}

bool printLine3D(LGFX_Sprite *display, line_3d_t *line, uint32_t color)
{
    uint8_t start_x, start_y, end_x, end_y;
    point_2d_t point;
    point3Dto2D(&line->start_point, &point);
    point2DToDisPoint(&point, &start_x, &start_y);
    point3Dto2D(&line->end_point, &point);
    point2DToDisPoint(&point, &end_x, &end_y);

    display->drawLine(start_x, start_y, end_x, end_y, (int)color);

    return true;
}

void RotatePoint(point_3d_t *point, double x, double y, double z)
{
    if (x != 0)
    {
        point->y = point->y * cos(x * r_rand) - point->z * sin(x * r_rand);
        point->z = point->y * sin(x * r_rand) + point->z * cos(x * r_rand);
    }

    if (y != 0)
    {
        point->x = point->z * sin(y * r_rand) + point->x * cos(y * r_rand);
        point->z = point->z * cos(y * r_rand) - point->x * sin(y * r_rand);
    }

    if (z != 0)
    {
        point->x = point->x * cos(z * r_rand) - point->y * sin(z * r_rand);
        point->y = point->x * sin(z * r_rand) + point->y * cos(z * r_rand);
    }
}

void RotatePoint(point_3d_t *point, point_3d_t *point_new, double x, double y,
                 double z)
{
    if (x != 0)
    {
        point_new->y = point->y * cos(x * r_rand) - point->z * sin(x * r_rand);
        point_new->z = point->y * sin(x * r_rand) + point->z * cos(x * r_rand);
    }

    if (y != 0)
    {
        point_new->x = point->z * sin(y * r_rand) + point->x * cos(y * r_rand);
        point_new->z = point->z * cos(y * r_rand) - point->x * sin(y * r_rand);
    }

    if (z != 0)
    {
        point_new->x = point->x * cos(z * r_rand) - point->y * sin(z * r_rand);
        point_new->y = point->x * sin(z * r_rand) + point->y * cos(z * r_rand);
    }
}
namespace imuStream
{

    void LCD::Displaybuff()
    {
        Disbuff->pushSprite(0, 0);
    }


    void LCD::lcd_init()
    {
        lcd.begin();
        lcd.setRotation(3);
        /* Full scren sprite */
        Disbuff = new LGFX_Sprite(&lcd);
        Disbuff->createSprite(lcd.width(), lcd.height());

        theta = 0; last_theta = 0;
        phi = 0; last_phi = 0;
        alpha = 0.2;
        for (int n = 0; n < 12; n++)
        {
            rect_source[n].start_point.x = rect[n].start_point.x * 30;
            rect_source[n].start_point.y = rect[n].start_point.y * 30;
            rect_source[n].start_point.z = rect[n].start_point.z * 30;
            rect_source[n].end_point.x = rect[n].end_point.x * 30;
            rect_source[n].end_point.y = rect[n].end_point.y * 30;
            rect_source[n].end_point.z = rect[n].end_point.z * 30;
        }

    }

    void LCD::ColorBar()
    {
        float color_r, color_g, color_b;

        color_r = 0;
        color_g = 0;
        color_b = 255;

        for (int i = 0; i < 384; i = i + 4)
        {
            if (i < 128)
            {
                color_r = i * 2;
                color_g = 0;
                color_b = 255 - (i * 2);
            }
            else if ((i >= 128) && (i < 256))
            {
                color_r = 255 - ((i - 128) * 2);
                color_g = (i - 128) * 2;
                color_b = 0;
            }
            else if ((i >= 256) && (i < 384))
            {
                color_r = 0;
                color_g = 255 - ((i - 256) * 2);
                ;
                color_b = (i - 256) * 2;
                ;
            }
            Disbuff->fillRect(0, 0, 240, 135,
                              Disbuff->color565(color_r, color_g, color_b));
            Displaybuff();
        }

        for (int i = 0; i < 4; i++)
        {
            switch (i)
            {
            case 0:
                color_r = 0;
                color_g = 0;
                color_b = 0;
                break;
            case 1:
                color_r = 255;
                color_g = 0;
                color_b = 0;
                break;
            case 2:
                color_r = 0;
                color_g = 255;
                color_b = 0;
                break;
            case 3:
                color_r = 0;
                color_g = 0;
                color_b = 255;
                break;
            }
            for (int n = 0; n < 240; n++)
            {
                color_r = (color_r < 255) ? color_r + 1.0625 : 255U;
                color_g = (color_g < 255) ? color_g + 1.0625 : 255U;
                color_b = (color_b < 255) ? color_b + 1.0625 : 255U;
                Disbuff->drawLine(n, i * 33.75, n, (i + 1) * 33.75,
                                  Disbuff->color565(color_r, color_g, color_b));
            }
        }
        Displaybuff();
        delay(500);

        for (int i = 0; i < 4; i++)
        {
            switch (i)
            {
            case 0:
                color_r = 255;
                color_g = 255;
                color_b = 255;
                break;
            case 1:
                color_r = 255;
                color_g = 0;
                color_b = 0;
                break;
            case 2:
                color_r = 0;
                color_g = 255;
                color_b = 0;
                break;
            case 3:
                color_r = 0;
                color_g = 0;
                color_b = 255;
                break;
            }
            for (int n = 0; n < 240; n++)
            {
color_r = (color_r > 2) ? color_r - 1.0625 : 0U;
                color_g = (color_g > 2) ? color_g - 1.0625 : 0U;
                color_b = (color_b > 2) ? color_b - 1.0625 : 0U;
                Disbuff->drawLine(239 - n, i * 33.75, 239 - n, (i + 1) * 33.75,
                                  Disbuff->color565(color_r, color_g, color_b));
            }
        }
        Displaybuff();
        delay(500);
    }

    void LCD::frame_test()
    {
        display->fillScreen(TFT_BLACK);
        display->drawRect(0, 0, display->width(), display->height(), TFT_GREEN);
        display->drawRect(1, 1, display->width() - 2, display->height() - 2, TFT_GREEN);
        display->drawLine(0, 0, display->width(), display->height(), TFT_GREEN);
        display->drawLine(0, display->height(), display->width(), 0, TFT_GREEN);
        displayUpdate();

        waitNext();
    }

    void LCD::color_test()
    {
        std::vector<int> color_list = {TFT_RED, TFT_GREEN, TFT_BLUE, TFT_BLACK, TFT_WHITE};
        for (auto i : color_list)
        {
            display->fillScreen(i);
            displayUpdate();
            waitNext();
        }
    }

    void LCD::waitNext()
    {
        while (!checkNext())
        {
            checkReboot();
            delay(10);
        }
        printf("go next\n");
    }

     void LCD::checkPowerOff()
    {
   /* Long press power off */
        if (!btnPWR.read())
        {
            _tone(3500, 50);

            uint32_t time_count = millis();

            while (!btnPWR.read())
            {

                display->setCursor(0, 10);
                display->setFont(&fonts::Font0);
                display->setTextSize(2);
                display->setTextColor(TFT_YELLOW, TFT_BLACK);
                display->printf(" PWR OFF IN %d/3\n", (millis() - time_count) / 1000 + 1);
                displayUpdate();

                delay(10);
            }
        }
    }

     void LCD::checkReboot()
    {
   /* Long press power off */
        if (!btnPWR.read())
        {
            _tone(3500, 50);

            uint32_t time_count = millis();

            while (!btnPWR.read())
            {

                display->setCursor(0, 10);
                display->setFont(&fonts::Font0);
                display->setTextSize(2);
                display->setTextColor(TFT_YELLOW, TFT_BLACK);
                display->printf(" PWR OFF IN %d/3\n", (millis() - time_count) / 1000 + 1);
                displayUpdate();

                delay(10);
            }

            printf("rebooting...\n");

            display->fillScreen(TFT_BLACK);
            display->setCursor(0, 10);
            display->setFont(&fonts::Font0);
            display->setTextSize(2);
            display->setTextColor(TFT_RED, TFT_BLACK);

            display->drawCenterString("REBOOTING...\n", display->width() / 2, display->height() / 2 - 12);
            displayUpdate();

            delay(500);

            esp_restart();
        }
    }

    bool LCD::checkNext()
    {
        // if (btnA.pressed() || btnB.pressed()) {
        //     return true;
        // }
        if (btnA.pressed())
        {
            _tone(5000, 50);
            return true;
        }
        if (btnB.pressed())
        {
            _tone(5500, 50);
            return true;
        }

        return false;
    }

    void LCD::key_init()
    {
        btnA.begin();
        btnB.begin();
        btnPWR.begin();
    }

    void LCD::setup()
    {
        lcd_init();
        key_init();

         ColorBar();
    }

    void LCD::loop()
    {
        // rtc_wakeup_test();

        //wifi_test();

        //lcd_test();
        display->setFont(&fonts::Font0);
        display->setTextSize(1);
        displayUpdate();

    }
    void LCD::draw_accel(float accX, float accY, float accZ)
    {
        line_3d_t rect_dis;

        if ((accX < 1) && (accX > -1))
        {
            theta = asin(-accX) * 57.295;
        }
        if (accZ != 0)
        {
            phi = atan(accY / accZ) * 57.295;
        }

        theta = alpha * theta + (1 - alpha) * last_theta;
        phi = alpha * phi + (1 - alpha) * last_phi;

        // Disbuff->fillRect(0, 0, 240, 135, TFT_BLACK);
        // Disbuff->setTextSize(1);
        // Disbuff->setCursor(10, 115);
        // Disbuff->printf("theta: %.2f", theta);
        // Disbuff->setCursor(10, 125);
        // Disbuff->printf("phi: %.2f", phi);

        display->fillScreen(TFT_BLACK);
        display->setTextColor(TFT_WHITE, TFT_BLACK);

            display->setFont(&fonts::Font0);
            Disbuff->fillRect(0, 0, 240, 135, TFT_BLACK);
            Disbuff->setTextSize(1);
            Disbuff->setCursor(10, 115);
            Disbuff->printf("%.2f", theta);
            Disbuff->setCursor(10, 125);
            Disbuff->printf("%.2f", phi);

        z.end_point.x = 0;
        z.end_point.y = 0;
        z.end_point.z = 60;
        RotatePoint(&z.end_point, theta, phi, 0);
        RotatePoint(&z.end_point, &x.end_point, -90, 0, 0);
        RotatePoint(&z.end_point, &y.end_point, 0, 90, 0);

        for (int n = 0; n < 12; n++)
        {
            RotatePoint(&rect_source[n].start_point, &rect_dis.start_point,
                    theta, phi, (double)0);
            RotatePoint(&rect_source[n].end_point, &rect_dis.end_point, theta,
                    phi, (double)0);
            printLine3D(Disbuff, &rect_dis, TFT_WHITE);
        }
        // Disbuff.fillRect(0,0,160,80,BLACK);
        printLine3D(Disbuff, &x, TFT_RED);
        printLine3D(Disbuff, &y, TFT_GREEN);
        printLine3D(Disbuff, &z, TFT_BLUE);
        /*
           Disbuff.setTextColor(TFT_WHITE);
           Disbuff.setTextSize(1);
           Disbuff.fillRect(0,0,52,18,Disbuff.color565(20,20,20));
           Disbuff.drawString("MPU6886",5,5,1);
           */
        Displaybuff();
        last_theta = theta;
        last_phi = phi;
        
        checkPowerOff();
    }
}
