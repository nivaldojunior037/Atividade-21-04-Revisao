// Definição das biliotecas a serem utilizadas na execução
#include <stdio.h> 
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h" 
#include "hardware/uart.h"
#include "hardware/pio.h"
#include "AtvRevisao.pio.h" 

#define I2C_PORT i2c1
#define I2C_SDA 14          //Definição da porta GPIO da comunicação I2C (DADOS)
#define I2C_SCL 15          //Definição da porta GPIO da comunicação I2C (CLOCK)
#define endereco 0x3c       //Definição do endereço do display ssd1306
#define NUM_PIXELS 25       //Definição do número de LEDs da matriz 5X5
#define LEDS_PIN 7          //Definição da porta GPIO da matriz de LEDs 5X5
#define LED_PIN_RED 13      //Definição da porta GPIO do led vermelho do conjunto RGB
#define LED_PIN_GREEN 11    //Definição da porta GPIO do led verde do conjunto RGB
#define BUTTON_A 5          //Definição da porta GPIO do botão A
#define BUTTON_B 6          //Definição da porta GPIO do botão B
#define BUZZER 21           //Definição da porta GPIO do Buzzer A
#define VRX_PIN 27          //Definição da porta GPIO do potenciômetro do eixo X do Joystick
#define VRY_PIN 26          //Definição da porta GPIO do potenciômetro do eixo Y do Joystick
#define UART_ID uart0       //Seleciona a UART0
#define BAUD_RATE 115200    //Define a taxa de transmissão
#define UART_TX_PIN 0       //Pino GPIO usado para TX
#define UART_RX_PIN 1       //Pino GPIO usado para RX

// Definição de todos os contadores, flags, variáveis e estruturas que serão utilizadas de forma global
static volatile double r = 0.0, b = 0.0, g = 0.0;
static volatile uint32_t valor_led;
static volatile PIO pio = pio0;
static volatile uint offset;
static volatile uint sm;
static volatile uint32_t ultimo_tempo = 0;
static volatile uint32_t contagem = 0;
static ssd1306_t ssd; 
static volatile bool cor = true;
static volatile int temp = 11;

// Vetores usados para criar os desenhos na matriz de LEDs
double numero0[NUM_PIXELS] = {0.0, 0.5, 0.5, 0.5, 0.0,
                            0.0, 0.5, 0.0, 0.5, 0.0,
                            0.0, 0.5, 0.0, 0.5, 0.0,
                            0.0, 0.5, 0.0, 0.5, 0.0,
                            0.0, 0.5, 0.5, 0.5, 0.0}; 

double numero1[NUM_PIXELS] = {0.0, 0.0, 0.5, 0.0, 0.0,
                            0.0, 0.0, 0.5, 0.0, 0.0,
                            0.0, 0.0, 0.5, 0.0, 0.0,
                            0.0, 0.0, 0.5, 0.0, 0.0,
                            0.0, 0.0, 0.5, 0.0, 0.0}; 

double numero2[NUM_PIXELS] = {0.0, 0.5, 0.5, 0.5, 0.0,
                            0.0, 0.5, 0.0, 0.0, 0.0,
                            0.0, 0.5, 0.5, 0.5, 0.0,
                            0.0, 0.0, 0.0, 0.5, 0.0,
                            0.0, 0.5, 0.5, 0.5, 0.0}; 

double numero3[NUM_PIXELS] = {0.0, 0.5, 0.5, 0.5, 0.0,
                            0.0, 0.5, 0.0, 0.0, 0.0,
                            0.0, 0.5, 0.5, 0.5, 0.0,
                            0.0, 0.5, 0.0, 0.0, 0.0,
                            0.0, 0.5, 0.5, 0.5, 0.0}; 

double numero4[NUM_PIXELS] = {0.0, 0.5, 0.0, 0.5, 0.0,
                            0.0, 0.5, 0.0, 0.5, 0.0,
                            0.0, 0.5, 0.5, 0.5, 0.0,
                            0.0, 0.5, 0.0, 0.0, 0.0,
                            0.0, 0.0, 0.0, 0.5, 0.0}; 

double numero5[NUM_PIXELS] = {0.0, 0.5, 0.5, 0.5, 0.0,
                            0.0, 0.0, 0.0, 0.5, 0.0,
                            0.0, 0.5, 0.5, 0.5, 0.0,
                            0.0, 0.5, 0.0, 0.0, 0.0,
                            0.0, 0.5, 0.5, 0.5, 0.0}; 

double numero6[NUM_PIXELS] = {0.0, 0.5, 0.5, 0.5, 0.0,
                            0.0, 0.0, 0.0, 0.5, 0.0,
                            0.0, 0.5, 0.5, 0.5, 0.0,
                            0.0, 0.5, 0.0, 0.5, 0.0,
                            0.0, 0.5, 0.5, 0.5, 0.0}; 

