#include <stdint.h>
#include <stdbool.h>

#define SYSCTL_RCGCGPIO_R   *((volatile unsigned long *)0x400FE608)    //REGISTRO DEL RELOJ
#define clk_enable          *((volatile unsigned long *)0x1100)    //REGISTRO DEL RELOJ

#define GPIO_PORTN_DIR_N    *((volatile unsigned long *)0x40064400) //Registro de Dirección N
#define GPIO_PORTN_DEN_N    *((volatile unsigned long *)0x4006451C) //Registro de habilitación N
#define GPIO_PORTN_DATA_N   *((volatile unsigned long *)0x4006400C) //Registro de Datos N

#define GPIO_PORTN_DIR_J    *((volatile unsigned long *)0x40060400) //Registro de Dirección J
#define GPIO_PORTN_PUR_J    *((volatile unsigned long *)0x40060510) //Registro de pull-up J
#define GPIO_PORTN_DEN_J    *((volatile unsigned long *)0x4006051C) //Registro de habilitación J
#define GPIO_PORTN_DATA_J   *((volatile unsigned long *)0x4006000C) //Registro de Datos J

main(){
    int bot;
    SYSCTL_RCGCGPIO_R|=clk_enable;
    while(SYSCTL_RCGCGPIO_R && 0x1100==0){};

    GPIO_PORTN_DEN_N|=0xFF;
    GPIO_PORTN_DIR_N|=0xFF;

    GPIO_PORTN_DIR_J|=0x00;
    GPIO_PORTN_PUR_J|=0x03;
    GPIO_PORTN_DEN_J|=0xFF;

    while(1){
        //if (GPIO_PORTN_DATA_J!=3){
        bot=GPIO_PORTN_DATA_J;
        if(bot==0)
            bot=3;
        else if (bot==1)
            bot=2;
        else if (bot==2)
            bot=1;
        else
            bot=0;
        GPIO_PORTN_DATA_N=bot;
     }
}
