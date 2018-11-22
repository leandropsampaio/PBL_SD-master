#include "sys/alt_stdio.h"
#include <stdio.h>
#include <system.h>
#include <altera_avalon_pio_regs.h>
#include "altera_avalon_uart_regs.h"
#define LCD_WR_COMMAND_REG 0x0
#define LCD_WR_DATA_REG 0x1
char button_value = 3;
#define UP_BUTTON 1
#define DOWN_BUTTON 2


void write_uart(int tamanho, char *comando)
{
    unsigned long status = 0;
    int i;
    // acenderLeds(1, 0);
    // Verifica até que seja possível a transmissão de dados
    while((status & 0x00000040) != 0x00000040)
    {
        status = IORD_ALTERA_AVALON_UART_STATUS(UART_1_BASE);
    }

    for (i = 0; i < tamanho; i++) {
    	IOWR_ALTERA_AVALON_UART_TXDATA(UART_1_BASE, comando[i]);
    	usleep(1000);
    }
    IOWR_ALTERA_AVALON_UART_TXDATA(UART_1_BASE, '\r');
    usleep(1000);
    IOWR_ALTERA_AVALON_UART_TXDATA(UART_1_BASE, '\n');

}

void write_uart_quick(int tamanho, char comando)
{
	printf("COMANDO: %c", comando);
    unsigned long status = 0;
    int i;
    // acenderLeds(1, 0);
    // Verifica até que seja possível a transmissão de dados
    while((status & 0x00000040) != 0x00000040)
    {
        status = IORD_ALTERA_AVALON_UART_STATUS(UART_1_BASE);
    }

    for (i = 0; i < tamanho; i++) {
    	IOWR_ALTERA_AVALON_UART_TXDATA(UART_1_BASE, comando);
    	usleep(1000);
    }

}

char read_uart()
{
    int x=0;
	char data;
    while(1){
    	if(IORD_ALTERA_AVALON_UART_STATUS( UART_1_BASE ) & 0x80 ) {
    		data = IORD_ALTERA_AVALON_UART_RXDATA( UART_1_BASE );
	        printf("%c",data);

	        if(data == 'K'){
	        	break;
	        }
    	}
	}
}


void acenderLeds(int opcao, int nivelLogico) {
	if (opcao == 1) {
		IOWR_ALTERA_AVALON_PIO_DATA(LED1_BASE, nivelLogico);
	} else if (opcao == 2) {
		IOWR_ALTERA_AVALON_PIO_DATA(LED2_BASE, nivelLogico);
	} else if (opcao == 3) {
		IOWR_ALTERA_AVALON_PIO_DATA(LED3_BASE, nivelLogico);
	} else if (opcao == 4) {
		IOWR_ALTERA_AVALON_PIO_DATA(LED4_BASE, nivelLogico);
	} else if (opcao == 5) {
		IOWR_ALTERA_AVALON_PIO_DATA(LED5_BASE, nivelLogico);
	}
}

/*
 * Função para escrever palavras no LCD
 */
void printalcd(char word[][16], int lugar) {
	lugar = lugar - 1;

	int i;
	/* Set the Cursor to the home position */
	ALT_CI_LCD_0(LCD_WR_COMMAND_REG, 0x02);
	usleep(2000);

	/* Display clear */
	ALT_CI_LCD_0(LCD_WR_COMMAND_REG, 0x01);
	usleep(2000);
	//printf("%i",strlen(word));
	for (i = 0; i < strlen(word); i++) {
		//printf("PALAVRA: %c \n", word[lugar][i]);
		ALT_CI_LCD_0(LCD_WR_DATA_REG, word[lugar][i]);
		usleep(100);
	}
}

