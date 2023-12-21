# WebCar
> Repositório criado para a documentação do projeto WebCar, desenvolvido durante a matéria de Computação Física e Aplicações, ministrada pelo Professor Doutor Fábio Nakano na Universidade de São Paulo. 

## 1. Introdução
O WebCar é um projeto de um carrinho que pode ser controlado remotamente por meio de uma rede WiFi, sendo possível controlar a sua velocidade, direção, além de poder posicionar uma câmera (acoplada) de acordo com a vontade do usuário.

## 2. Funcionalidades
O WebCar é controlado por meio de uma página web. Com isso, o usuário pode ter acesso as seguintes funcionalidades:
- Controle velocidade do carrinho;
- Alteração da direção do carrinho (frente, ré, esquerda e direita);
- Alteração do ângulo de visualização da câmera por meio de dois dois servos (horizontal e vertical);
- Transmissão da imagem da câmera.

## 3. Configuração do ambiente
### Requisitos de hardware
| Componente               | Quantidade |
|--------------------------|------------|
| ESP32 DevKit v1          | 1          |
| TTGO T-CAMERA ESP32      | 1          |
| Servo Motor - SG90       | 2          |
| Ponte H - L298N          | 1          |
| Motor DC 3-6V            | 2          |
| Roda 68mm                | 2          |
| Roda Boba Nylon          | 1          |
| Power Bank 5V            | 1          |
| Chassi MDF               | 2          |

### Requisitos de software
- Possuir o projeto "clonado" em sua máquina;
- Possuir a Arduino IDE para utilização do ESP32 https://randomnerdtutorials.com/getting-started-with-esp32/

## Organização e Arquitetura
### Diagrama de Arquitetura
### Conexões
### Comunicação

## Considerações finais
### Dificuldades encontradas

## Referências
- Configuração do ambiente de desenvolvimento: https://randomnerdtutorials.com/getting-started-with-esp32/
- Servo motor: https://esp32io.com/tutorials/esp32-servo-motor
- Criação da comunicação websocket: https://lastminuteengineers.com/esp32-websocket-tutorial/#uploading-the-code