double numero7[NUM_PIXELS] = {0.0, 0.5, 0.5, 0.5, 0.0,
                            0.0, 0.5, 0.0, 0.0, 0.0,
                            0.0, 0.0, 0.0, 0.5, 0.0,
                            0.0, 0.5, 0.0, 0.0, 0.0,
                            0.0, 0.0, 0.0, 0.5, 0.0}; 

double numero8[NUM_PIXELS] = {0.0, 0.5, 0.5, 0.5, 0.0,
                            0.0, 0.5, 0.0, 0.5, 0.0,
                            0.0, 0.5, 0.5, 0.5, 0.0,
                            0.0, 0.5, 0.0, 0.5, 0.0,
                            0.0, 0.5, 0.5, 0.5, 0.0}; 

double numero9[NUM_PIXELS] = {0.0, 0.5, 0.5, 0.5, 0.0,
                            0.0, 0.5, 0.0, 0.5, 0.0,
                            0.0, 0.5, 0.5, 0.5, 0.0,
                            0.0, 0.5, 0.0, 0.0, 0.0,
                            0.0, 0.0, 0.0, 0.5, 0.0};

// Função para converter os parâmetros r, g, b em um valor de 32 bits
uint32_t matrix_rgb(double b, double r, double g)
{
unsigned char R, G, B;
R = r * 255;
G = g * 255;
B = b * 255;
return (G << 24) | (R << 16) | (B << 8);
}

// Função para formar os desenhos na matriz de LEDs 5x5
void desenhos(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{
// O loop aciona cada LED da matriz com base em um valor de cor 
for (int i = 0; i < NUM_PIXELS; i++){
// Determinação da cor de cada LED
uint32_t valor_led = matrix_rgb(desenho[24 - i]*r, desenho[24 - i]*g, desenho[24 - i]*b);
// O valor é enviado ao LED para ser exibido
pio_sm_put_blocking(pio, sm, valor_led);
}
}

// Função que administrará a interrupção
static void gpio_irq_handler(uint gpio, uint32_t events);

// Função que determina qual número será desenhado na matriz de LEDs quando uma tecla numérica for digitada
static void colorirmatriz(int temp){
b = 0.5;
r = 0.5;
g = 0.5;
switch (temp){
case 0:
// A função de desenho é chamada e os parâmetros são passados a ela
desenhos(numero0, valor_led, pio, sm, b, r ,g);
// O caso se encerra e o número fica exposto até uma nova interrupção ser detectada
break;

case 1:
desenhos(numero1, valor_led, pio, sm, b, r ,g);
break;

case 2:
desenhos(numero2, valor_led, pio, sm, b, r ,g);
break;

case 3:
desenhos(numero3, valor_led, pio, sm, b, r ,g);
break;

case 4:
desenhos(numero4, valor_led, pio, sm, b, r ,g);
break;

case 5:
desenhos(numero5, valor_led, pio, sm, b, r ,g);
break;

case 6:
desenhos(numero6, valor_led, pio, sm, b, r ,g);
break;

case 7:
desenhos(numero7, valor_led, pio, sm, b, r ,g);
break;

case 8:
desenhos(numero8, valor_led, pio, sm, b, r ,g);
break;

case 9:
desenhos(numero9, valor_led, pio, sm, b, r ,g);
break;

default:
b = 0.0; 
r = 0.0;
g = 0.0;
desenhos(numero0, valor_led, pio, sm, b, r ,g);

}
}

// Função que administrará a interrupção
static void gpio_irq_handler(uint gpio, uint32_t events);

// Função que acionará o buzzer com uma dada frequência e duração
void tocar_buzzer(int frequencia, int duracao)
{
    for (int i = 0; i < duracao * 1000; i += (1000000 / frequencia) / 2)
    {
        gpio_put(BUZZER, 1);
        sleep_us((1000000 / frequencia) / 2);
        gpio_put(BUZZER, 0);
        sleep_us((1000000 / frequencia) / 2);
    }
}

// Função que inicializará todas as entradas, saídas e interfaces necessárias à execução do código
void setup(){
    // Inicialização dos LEDs verde e vermelho do conjunto RGB e do buzzer A e a definição de todos como saída
    gpio_init(LED_PIN_RED);
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);
    gpio_put(LED_PIN_RED, 0);
    gpio_init(LED_PIN_GREEN);
    gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);
    gpio_put(LED_PIN_GREEN, 0);
    gpio_init(BUZZER);
    gpio_set_dir(BUZZER, GPIO_OUT);
    gpio_put(BUZZER, 0);

    // Inicialização do botão A e do botão B, definição como entrada e acionamento do pull-up interno
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    // Inicialização e configuração da I2C
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); 
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); 
    gpio_pull_up(I2C_SDA); 
    gpio_pull_up(I2C_SCL);

    // Inicialização e configuração do display ssd1306
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); 
    ssd1306_config(&ssd); 
    ssd1306_send_data(&ssd); 
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    // Inicialização do ADC e configuração para os GPIOs 26 e 27
    adc_init();
    adc_gpio_init(VRX_PIN);
    adc_gpio_init(VRY_PIN);

    // Determinação de variáveis usadas pela matriz de LEDs
    offset = pio_add_program(pio, &AtvRevisao_program);
    sm = pio_claim_unused_sm(pio, true);
    AtvRevisao_program_init(pio, sm, offset, LEDS_PIN);

    // Inicialização e configuração da UART
    uart_init(UART_ID, 115200);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART); 
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART); 
    gpio_pull_up(UART_RX_PIN);

}