void lcd_init(void) {
	usleep(15000); /* Wait for more than 15 ms before init */
	/* Set function code four times -- 8-bit, 2 line, 5x7 mode */
	ALT_CI_LCD_0(LCD_WR_COMMAND_REG, 0x38);
	usleep(4100); /* Wait for more than 4.1 ms */
	ALT_CI_LCD_0(LCD_WR_COMMAND_REG, 0x38);
	usleep(100); /* Wait for more than 100 us */
	ALT_CI_LCD_0(LCD_WR_COMMAND_REG, 0x38);
	usleep(5000); /* Wait for more than 100 us */
	ALT_CI_LCD_0(LCD_WR_COMMAND_REG, 0x38);
	usleep(100); /* Wait for more than 100 us */

	/* Set Display to OFF*/
	ALT_CI_LCD_0(LCD_WR_COMMAND_REG, 0x08);
	usleep(100);

	/* Set Display to ON */
	ALT_CI_LCD_0(LCD_WR_COMMAND_REG, 0x0C);
	usleep(100);

	/* Set Entry Mode -- Cursor increment, display doesn't shift */
	ALT_CI_LCD_0(LCD_WR_COMMAND_REG, 0x06);
	usleep(100);

	/* Set the Cursor to the home position */
	ALT_CI_LCD_0(LCD_WR_COMMAND_REG, 0x02);
	usleep(2000);

	/* Display clear */
	ALT_CI_LCD_0(LCD_WR_COMMAND_REG, 0x01);
	usleep(2000);
}

