# ESP32-ADC-sense-battery-voltage

##         通过ESP32的ADC引脚检测电池电压(并通过均值滤波与软件校准)
-------------------------------
#####      注意，若要使用ADC2请留意[该警告](https://docs.espressif.com/projects/esp-idf/en/release-v4.4/esp32c3/api-reference/peripherals/adc.html#adc-limitations)
-------------------------------
* #####      本示例芯片型号：ESP32C3(兼容ESP32其它型号芯片【ESP32S2不支持软件校准】，但需要根据idf编程指南ADC部分修改衰减值对应的输入电压范围)
* #####      可以通过设置电池最大与最小电压（最小电压需要大于保护板断电电压）换算电池电量百分比，还可以通过设置分压电阻阻值等等灵活检测不同电压（不局限于电池）
* #####      使用前请adc接地读取偏差值，设置```adc_offset```。或使用以下函数将ADC1或ADC2的基准电压输出至IO，然后可以手动测量基准电压以进行校准
```c 
  esp_err_t adc_vref_to_gpio（adc_unit_t adc_unit， gpio_num_t GPIO)
```
* #####      ESP32 ADC 对噪声非常敏感，导致 ADC 读数存在较大差异。根据使用场景，可以将旁路电容（例如100 nF陶瓷电容）连接到正在使用的ADC输入焊盘，以最大限度地降低噪声
* #####      有任何不明白的地方可以提issue或对照[官方文档](https://docs.espressif.com/projects/esp-idf/en/release-v4.4/esp32c3/api-reference/peripherals/adc.html#_CPPv416adc_vref_to_gpio10adc_unit_t10gpio_num_t)进行学习

##         相关文档资料
* #####       官方文档：https://docs.espressif.com/projects/esp-idf/en/release-v4.4/esp32c3/api-reference/peripherals/adc.html
* #####       中文参考：https://blog.csdn.net/qq_33862616/article/details/107461001
* #####       英文参考：https://stackoverflow.com/questions/56833346/how-to-measure-battery-voltage-with-internal-adc-esp32#

##        实验图片
#####     电压精度能达到±0.01V左右
* #####       串口显示
![串口](https://user-images.githubusercontent.com/60030172/230500531-f96f4d2b-c7cd-4475-9d0a-f4bab2bddecd.png)
* #####       万用表显示
![万用表](https://user-images.githubusercontent.com/60030172/230500579-aea9954b-cf4e-4dd5-b2fc-6e065612132c.jpg)
