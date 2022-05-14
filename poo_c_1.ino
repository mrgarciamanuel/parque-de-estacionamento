// C++ code
//
//biblioteca do servo
#include<Servo.h>

//variáveis que farão e gestão dos lugares no parque
#define totLotacao 10
int lugaresDispo = 3;

//constantes para os botões
#define BTN_Entrada 13
#define BTN_Saida 12

//variaveis para cada cor do LED RGB
int LED_RED = 11;
int LED_BLUE = 10;
int LED_GREEN = 9;

//constantes para cancela
#define CancelaFechada 0
#define cancelaAberta 177

//instanciação da classe servo
Servo cancela;
int pino_cancela = 2;

//Sensor de sistância
int TRIG = 6;
int ECHO = 8;

float distancia;

//classe pesssoa é uma classe abstrata
struct Pessoa{
	protected:
  String nome;
  int idade;
};

 //class Utilizador
  class Utilizador:public Pessoa{
  	private:
    String matricula_user;//matricula do carro
      
    public:
    int totTicket= 10;//o total de tickets que um utilizador pode comprar
    int ticket;//nº de bilhetes que um user tem na conta
    
    //metodo construtor de Utilizadores
    Utilizador(String _nome, int _idade, String _matricula_user){
      this->nome = _nome;
      this->idade = _idade;
      this->matricula_user = _matricula_user;
      //this->matricula = _matricula;
      this->ticket = 3; 
    }
    
    //metodos getters e setters de Utilizador
    public:
    void SetNome(String _nome){
    	this->nome = _nome;
    }
    String GetNome(){
    	return this->nome;
    }
    
    void SetIdade(int _idade){
    	this->idade = _idade;
    }
    int GetIdade(){
    	return this->idade;
    }
    
    void SetMatricula_user(String _matricula_user){
    	this->matricula_user = _matricula_user;
    }
    String GetMatricula(){
    	return this->matricula_user;
    }
    
    //função que permite ao utilizador pagar ticket
    void pagarTicket(int n){// n é o número de tickets que se deseja adquirir
      if (ticket >= 0 && ticket < totTicket){
    	ticket = ticket + n;
        Serial.println("Carregado");
      }else if (ticket = totTicket){
        Serial.println("Numero de Tickets completo");                      	
      }
    }
    
    //função que permite ao utilizador entrar no parque
    void entrarNoParque(){
      if (lugaresDispo <=0){
      	AcenderVermelho();
        Serial.print("Vagas do parque lotadas!");
      }else{
        AcenderVerde();
        Serial.print("Lugares dispiveis no parque:");
        Serial.println(lugaresDispo);
      	String ajuda = Interface("Insira a sua matricula:");
        
        //se cliente existe e tiver tickets disponivel na conta
        if (ajuda == this->matricula_user && this->ticket >= 1){
        	Serial.println("Seja bem vindo ao parque");
          	lugaresDispo --;
          	this->ticket --;
          	cancela.write(177);
        }else{
          Serial.println("Cliente nao registado ou tickets esgotados!");        
        }
      }
    }
    
    //função que permite ao utilizador sair do parque
    void sairDoParque(){
        String ajuda = Interface("Insira o valor da sua matricula:");
        if (ajuda == this->matricula_user){
          Serial.println("Obrigado, Volte sempre!");
          lugaresDispo ++;
          cancela.write(177);
        }else{
          Serial.print("Algo correu mal, insira de novo a sua matricula");
        }
       }
  };

class Visitante: public Pessoa{
  private:
  String matricula_vis;
  
  public:
  int hora = 2; //variavel que armazena o valor pago por hora
  int cartao; //variavel que armazena o cartão multibanco do visitante
  
  //metodo construtor dos visitantes
  Visitante(String _nome, int _idade, String _matricula_vis){
    this->nome = _nome;
    this->idade = _idade;
    this->matricula_vis = _matricula_vis;
    this->cartao = 100;
  }
  
  //getters e setters de visitantes
  void setNome(String _nome){
  	this->nome = _nome;
  }
  String GetNome(){
  	return this->nome;
  }
  
