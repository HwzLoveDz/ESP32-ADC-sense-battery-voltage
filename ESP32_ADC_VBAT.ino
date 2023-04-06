/**
 * @file ESP32_ADC_VBAT.ino
 * @author mondraker (691806052@qq.com)
 * 
 * @brief 通过ESP32的ADC引脚检测电池电压，并通过均值滤波与软件校准
 *        该示例芯片型号：ESP32C3(兼容ESP32其它型号芯片【ESP32S2不支持软件校准】，需要根据idf编程指南ADC部分修改衰减值对应的输入电压范围)
 * 
 *        官方文档：https://docs.espressif.com/projects/esp-idf/en/release-v4.4/esp32c3/api-reference/peripherals/adc.html
 *        中文参考：https://blog.csdn.net/qq_33862616/article/details/107461001
 *        英文参考：https://stackoverflow.com/questions/56833346/how-to-measure-battery-voltage-with-internal-adc-esp32#
 *        一些中英文资料注释来自官方文档
 * 
 * @version 0.1
 * @date 2023-04-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <driver/adc.h>
#include "esp_adc_cal.h"

/*
 *       衰减           可测量的输入电压范围
 *
 *  ADC_ATTEN_DB_0        0 mV ~ 750 mV
 *
 *  ADC_ATTEN_DB_2_5      0 mV ~ 1050 mV
 *
 *  ADC_ATTEN_DB_6        0 mV ~ 1300 mV
 *
 *  ADC_ATTEN_DB_11       0 mV ~ 2500 mV
 */
#define ADC_CHANNEL     ADC1_CHANNEL_2
#define ADC_ATTEN_DB    ADC_ATTEN_DB_2_5
#define ADC_WIDTH_BIT   ADC_WIDTH_BIT_12
#define ADC_NUM         ADC_UNIT_1      // ADC to characterize (ADC_UNIT_1 or ADC_UNIT_2)

static esp_adc_cal_characteristics_t *adcChar;

//! 使用1%分压电阻放大检测范围至[battery_min,battery_max],并且在ADC输入端并联一个0.1uF电容用以防止干扰
//!
//! VBAT--|R1|-->ADC<----|R2|----GND
//!                   ^|0.1uF|^
/* VBAT */
const float battery_min = 3.3; // (V) minimum voltage of battery before shutdown
const float battery_max = 4.2; // (V) maximum voltage of battery
/* R1 & R2 */
const float R1 = 300500.0;     //300k
const float R2 = 100400.0;     //100k
/* adc电压手动校准（adc接地读出来多少这里就写多少） */
const float adc_offset = 0.02;
/* 电池电压手动补偿 */
const float vol_offset = 0.00;

uint32_t adc_read_val()
{

    int samplingFrequency = 500; // 采样频率（可调）
    long sum = 0;             // 采样和
    float samples = 0.0;      // 采样平均值

    for (int i = 0; i < samplingFrequency; i++)
    {
        sum += adc1_get_raw(ADC_CHANNEL); // Take an ADC1 reading from a single channel.
        delayMicroseconds(1000);
    }
    samples = sum / (float)samplingFrequency;

    uint32_t voltage = esp_adc_cal_raw_to_voltage(samples, adcChar); // 2.调用 API 函数将 ADC 值转换为电压（在调用此函数之前，必须初始化特征结构，也就是步骤 1）
    // uint32_t voltage = (samples * 2.6) / 4096.0; //通过公式转换为电压

    return voltage; //单位(mV)
}

void setup()
{

    adc1_config_width(ADC_WIDTH_BIT);                     // Configure ADC1 capture width, meanwhile enable output invert for ADC1. The configuration is for all channels of ADC1.
    adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN_DB); // Configure the ADC2 channel, including setting attenuation.

    /*
     ! 对于 ESP32s2，仅支持两点值校准，仅支持ADC_WIDTH_BIT_13。参数default_vref未使用。
     * 使用 ADC 校准转换测量电压需要两步：
     * 1.获取 ADC-Voltage 特征曲线
     * 2.调用 API 函数将 ADC 值转换位电压
     * 对于带有eFuse ADC校准位的电路板，可以使用esp_adc_cal_raw_to_voltage（）来获得校准的转换结果。
     * 这些结果代表实际电压（以mV为单位）。无需通过公式 （Vout = Dout * Vmax / Dmax） 转换这些数据。
     * 如果在没有电子保险丝 ADC 校准位的电路板上使用 ADC 校准 API，则会生成警告。
     * esp_err_t esp_adc_cal_check_efuse（esp_adc_cal_value_t value_type)
     * 检查 ADC 校准值是否已刻录到电子保险丝中。
     */
    adcChar = (esp_adc_cal_characteristics_t *)calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t cal_mode = esp_adc_cal_characterize(ADC_NUM, ADC_ATTEN_DB, ADC_WIDTH_BIT, ESP_ADC_CAL_VAL_DEFAULT_VREF, adcChar); // 1.获取 ADC-Voltage 特征曲线

    Serial.begin(9600);
}

void loop()
{
    /*adc端电压*/
    float adc_voltage = (float)adc_read_val() / 1000.0f + adc_offset;
    Serial.print("ADC voltage: ");
    Serial.print(adc_voltage);
    Serial.println("V");

    /*通过R1&R2推算电池电压*/
    float battery_voltage = (R1 + R2) / R2 * adc_voltage + vol_offset;
    Serial.print("Battery voltage: ");
    Serial.print(battery_voltage);
    Serial.println("V");

    /*通过电池最低与最高电压推算电池电量*/
    float battery_level = ((battery_voltage - battery_min) / (battery_max - battery_min)) * 100;
    Serial.print("Battery Level: ");
    if (battery_level < 100)    Serial.print(battery_level);
    else                        Serial.print(100.0f); 
    Serial.println("%");

    Serial.print("\n");
    vTaskDelay(500);
}