int main() {
	//printf("Hello from Nioss II!!\n");

	// Alterar o x12 - Tamanho do pacote
	// O ID= 'Nios 2'
	char connect[] = {0x10, 0x11, 0x00, 0x04, 0x4D, 0x51, 0x54, 0x54,
			0x04, 0x02, 0x00, 0x14, 0x00, 0x05, 0x4E, 0x69, 0x6F, 0x73, 0x35};

	// 0x30 -> Código da mensagem para o publish
	// 0x0F -> Tamanho da mensagem (decimal)
	// 0x00, 0x02 -> Tamanho do tópico
	char publish[] = {0x30, 0x18, 0x00, 0x0C,
			0x74, 0x65, 0x73, 0x74, 0x65, 0x2f, 0x74, 0x65, 0x73, 0x74, 0x65, 0x31,
			0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x57, 0x6F, 0x72, 0x6C, 0x64};
	/*char publish[] = {0x30, 0x11, 0x05,
				0x74, 0x65, 0x73, 0x74, 0x65,
				0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x57, 0x6F, 0x72, 0x6C, 0x64};*/
	/*char publish[] = {0x30, 0x0F, 0x00, 0x02,
					0x53, 0x44, 0x48, 0x65, 0x6C,
					0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64};*/
	//char disconnect[] = {0xE0, 0x00};

 	int sw_value, sw_value1, sw_value2, sw_value3;
	int opcao = 1;
	int entrouOpcao = 0, liberado = 1;

	// INICIALIZAÇÃO DO LCD
	lcd_init();
	char bemVindo[1][16] = { " SEJA BEM VINDO" };
	char options[5][16] = { "1 opcao", "2 opcao", "3 opcao", "4 opcao",
			"5 opcao" };
	//char options2[5][16] = {"1 opcao","2 opcao","3 opcao","4 opcao","5 opcao"};
	char options2[5][16] = { "Entrou no 1", "Entrou no 2", "Entrou no 3",
			"Entrou no 4", "Entrou no 5" };
	// Enviando palavra para ser exibida no LCD

	// Desligar todos os LEDS
	IOWR_ALTERA_AVALON_PIO_DATA(LED1_BASE, 1);
	IOWR_ALTERA_AVALON_PIO_DATA(LED2_BASE, 1);
	IOWR_ALTERA_AVALON_PIO_DATA(LED3_BASE, 1);
	IOWR_ALTERA_AVALON_PIO_DATA(LED4_BASE, 1);
	IOWR_ALTERA_AVALON_PIO_DATA(LED5_BASE, 1);

	printalcd(bemVindo, opcao);
	usleep(2000000);
	printalcd(options, opcao);

//_____________________________________________________

	int x, z;
	char comando[] = "AT";
	char resposta;
	//printf("%d",strlen(comando));
	write_uart(strlen(comando), comando);
	read_uart();
	char cmd[] = "AT+CWJAP=\"WLessLEDS\",\"HelloWorldMP31\"";
	//char cmd[] = "AT+CWJAP=\"IEEE_UEFS\",\"ramoIEEEUEFS\"";
	//char cmd[] = "AT+CWJAP=\"Gilberto WiFi\",\"20152015\"";
	write_uart(strlen(cmd), cmd);
	read_uart();
	printf("CONECTOU A REDE!");

	//192.168.1.100 1883
	// Conectando ao cloudmqtt
	char cmd2[] = "AT+CIPSTART=\"TCP\",\"192.168.1.103\",1883";
	//char cmd2[] = "AT+CIPSTART=\"TCP\",\"test.mosquitto.org\",1883";
	write_uart(strlen(cmd2), cmd2);
	read_uart();
	//printf("%c", connect[0]);
	printf("CONEXAO TCP!");
	printf("%d", sizeof(connect));
	/*
	char cmd3[] = "AT+CIPSTATUS";
	write_uart(strlen(cmd3), cmd3);
	read_uart();
	*/
	char cmd3[] = "AT+CIPSEND=19";
	//char cmd4 = strlen(connect);
	//write_uart_quick(strlen(cmd3), cmd3);
	write_uart(strlen(cmd3),cmd3);
	read_uart();
	//printf(strlen(connect));
	//write_uart(strlen(connect),connect);
	for(z=0; z < sizeof(connect); z++){
		printf("%c", connect[z]);
		write_uart_quick(1, connect[z]);
	}
	IOWR_ALTERA_AVALON_UART_TXDATA(UART_1_BASE, '\r');
	usleep(1000);
	IOWR_ALTERA_AVALON_UART_TXDATA(UART_1_BASE, '\n');
	read_uart();

	printf("\nCONEXAO COM O BROKER CONCLUIDA!");

	// 26- teste/teste1
	char cmd4[] = "AT+CIPSEND=26";
	write_uart(strlen(cmd4),cmd4);
	//read_uart();
	printf("\nTAMANHO: %d\n",sizeof(publish));
	//write_uart(strlen(connect),connect);
	printf("---------------\n");
	printf("%c", publish[0]);
	printf("---------------\n");
	printf("%c", publish[1]);
	printf("----------------\n");

	for(z=0; z < sizeof(publish); z++){
		printf("---------------\n");
		printf("%d",z);
		printf(": %c", publish[z]);
		write_uart_quick(1, publish[z]);
	}

	IOWR_ALTERA_AVALON_UART_TXDATA(UART_1_BASE, '\r');
	usleep(1000);
	IOWR_ALTERA_AVALON_UART_TXDATA(UART_1_BASE, '\n');
	read_uart();



	printf("Terminou!");

//_____________________________________________________
	while (1) {
		sw_value = IORD_ALTERA_AVALON_PIO_DATA(PUSHBUTON1_BASE);
		sw_value1 = IORD_ALTERA_AVALON_PIO_DATA(PUSHBUTON2_BASE);
		sw_value2 = IORD_ALTERA_AVALON_PIO_DATA(PUSHBUTON3_BASE);
		sw_value3 = IORD_ALTERA_AVALON_PIO_DATA(PUSHBUTON4_BASE);
		//printf("%x", sw_value3);
		//usleep(150000); /* Wait for more than 15 ms before init */

		if (sw_value == 0 && entrouOpcao == 0 && liberado == 1) { // Aperta o button1
			if (opcao == 1) {
				opcao = 5;
			} else {
				opcao--;
			}
			printalcd(options, opcao);
			liberado = 0;
		} else if (sw_value1 == 0 && entrouOpcao == 0 && liberado == 1) { // Aperta o button2
			if (opcao == 5) {
				opcao = 1;
			} else {
				opcao++;
			}
			printalcd(options, opcao);
			liberado = 0;
		} else if (sw_value2 == 0) { // Aperta o button3
			acenderLeds(opcao, 1);
			entrouOpcao = 0;
			printalcd(options, opcao);
		} else if (sw_value3 == 0 && liberado == 1) { // Aperta o button4
			acenderLeds(opcao, 0);
			printalcd(options2, opcao);
			entrouOpcao = 1;
			liberado = 0;
		}
		//printf("OPCAO: %i \n", opcao);

		// Liberar button
		if (sw_value == 1 && sw_value1 == 1 && sw_value3 == 1) {
			liberado = 1;
		}
	}
	return 0;
}
