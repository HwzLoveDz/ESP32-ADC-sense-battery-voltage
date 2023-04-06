# ESP32-ADC-sense-battery-voltage
##         通过ESP32的ADC引脚检测电池电压(并通过均值滤波与软件校准)
#####      本示例芯片型号：ESP32C3(兼容ESP32其它型号芯片【ESP32S2不支持软件校准】，但需要根据idf编程指南ADC部分修改衰减值对应的输入电压范围)
#####      可以通过设置分压电阻阻值等等灵活检测不同电池电压

* #####       官方文档：https://docs.espressif.com/projects/esp-idf/en/release-v4.4/esp32c3/api-reference/peripherals/adc.html
* #####       中文参考：https://blog.csdn.net/qq_33862616/article/details/107461001
* #####       英文参考：https://stackoverflow.com/questions/56833346/how-to-measure-battery-voltage-with-internal-adc-esp32#

##        实验图片
#####     电压精度能达到±0.2左右

* #####       串口显示
![串口](https://user-images.githubusercontent.com/60030172/230500531-f96f4d2b-c7cd-4475-9d0a-f4bab2bddecd.png)

* #####       万用表显示
![万用表](https://user-images.githubusercontent.com/60030172/230500579-aea9954b-cf4e-4dd5-b2fc-6e065612132c.jpg)
