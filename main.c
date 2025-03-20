#define F_CPU   16000000

#include <util/delay.h>
#include <avr/io.h>

#define K1_OFF PORTB &= ~(1<<5)
#define K2_OFF PORTB &= ~(1<<4)
#define K3_OFF PORTB &= ~(1<<3)

#define K1_ON PORTB |= (1<<5)
#define K2_ON PORTB |= (1<<4)
#define K3_ON PORTB |= (1<<3)

int main(void)
{  
    // Configuração das entradas e saídas
    DDRB &= ~((1<<2)|(1<<1));  // Configura PB2 e PB1 como entrada
    PORTB |= ((1<<2)|(1<<1));  // Ativa pull-ups internos 

    DDRB |= ((1<<3)|(1<<4)|(1<<5)); // Configura PB3, PB4 e PB5 como saída
    K1_OFF; 
    K2_OFF; 
    K3_OFF; 

    int estado = 0; 

    while (1) 
    {
        // Se pressionar S1 (PB2), inicia sequência estrela-triângulo
        if (!(PINB & (1<<2)))  
        {
            estado = 1; 
        }

        // Se pressionar S0 (PB1), desliga tudo
        if (!(PINB & (1<<1)))  
        {
            estado = 2; 
        }

        switch (estado)
        {
            case 1:
                K1_ON;  // Liga K1 (estrela)
                K2_ON;  // Liga K2 
                _delay_ms(5000); // Aguarda tempo da estrela
				K2_OFF; // Desliga K2 (transição)
				_delay_ms(1); // Pequeno intervalo de segurança
 				break;  
			case 2:
				K3_ON;  // Liga K3 (triângulo)
				estado = 0; // Retorna ao estado inicial
				break;
            case 3:
                // Desliga todos os relés
                PORTB &= ~((1<<3)|(1<<4)|(1<<5));
                estado = 0; // Retorna ao estado inicial
                break;
        }
    }
}
