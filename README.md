# ESP32-ADC-sense-battery-voltage
The battery voltage is sensed using the ADC pin of the ESP32 and calibrated by averaging filtering and software

 *        通过ESP32的ADC引脚检测电池电压(并通过均值滤波与软件校准)
          该示例芯片型号：ESP32C3(兼容ESP32其它型号芯片【ESP32S2不支持软件校准】，需要根据idf编程指南ADC部分修改衰减值对应的输入电压范围)
 *        官方文档：https://docs.espressif.com/projects/esp-idf/en/release-v4.4/esp32c3/api-reference/peripherals/adc.html
 *        中文参考：https://blog.csdn.net/qq_33862616/article/details/107461001
 *        英文参考：https://stackoverflow.com/questions/56833346/how-to-measure-battery-voltage-with-internal-adc-esp32#
