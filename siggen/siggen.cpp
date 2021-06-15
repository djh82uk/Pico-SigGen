#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/irq.h"
#include <logo.hpp>
#include <GFX.hpp>
#include "pico/multicore.h"
#include <string> 
using namespace std;


int freq = 1000;
float freqhz;
string mode = "auto";

// Debounce control
unsigned long time = to_ms_since_boot(get_absolute_time());
const int delayTime = 100; // 50ms worked fine for me .... change it to your needs/specs.


static int core0_rx_val = 0, core1_rx_val = 0;



void core0_sio_irq() {
    // Just record the latest entry
    while (multicore_fifo_rvalid())
        core0_rx_val = multicore_fifo_pop_blocking();

    multicore_fifo_clear_irq();
}

void core1_sio_irq() {
    // Just record the latest entry
    while (multicore_fifo_rvalid())
        core1_rx_val = multicore_fifo_pop_blocking();

    multicore_fifo_clear_irq();
}




void freq_change_irq_handler(uint gpio, uint32_t events) {
        if ((to_ms_since_boot(get_absolute_time())-time)>delayTime) {
            time = to_ms_since_boot(get_absolute_time());           
            
            if (gpio == 14 && (freq > 1)) {
                printf("Frequency Increase\n");
                while (gpio_get(gpio) == 1) {
                    if (freq > 100) {
                    printf("Sustained Up\n");
                    freq = freq - 10;
                    sleep_ms(100);
                    } else if (freq <= 100 && ( freq > 1)) {
                    printf("Throttled Up\n");
                    freq = freq - 1;
                    sleep_ms(100);

                    } else {
                    printf("Upper Limit Reached\n");
                    }
                
                }
        }
            else if (gpio == 15 && (freq <5000)) {
                printf("Frequency Decrease\n");
                while (gpio_get(gpio) == 1) {
                    if (freq < 5000) {
                    printf("Sustained Down\n");
                    freq = freq + 10;
                    sleep_ms(100);
                    } else {
                    printf("Lower Limit Reached\n");
                    }
                
                }
                
                
                } else if (gpio == 12) {
                    printf("Mode Toggle\n");
                    if (mode == "auto") {
                        mode = "manual";
                    }
                    else if (mode == "manual") {
                        mode = "auto";
                    }
                    }
                    
                else if (gpio == 11 && (mode == "manual")) {
                    printf("Manual Pulse\n");
                    gpio_put(13, 1);
                    sleep_ms(500);
                    gpio_put(13, 0);
                    sleep_ms(500);
                    }
            
            
        
    

        printf("Interupt on Pin: %i\n", gpio);

}    
}



// Core 1 Main Code
void core1_entry() {
    printf("Starting Core 1\n");
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    multicore_fifo_clear_irq();
    irq_set_exclusive_handler(SIO_IRQ_PROC1, core1_sio_irq);

    irq_set_enabled(SIO_IRQ_PROC1, true);
    gpio_init(13);
    gpio_set_dir(13, GPIO_OUT);
    gpio_init(11);
    gpio_pull_down(11);
    gpio_init(12);
    gpio_pull_down(12);
    gpio_init(14);
    gpio_pull_down(14);
    gpio_init(15);
    gpio_pull_down(15);
    gpio_set_irq_enabled_with_callback(14, GPIO_IRQ_EDGE_RISE  , true, &freq_change_irq_handler);
    gpio_set_irq_enabled_with_callback(15, GPIO_IRQ_EDGE_RISE  , true, &freq_change_irq_handler);
    gpio_set_irq_enabled_with_callback(12, GPIO_IRQ_EDGE_RISE  , true, &freq_change_irq_handler);
    gpio_set_irq_enabled_with_callback(11, GPIO_IRQ_EDGE_RISE  , true, &freq_change_irq_handler);
    


    // Send something to Core0, this should fire the interrupt.
    

    while(true) {
        if (mode == "auto") {
            gpio_put(13, 1);
            sleep_ms(freq / 2);
            gpio_put(13, 0);
            sleep_ms(freq / 2);
            multicore_fifo_push_blocking(freq);
    } 
    } 
    

}

int main() {
    stdio_init_all();
    
 
    multicore_launch_core1(core1_entry);
    irq_set_exclusive_handler(SIO_IRQ_PROC0, core0_sio_irq);
    irq_set_enabled(SIO_IRQ_PROC0, true);
    //setup
    //stdio_init_all();
    i2c_init(i2c1, 400000);                 //Initialize I2C on i2c1 port with 400kHz
    gpio_set_function(2, GPIO_FUNC_I2C);    //Use GPIO2 as I2C
    gpio_set_function(3, GPIO_FUNC_I2C);    //Use GPIO3 as I2C
    gpio_pull_up(2);                        //Pull up GPIO2
    gpio_pull_up(3);                        //Pull up GPIO3

    GFX oled(0x3C, size::W128xH64, i2c1);   //Declare oled instance
    // if you are using 128x32 oled try size::W128xH32

    oled.display(logo);                     //Display bitmap
    
    while(true) 
    {
        sleep_ms(1000);
        oled.clear();                       //Clear buffer
        freqhz = 1.0 / freq;
        freqhz = freqhz * 1000;
        string ssfreq = to_string(freq);
        string ssfreqhz = to_string(freqhz);
        ssfreqhz.erase(ssfreqhz.size() - 4);
        oled.drawString(0, 0,  "mode: " + mode);
        oled.drawString(0, 10,  "Period: " + ssfreq);
        oled.drawString(0, 20,  "Frequency: " + ssfreqhz + "Hz");
        
       
        

        oled.display();                     //Send buffer to the screen
        //printf("Irq handlers should have rx'd some stuff - core 0 got %d, core 1 got %d!\n", core0_rx_val, core1_rx_val);
    }
    return 0;
}




