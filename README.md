<center>

# Computação Física e Aplicações
## ACH2157
## Escola de Artes, Ciências e Humanidades
**Autores**
Juan
Ryan Alexsander Forti **126932674**
Vitor



</center>


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
| Jumpers                  | x          |

### Requisitos de software
- Possuir o projeto "clonado" em sua máquina;
- Possuir a Arduino IDE para utilização do ESP32 https://randomnerdtutorials.com/getting-started-with-esp32/

## Organização e Arquitetura
### Diagrama de Arquitetura
<img src="./assets/esquema.png" />

### Componentes físicos
- **ESP32**


- **Ponte H**


- **Servos Motores**


- **TTGO T-CAMERA ESP32**


### Conexões

### Comunicação

### Bibliotecas externas

## Considerações finais
### Dificuldades encontradas
O grupo encontrou algumas dificuldades durante o desenvolvimento do projeto. Em um primeiro momento, a intenção do grupo era utilizar apenas o compenente ESP32CAM realizando o papel de subir uma rede WiFi, sendo um AP (*access point*), uma pagína web para receber os comandos do usuário e compartilhar a imagem capturada, além de gerenciar os outros componentes físicos. Porém o grupo não conseguiu fazer os ajustes do ESP32CAM, conseguindo seguir com o projeto apenas com a troca do componente para uma TTGO T-CAMERA ESP32.
Apesar de solucionar um problema, gerou outro, a nova câmera não possui conexões suficientes para gerenciar os demais componentes físicos. Com isso, foi necessário adicionar um ESP32, este conecta na rede estabelecida pela câmera e realiza o gerenciamento da página web e componentes físicos.
O que leva ao terceiro ponto de dificuldade do grupo, pois aumentando a quantidade de componentes, aumenta a quantidade de energia necessária, aparentemente o powerbank não está conseguindo fornecer energia suficiente, levando a uma perca de conexão dos componentes e o usuário perdendo a conexão, temporariamente, com a página web. Pois fornecendo energia de uma fonte separada (Notebook, via cabo USB) para o ESP32 e do powerbank para o restante dos compenentes, não observamos esse comportamento de falha, tendo funcionamento íntegro do projeto. 

## Referências
- Configuração do ambiente de desenvolvimento: https://randomnerdtutorials.com/getting-started-with-esp32/
- Servo motor: https://esp32io.com/tutorials/esp32-servo-motor
- Criação da comunicação websocket: https://lastminuteengineers.com/esp32-websocket-tutorial/#uploading-the-code
