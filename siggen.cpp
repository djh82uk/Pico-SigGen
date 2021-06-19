#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/irq.h"
#include "logo.hpp"
#include "GFX.hpp"
#include "pico/multicore.h"
#include <string> 
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "clk.pio.h"
using namespace std;


//Declare Variables
string mode = "auto";
double freq = 1.00;
uint32_t base = 62500000;
uint sm;
PIO pio = pio0;
uint32_t freqcalc = base / freq; // 62500000 / freq in hz
float btn_time;

// Debounce control
unsigned long time = to_ms_since_boot(get_absolute_time());
const int delayTime = 200; // 50ms worked fine for me .... change it to your needs/specs.


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

void clk_gen(PIO pio, uint sm, uint offset, uint pin, uint freq);



// Interupt Function
void freq_change_irq_handler(uint gpio, uint32_t events) {
        if ((to_ms_since_boot(get_absolute_time())-time)>delayTime) {
            time = to_ms_since_boot(get_absolute_time());           
            
            if (gpio == 15 && (freq < 50000.0)) { //Max Frequency is 50KHz
                printf("Frequency Increase\n");  
                while (gpio_get(gpio) == 1 && (freq <= 50000.0)) {  
                    if (freq <= 50000.0) {
                    printf("Time = %i\n" ,((to_ms_since_boot(get_absolute_time())-time)));
                    btn_time = ((to_ms_since_boot(get_absolute_time())-time));
                    if (btn_time > 1000) {
                        btn_time = 1000;
                    } 
                    else if (btn_time == 0) {
                        freq = freq + 1;
                    }
                    sleep_ms(500);
                    freq = freq + btn_time; //Scale the increment by how long the button has bee held
                    if (freq > 50000.0) {
                        freq = 50000;
                    }
                    sleep_ms(100);
                    freqcalc = base / freq; // 62500000 / freq in hz
                    } 
                
                }
        }
    
            else if (gpio == 14 && (freq > 0.2)) {
                printf("Frequency Decrease\n");
                while (gpio_get(gpio) == 1 && (freq > 0.2)) {
                    if (freq >= 0.2) {
                    printf("Time = %i\n" ,((to_ms_since_boot(get_absolute_time())-time)));
                    btn_time = ((to_ms_since_boot(get_absolute_time())-time));
                    if (btn_time > 1000) {
                    btn_time = 1000;
                    } 
                    else if (btn_time == 0) {
                        freq = freq - 1;  //Scale the Deccrement by how long the button has bee held
                    }
                    sleep_ms(500);
                    freq = freq - btn_time;
                    if (freq < 0.2) {
                        freq = 0.2;
                    }
                    sleep_ms(100);
                    freqcalc = base / freq; // 62500000 / freq in hz
                
                    } 

                }           
        
                
                } else if (gpio == 12) { // Detect if Mode Button is Pressed
                    printf("Mode Toggle\n");
                    if (mode == "auto") {
                        mode = "manual";
                        pio_sm_set_enabled(pio, sm, false);  //Disable the PIO for Manual Mode
                    }
                    else if (mode == "manual") {
                        mode = "auto";
                        pio_sm_set_enabled(pio, sm, true);  //Enable the PIO for Auto Mode
                    }
                    }
                    
                else if (gpio == 11 && (mode == "manual")) {
                    printf("Manual Pulse\n");
                    //Need to fill in and figure out how to pulse the CLK when it is handled by the PIO
                    sleep_ms(500);
                }
                    }          
            
        
    
    
    // Clear the Buffer and Re-initialise
    pio_sm_clear_fifos(pio, sm);
    pio->txf[sm] = freqcalc;


}

// Core 1 Main Code
void core1_entry() {
    printf("Starting Core 1\n");

    
    multicore_fifo_clear_irq();
    irq_set_exclusive_handler(SIO_IRQ_PROC1, core1_sio_irq);
    irq_set_enabled(SIO_IRQ_PROC1, true);
    
    //Setup Interupt Pins
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
    
    while(true) 
    {
        // Keep the Buffer Filled
        sleep_us(10);
        freqcalc = base / freq; // 62500000 / freq in hz
        pio->txf[sm] = freqcalc; 
    }


}





int main() {
    stdio_init_all();
    sleep_ms(1000);
    uint offset = pio_add_program(pio, &clk_program);
    printf("Loaded program at %d\n", offset);
    clk_gen(pio, 0, offset, 13, freq);

    irq_set_exclusive_handler(SIO_IRQ_PROC0, core0_sio_irq);
    irq_set_enabled(SIO_IRQ_PROC0, true);
    //setup
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
        string ssfreq = to_string(freq);
        ssfreq.erase(ssfreq.size() - 4);
        oled.drawString(0, 0,  "mode: " + mode);
        if (freq < 1000.0) {
            string ssfreq = to_string(freq);
            ssfreq.erase(ssfreq.size() - 4);
            oled.drawString(0, 10,  "Frequency: " + ssfreq + "Hz");
        } else if (freq < 1000000.0) {
            string ssfreq = to_string(freq / 1000);
            ssfreq.erase(ssfreq.size() - 4);
            oled.drawString(0, 10,  "Frequency: " + ssfreq + "KHz");
        } else {
            string ssfreq = to_string(freq / 1000000);
            ssfreq.erase(ssfreq.size() - 4);
            oled.drawString(0, 10,  "Frequency: " + ssfreq + "MHz");
        } 
     

        oled.display();                     //Send buffer to the screen
        //printf("Irq handlers should have rx'd some stuff - core 0 got %d, core 1 got %d!\n", core0_rx_val, core1_rx_val);
    }
    return 0;
}


void clk_gen(PIO pio, uint sm, uint offset, uint pin, uint freq) {
    clk_program_init(pio, sm, offset, pin);
    pio_sm_set_enabled(pio, sm, true);
    multicore_launch_core1(core1_entry);
}


