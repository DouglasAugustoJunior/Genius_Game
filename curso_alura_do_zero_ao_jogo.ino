// Constantes - Sem ponto e vírgula

// LEDs
#define LED_GREEN 2
#define LED_YELLOW 3
#define LED_RED 4
#define LED_BLUE 5

// Botões
#define BUTTON_GREEN 8
#define BUTTON_YELLOW 9
#define BUTTON_RED 10
#define BUTTON_BLUE 11

// Buzzer
#define BUZZER 13

#define UNDEFINED -1 // Para diferenciar das portas válidas

// Timers
#define UM_SEGUNDO 1000
#define MEIO_SEGUNDO 500

#define TAMANHO_SEQUENCIA 4

// Variáveis

int sequenciaLuzes[TAMANHO_SEQUENCIA];
int rodada = 0;
int leds_respondidos= 0;

enum Estados{
  PRONTO_PROX_RODADA,      // 0
  USUARIO_RESPONDENDO,     // 1
  JOGO_FINALIZADO_SUCESSO, // 2
  JOGO_FINALIZADO_FALHA    // 3
};


void setup(){ // Método chamado ao iniciar

  iniciaPortas();
  iniciaJogo();

}

void iniciaPortas(){

  // Monitor
  Serial.begin(9600); // Habilitando terminal de Log

  // LEDs
  pinMode(LED_GREEN,OUTPUT);  // Porta onde está conectado = 2, Saída
  pinMode(LED_YELLOW,OUTPUT); // Porta onde está conectado = 3, Saída
  pinMode(LED_RED,OUTPUT);    // Porta onde está conectado = 4, Saída
  pinMode(LED_BLUE,OUTPUT);   // Porta onde está conectado = 5, Saída
  
  //Botões
  pinMode(BUTTON_GREEN,  INPUT_PULLUP); // PULLUP => Informa que o Arduino deve usar seu resistor interno
  pinMode(BUTTON_YELLOW, INPUT_PULLUP);
  pinMode(BUTTON_RED,    INPUT_PULLUP);
  pinMode(BUTTON_BLUE,   INPUT_PULLUP);
  
  //Buzzer
  pinMode(BUZZER, OUTPUT);

}

void iniciaJogo(){
    
  int game = analogRead(0); // Lê a porta analógica aberta que vai passar valores aleatórios
  randomSeed(game); // Atribui valor aleatório á função random()
  
  for(int i = 0; i < TAMANHO_SEQUENCIA; i++) { // Uma cor aleatória para cada posição
    sequenciaLuzes[i] = sorteiaCor();
  }

}

int sorteiaCor(){
  return random(LED_GREEN, LED_BLUE + 1); // DE 2 ATÉ 5
}

void loop(){

  switch(estadoAtual()) {

  case PRONTO_PROX_RODADA: //executa funções próxima rodada
    preparaNovaRodada();
  break;
    
  case USUARIO_RESPONDENDO:
    processaRespostaUsuario();
  break;
    
  case JOGO_FINALIZADO_SUCESSO:
    jogoFinalizadoSucesso();
  break;
    
  case JOGO_FINALIZADO_FALHA:
    jogoFinalizadoFalha();
  break;
  }

  delay(MEIO_SEGUNDO);
}

void preparaNovaRodada(){

  rodada++; // Incrementa a rodada
  leds_respondidos = 0; // limpa os LEDs respondidos para nova rodada
  if(rodada <= TAMANHO_SEQUENCIA){ // Se ainda está dentro das rodadas, assim não toca os LEDs depois de acabar o jogo
     tocaLedsRodada(); // Pisca os LEDs dessa rodada
  }

}

void processaRespostaUsuario(){

  int resposta = checaRespostaJogador(); // Pega o clique do usuário

  if(resposta == UNDEFINED){ // Se ele ainda não respondeu ele segura
    return;
  }

  if(resposta == sequenciaLuzes[leds_respondidos]){ // Se acertou
    leds_respondidos++;
  }else{
    rodada = TAMANHO_SEQUENCIA +2; // Vai dar falha no estadoAtual()
  }

}

int estadoAtual(){ // Retorna o estado atual do jogo

  if(rodada <= TAMANHO_SEQUENCIA){ // se a sequencia ainda é maior que a rodada
    if(leds_respondidos == rodada) { // se já respondeu todos os LEDs
       return PRONTO_PROX_RODADA; // Avança de rodada
     }else{ // Se ainda falta responder
       return USUARIO_RESPONDENDO; // Aguarda
    }
  }else if(rodada == TAMANHO_SEQUENCIA +1){ // Se passou pela tamanho da sequência +1 ele terminou a última fase
    return JOGO_FINALIZADO_SUCESSO;
  }else{ // Senão ele errou a cor e perdeu
    return JOGO_FINALIZADO_FALHA;
  }

}

void tocaLedsRodada(){ // Mostra a sequência atual

  for(int i = 0; i < rodada; i++){
    piscaLed(sequenciaLuzes[i]);
  }

}

void jogoFinalizadoSucesso(){ // Na sequência

  tocaSom(3000); // Som de vitória

  piscaLed(LED_GREEN);
  piscaLed(LED_YELLOW);
  piscaLed(LED_RED);
  piscaLed(LED_BLUE);
  delay(MEIO_SEGUNDO);

}

void jogoFinalizadoFalha(){ // Todos juntos

  tocaSom(300); // Som de derrota

  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_YELLOW, HIGH);
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_BLUE, HIGH);
  
  delay(MEIO_SEGUNDO);
  
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_BLUE, LOW);
  
  delay(MEIO_SEGUNDO);

}

int piscaLed(int led){ // Recebe o LED que deve piscar

  verificaSomDoLed(led); // Som ao piscar
  digitalWrite(led,HIGH); // Acende o led
  delay(UM_SEGUNDO); // espera 1s para apagar
  digitalWrite(led,LOW); // Apaga o led
  delay(MEIO_SEGUNDO); // espera 1s para acender
  return led; // Retorna o valor do LED

}

void tocaSom(int frequencia){ // Sons do jogo
  tone(BUZZER, frequencia, 100); // Componente, frequência, timer
}

void verificaSomDoLed(int portaLed){

  switch (portaLed) {
    case LED_GREEN:
      tocaSom(2000);
      break;
    case LED_YELLOW:
      tocaSom(2200);
      break;
    case LED_RED:
      tocaSom(2400);
      break;
    case LED_BLUE:
      tocaSom(2500);
      break;
  }

}

int checaRespostaJogador(){ // Retorna o botão pressionado

    if(digitalRead(BUTTON_GREEN) == LOW){
        return piscaLed(LED_GREEN); // Pisca o LED correspondente e retorna ele
    }
    if(digitalRead(BUTTON_YELLOW) == LOW){
        return piscaLed(LED_YELLOW);
    }
    if(digitalRead(BUTTON_RED) == LOW){
        return piscaLed(LED_RED);
    }
    if(digitalRead(BUTTON_BLUE) == LOW){
        return piscaLed(LED_BLUE);
    }

    return UNDEFINED; // Se nada foi pressionado retorna uma porta inválida

}