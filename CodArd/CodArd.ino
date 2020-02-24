
#define maxBraco 13
#define maxMao 18

unsigned long millisBracoTest = 0;
unsigned int delayBracoTest = 2000;

String strTemp;
char aux[20] = "";
char temp[3] = "99";
char comando[3] = "99";
char moveControl[5] = "9999";
int bracoControl = 13;
int maoControl = 10;

unsigned long time;

bool responder = false;

//delay parar movimento caso nao exista novo comando
unsigned long millisStop = 0;
unsigned int delayStop = 1100;

//delay para controlar braco e mao
unsigned long millisBracoMao = 0;
unsigned int delayBracoMao = 500;

////////////////////////////////////////////////////
#include <ros.h>
#include <std_msgs/String.h>

ros::NodeHandle listener;
std_msgs::String str_msg;
ros::Publisher answer("answer", &str_msg);
char hello[13] = "hello world!";

void messageROS( const std_msgs::String& toggle_msg);
ros::Subscriber<std_msgs::String> sub("talker" , &messageROS);

void setupROS()
{
  pinMode(13, OUTPUT);  
  listener.initNode();
  listener.subscribe(sub);

  listener.initNode();
  listener.advertise(answer);  
}

void answerROSmsg(char msg[10])
{
  str_msg.data= msg;
  answer.publish( &str_msg );
  listener.spinOnce();
  responder = false;
}

void messageROS( const std_msgs::String& toggle_msg)
{
  responder = true;
  millisStop = millis();
  //digitalWrite(13, HIGH-digitalRead(13));   // blink the led
//  digitalWrite(13, HIGH);   // blink the led
  
//  if(toggle_msg.data[0] == '9')
//  {

    //str_msg.data = hello;
    //answer.publish( &str_msg );
    //listener.spinOnce();
    
//    digitalWrite(13, LOW);   // blink the led
//    delay(5000);
//  }
  strcpy(moveControl, toggle_msg.data);
}

/*
0giraHorario
1giraAntiHorario
2frente
3tras
4esquerda
5direita
6dFD //diagonalFrenteDireita
7dTE //diagonalTrasEsquerda
8dFE //diagonalFrenteEsquerda
9dTD //diagonalTrasDireita
 */
void setup() {
  setupUteis();
  setupLocomover();
  setupServos();
  setupEletroima();
  setupUltrasonic();
  setupPressao();
  setupROS();
  //Serial.begin(9600); 
  //Serial.println("--- Start Serial Monitor SEND_RCVE ---");
}

void loop() {
  listener.spinOnce();

  //if(delayMillis(&millisBracoMao, delayBracoMao)) //Comanda braco e mao a cada intervalo de tempo
  //{
    moveBraco(bracoControl);
    moveMao(maoControl); 
  //}
  //stringOne.substring(14,18)
  strncpy(comando,moveControl,2);  
  switch (atoi(comando)) {
    case 0:
      strcpy(aux, "giraHorario");
      giraHorario();
      break;
    case 1:
      strcpy(aux, "giraAntiHorario");
      giraAntiHorario();
      break;
    case 2:
      strcpy(aux, "frente");
      frente();
      break;
    case 3:
      strcpy(aux, "tras");
      tras();
      break;
    case 4:
      strcpy(aux, "esquerda");
      esquerda();
      break;
    case 5:
      strcpy(aux, "direita");
      direita();
      break;
    case 6:
      strcpy(aux, "dFD");
      dFD();
      break;
    case 7:
      strcpy(aux, "dTE");
      dTE();
      break;
    case 8:
      strcpy(aux, "dFE");
      dFE();
      break;
    case 9:
      strcpy(aux, "frente");
      dTD();
      break;
    case 21:
      //bracoCima 
      strcpy(aux, "bracoCima");     
      strncpy(temp,moveControl+2,2);
      bracoControl = atoi(temp);
      //bracoControl = 10;
      strTemp = String(bracoControl);
      strTemp.toCharArray(aux,20);
      break;
    case 22:
      //bracoBaixo 
      strcpy(aux, "bracoBaixo");   
      strncpy(temp,moveControl+2,2);
      bracoControl = atoi(temp);
      //bracoControl = 3;
      strTemp = String(bracoControl);
      strTemp.toCharArray(aux,20);
      break;
    case 31:
      //maoCima 
      strcpy(aux, "maoCima");     
      strncpy(temp,moveControl+2,2);
      maoControl = atoi(temp);
      strTemp = String(maoControl);
      strTemp.toCharArray(aux,20);
      break;
    case 32:
      //maoBaixo 
      strcpy(aux, "maoBaixo");   
      strncpy(temp,moveControl+2,2);
      maoControl = atoi(temp);
      strTemp = String(maoControl);
      strTemp.toCharArray(aux,20);
      break;
    default:
      strcpy(aux, "default");
      break;
  }  
  if(responder)
    answerROSmsg(aux);
  if(delayMillisKeep(&millisStop, delayStop)) //Se não receber comando, fica parado
  {
    moveControl[0] = '-';
  }
}






/*
//utilizado para teste apenas
// movimenta o braco e a mao em todas as POSICOES possiveis
void testBracoMao()
{
  if(delayMillis(&millisBracoTest, delayBracoTest))
  {
    if(bracoAumentar)
    {
      if(bracoControl < maxBraco)
        bracoControl++;
      if(bracoControl >= maxBraco) //nunca deve ser maior
        bracoAumentar = false;
    }
    else
    {
      if(bracoControl > 0)
        bracoControl--;
      if(bracoControl <= 0) //nunca deve ser menor
        bracoAumentar = true;      
    }
    
    if(MaoAumentar)
    {
      if(maoControl < maxMao)
        maoControl++;
      if(maoControl >= maxMao) //nunca deve ser maior
        MaoAumentar = false;      
    }
    else
    {
      if(maoControl > 0)
        maoControl--;
      if(maoControl <= 0) //nunca deve ser menor
        MaoAumentar = true;  
    }      
  }
}

//utilizado para teste apenas
// locomove o carrinho em todas as DIRECOES possiveis
void testLocomover()
{
  if(millis() < 1000 * 2)
  {      
      strcpy(moveControl, "0giraHorario");
  }
  else if(millis() < 2000 * 2)
  {
      strcpy(moveControl, "1giraAntiHorario");
  }
  else if(millis() < 3000 * 2)
  {
      strcpy(moveControl, "2frente");
  }
  else if(millis() < 4000 * 2)
  {
      strcpy(moveControl, "3tras");
  }
  else if(millis() < 5000 * 2)
  {
      strcpy(moveControl, "4esquerda");
  }
  else if(millis() < 6000 * 2)
  {
      strcpy(moveControl, "5direita");
  }
  else if(millis() < 7000 * 2)
  {
      strcpy(moveControl, "6dFD");
  }
  else if(millis() < 8000 * 2)
  {
      strcpy(moveControl, "7dTE");
  }
  else if(millis() < 9000 * 2)
  {
      strcpy(moveControl, "8dFE");
  }
  else if(millis() < 10000 * 2)
  {
      strcpy(moveControl, "9dTD");
  }
  else
  {
      strcpy(moveControl, "-default");
  }
  
}
*/
