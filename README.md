# Sistemas Digitais

## Equipe:  </br> 
* Leandro Pereira Sampaio </br> 
* Gabriel Silva de Azevedo

##Instruções do projeto 

O arquivo base do código em C se encontra no caminho "PBL_SD-master/software/ProjetoPbl2" com o nome de "hello_world_small.c". Para executar o sistema na FPGA, é necessário utilizar o arquivo de compilação do projeto que se encontra dentro de "PBL_SD-master/output_files/PBL-SD-1.sof", o qual está preparado para ser descarregado na FPGA EP4CE6E22C8N, a qual possuí o chip da cyclone IV e as próprias pinagens. Caso seja necessário alterar o projeto para outra placa é possivel abrir o arquivo do projeto que se encontra na pasta "PBL_SD-master" e lá alterar as configurações e pinagens e configurações de placa do projeto. Utilizando o programmer e descarregando o projeto com o NIOS II na placa é possível executar o software em C com o Eclipse índigo presente no próprio Altera Quartus, a pasta do projeto de software em C encontra-se dentro da pasta "PBL_SD-master/software/ProjetoPbl2" sendo que esta mesma pasta está filiada com a pasta "PBL_SD-master/software/ProjetoPbl2_bsp" esta que possui os endereços de memória para as entradas e saídas da placa.
