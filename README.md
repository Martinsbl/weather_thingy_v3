# Weather Thingy
This is a project I use to measure temperature, humidity, and pressure in and around my apartment. It uses a BME280 sensor to measure the environmental data and the ADC to measure the battery status of the device. The data is put in an advertising packet and advertised at a slow interval. 

Buttonless [DFU](http://infocenter.nordicsemi.com/topic/com.nordic.infocenter.sdk5.v14.2.0/ble_sdk_app_buttonless_dfu.html?cp=4_0_1_4_2_2_6) is also implemented so that I can update the FW over the air. 

I have also used this project as a platform to learn how to use CLion and to improve my scripting skills. 

![Picture](https://lh3.googleusercontent.com/xH6-bNtWXYlkLxjI9prHgfwVmA8SUuYQaIUFgpT_b5wWoZiEfkm3NsSmccnS1lBW07k4Js2Zb2wdKWIqQx3GMje4B-aLNnctAZ111hisuiwNY076TZISHeV-5gCmAFlnBUacalBRHpFhgr9J8o2wFPI8-eMqWgVYrlaY2Qx7ONURrxQ-I-TMEQT6bkIx7ptE99fHFEEDL0RWP2nOBEVmGT04F255hbTSCdsaRb--wIwxGCaQLjsKvfPi8FvsHrWVKSlAn2e_UpAFh0bg0xu5d946KErT99ImZcqHuxrPVg8IcqwY4jllQCLcTlho07foMkCr0E61XQZFJTCbO3gb99inl6akHsamp5GeVSnBjz-V8j70x6E28z32Z1G-hSfqOS1mZ8xq8nXsdJbargQ7GVZgm_7wpyQ7v30xqkR_1BWIMLaUpxp9-p5H5Z1uR9zN_B3ATG8hfcTt8EVlh7OogT4zPvj-afXkX49mGlWA0lD91PraLq76Cd2dHe9O7UNAdX0GkTVn1XOiknCdYTxVTRU9sUsKcOCMHDqPmeG-iRqL2KMgPdXXcr7B5m71XXZ6jLJKKaXTVAICwQJdOhEbjN8mIdu3XQF7e7NHVus=w557-h989-no)
