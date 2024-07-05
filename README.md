Documentação do Projeto
Introdução
Este projeto implementa um jogo interativo utilizando uma matriz de LEDs 8x8 controlada por um módulo MD_MAX72XX e um anel de LEDs NeoPixel, com botões físicos para interação do usuário. O objetivo do jogo é encontrar lugares seguros em uma grade, evitando "bombas" escondidas.

Hardware Utilizado
Matriz de LEDs 8x8 (MD_MAX72XX)
Anel de LEDs NeoPixel (Adafruit NeoPixel)
Módulo de controle de matriz de LEDs (MD_MAX72XX)
Botões físicos
Arduino (ou qualquer microcontrolador compatível)
Buzzer
Componentes e Conexões
Matriz de LEDs:

Tipo do hardware: GENERIC_HW
Pinos:
DATA_PIN: 10 (DIN)
CS_PIN: 9
CLK_PIN: 11
Número de módulos: 1
Anel de LEDs:

Quantidade de LEDs: 12
Pino de controle: 12
Botões:

Botão A: pino 2
Botão B: pino 3
Botão C: pino 4
Botão D: pino 5
Botão Analógico: pino 8
Outros:

Pino do Buzzer: 13
Pinos Analógicos para leitura: PIN_ANALOG_X (A0), PIN_ANALOG_Y (A1)
Variáveis Globais
Controladores de hardware:

MD_MAX72XX matriz: Controlador da matriz de LEDs
MatrizLed Display8X8: Controlador de display personalizado
Adafruit_NeoPixel RODA_LED: Controlador do anel de LEDs
Estado do Jogo:

char ligados[MAX_PONTOS][MAX_TAMANHO_PONTO]: Guarda os pontos ligados
int numPontos: Contador de pontos
bool botaoAnalogicoPressionado: Flag para verificar se o botão analógico foi pressionado
bool estadoLEDs[8][8]: Estado de cada LED na matriz (ligado/desligado)
bool bombas[8][8]: Estado das bombas (verdadeiro/falso)
unsigned long lastButtonPressTimeA, lastButtonPressTimeB, lastButtonPressTimeC, lastButtonPressTimeD, lastButtonPressTimeAnalog: Controle de debounce para os botões
bool rodarColorido: Flag para iniciar animação colorida
int posicaoAtualLinha, posicaoAtualColuna: Posição atual na matriz
int lugaresSegurosMarcados: Contador de lugares seguros marcados
Configuração Inicial
A função setup inicializa a comunicação serial, os controladores de matriz de LEDs e anel de LEDs, define os modos dos pinos e chama as funções inicializarEstados e randomizarBombas.

Função Principal
A função loop é o coração do jogo, verificando a interação dos botões e atualizando o estado do jogo. Ela chama a função verificarBotao para cada botão, processa a entrada do botão analógico, marca lugares seguros e verifica bombas nas proximidades.

Funções Auxiliares
inicializarEstados: Inicializa os estados dos LEDs e bombas como falsos.
randomizarBombas: Aleatoriamente posiciona bombas na matriz.
pintadorDeLed: Controla a cor dos LEDs no anel NeoPixel.
verificarBombasVizinhas: Verifica bombas nas posições adjacentes à posição atual.
verificarBotao: Processa a interação dos botões e atualiza a posição atual na matriz.
animacaoColorida: Executa uma animação colorida no anel de LEDs.
Fluxo de Jogo
Movimentação: O jogador utiliza os botões para mover a posição atual na matriz.
Verificação: Ao pressionar o botão analógico, a posição atual é verificada.
Se houver uma bomba, o LED acende em vermelho e um som é emitido pelo buzzer.
Se não houver bomba, a posição é marcada como segura e o número de bombas nas proximidades é verificado.
Verificação de Bombas Próximas: Se houver bombas nas posições adjacentes (em cima, embaixo, à esquerda ou à direita) da posição atual, o anel de LEDs fica amarelo. Se não houver bombas próximas, o anel de LEDs fica verde.
Condição de Vitória: Se 30 lugares seguros forem marcados, o anel de LEDs fica colorido, indicando que o jogador ganhou o jogo.