// Função principal
int main(){

    // Inicialização da biblioteca stdio.h
    stdio_init_all();

    // Inicialização dos periféricos necessários
    setup();

    // Interrupção com callback para cada um dos botões 
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
   

    // Loop infinito
    while (true){
        // Quando temp é 10, a contagem regressiva é iniciada
        if(temp==10){
            printf("\n\nContagem regressiva iniciada.\n");
        } 

        // A matriz de LEDs é acionada de acordo com o valor de temp
        colorirmatriz(temp);
        uint32_t tempo_cont = to_ms_since_boot(get_absolute_time());
        // A cada 1 segundo, o valor de temp é decrementado em 1 
        if(tempo_cont - contagem >= 1000 && temp<11){
            if(temp<=10 && temp>0){
            printf("Contagem em andamento: 00:0%d\n", temp-1);
            }
            temp--;
            contagem = tempo_cont;
        }
        // Quando temp chega a -1, a contagem para, temp é redefinido para 11 e o alarme do buzzer é acionado
        if(temp == -1){
            tocar_buzzer(1000, 500);
            printf("Contagem finalizada.\n");
            temp = 11;
        }
        // Se o valor de temp estiver entre 0 e 10, o LED verde é acionado
        if(temp<11 && temp>=0){
            gpio_put(LED_PIN_GREEN, 1);
            gpio_put(LED_PIN_RED, 0);
        } 
        // Se o valor de temp for diferente, o LED vermelho fica acionado
        else {
            gpio_put(LED_PIN_GREEN, 0);
            gpio_put(LED_PIN_RED, 1);
        }
        // Leitura do ADC para a entrada 1 e 0 (GPIOs 27 e 26)
        adc_select_input(1);
        uint16_t vrx_value = adc_read();
        adc_select_input(0);
        uint16_t vry_value = adc_read();
        
        // O display é atualizado de acordo com a posição do quadrado 8x8
        ssd1306_fill(&ssd, !cor); 
        ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor);
        // Os valores do ADC para cada coordenada são convertidos para um valor que pode ser passado para o display 
        // O fator de cada conversão foi calculado manualmente, a depender dos retângulos que se desejou desenhar
        int coord_x = ceil(vrx_value/38.0);
        int coord_y = ceil(vry_value/88.0);
        // Os loops percorrem o display, colorindo os pixels determinados de acordo com o valor lido do ADC
        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
                // O fator 5 foi usado para limitar o quadrado ao espaço interior das bordas dos retângulos
                // O fator 48 foi usado para corrigir a direção de movimento no eixo Y
                ssd1306_pixel(&ssd, coord_x+5+j, 48-coord_y+5+i, cor);
            }
        }
        ssd1306_send_data(&ssd);
        // Pequeno delay entre duas iterações consecutivas
        sleep_ms(10);
    }
    return 0;
}


// Função de interrupção com debouncing
static void gpio_irq_handler(uint gpio, uint32_t events){
    // Obtenção do tempo em que ocorre a interrupção desde a inicialização
    uint32_t tempo_atual = to_us_since_boot(get_absolute_time());
    // Verificação de alteração em um intervalo maior que 200ms (debouncing)
    if(tempo_atual - ultimo_tempo > 200000){
        // Se o botão A foi pressionado, o valor de temp é definido em 10
        if(gpio_get(BUTTON_A)==0 && gpio_get(BUTTON_B)==1){
            temp = 10;
        } 
        // Se o botão B foi pressionado, o valor de temp é definido em 11 para interromper a contagem regressiva 
        else if (gpio_get(BUTTON_A)==1 && gpio_get(BUTTON_B)==0){
            temp=11;
            printf("Contagem interrompida.\n");
        }
        ultimo_tempo = tempo_atual; 
    }
}