  void SetIdade(int _idade){
  	this->idade = _idade;
  }
  int GetIdade(){
  	return this->idade;
  }
  
  void SetMatricula(String _matricula_vis){
  	this->matricula_vis = _matricula_vis;
  }
  String GetMatricula(){
  	return this->matricula_vis;
  }
  
  //funcao entar no parque para visitantes
  void entrarNoParque(){
    if (lugaresDispo == 0){
      	AcenderVermelho();
    	Serial.println("Nao pode ter acesso ao parque, parque esta lotado");
    }else{
      AcenderVerde();
      Serial.print("Lugares disponiveis no parque:");
      Serial.println(lugaresDispo);
      String ajuda = Interface("Insira a sua matricula:");
      //ajuda2 vai armazenar a quantidade de tickets que um utilizador não fidelizado terá que comprar
      int ajuda2 = Interface("Insira a qtd de tickets que pretende comprar:").toInt();
      if (ajuda == matricula_vis){
        if (cartao < (hora*ajuda2)){
          Serial.println("Nao pode pagar o parque, valor insuficiente no cartao");
        }else{
          cartao = cartao - (hora*ajuda2);
          Serial.println("Seja bem vindo ao parque");
          lugaresDispo --;
          cancela.write(177);
        }
      }
  	}
  }
  
  //funcao sair do parque para visitantes
  void sairDoParque(){
  	String ajuda = Interface("Insira a sua matricula:");
      if (ajuda == matricula_vis){
    	Serial.println("Obrigado, volte sempre!");
        lugaresDispo ++;
        cancela.write(177);
    }
    else{
    	Serial.println("Algo correu mal, Insira de novo a sua matricula");
    }
  }
};
	//array de objetos do tipo utilizador
	Utilizador *users[5];
	
	//array de objetos  do tipo visitante
	Visitante *guests[5];


void setup()
{
  cancela.write(0);
  //comunicação em serie
  Serial.begin(9600);
  
  //Modus pinos
  pinMode(13, INPUT);

  //modus pinos dos botões
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  
  //Modus pinos do sensor
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);  
  
  //metodo que faz ligação do servo 
  cancela.attach(pino_cancela);  
  
  //criação de um objeto/instanciação de utilizador
  users[0] = new Utilizador ("Garcia",20,"XQ-33-XQ");
  users[1] = new Utilizador("Michel", 25, "AS-13-DH");
  users[2] = new Utilizador("Yanisleide", 25, "AV-20-MS");
  users[3] = new Utilizador("Luis", 25, "AP-63-US");
   
  //criação de um objeto/instanciação de visitante
	guests[0] = new Visitante("Manuel", 25, "AD-23-DS");
}

void loop()
{ 
  
  digitalWrite(TRIG, LOW);
  delay(0005);
  digitalWrite(TRIG, HIGH);
  delay(0010);
  digitalWrite(TRIG, LOW);
  
  distancia = pulseIn(ECHO, HIGH);
  distancia = distancia / 58;
  //Serial.println(distancia);
  
  if (distancia >= 60){
  	Serial.println("Carro passou");
    delay(1000);
  }
  
  
  //users[0]->entrarNoParque();
  //users[1]->entrarNoParque();
  //users[2]->entrarNoParque();
  //users[3]->entrarNoParque();
  
  //guests[0]->entrarNoParque();
  
  Serial.print("Lugares disponiveis no parque:");
  Serial.println(lugaresDispo);
  delay(500);
  
}

//Input de dados 
String Interface(String pergunta){
	String string;
  Serial.println(pergunta);
  do{
  	string = Serial.readString();
  }while(string=="");
  return string;
}

//metodo que faz o semaforo ficar a vermelho
void AcenderVermelho(){
  analogWrite(LED_RED, 255);
  analogWrite(LED_BLUE, 0);
  analogWrite(LED_GREEN, 0);
}
//metodo que faz o semaforo ficar a verde
void AcenderVerde(){
  analogWrite(LED_RED, 0);
  analogWrite(LED_BLUE, 0);
  analogWrite(LED_GREEN, 255);
}


