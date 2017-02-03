#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Dns.h>
#include <Wire.h>
#include <SdFat.h>
#include <SdFatUtil.h>
//#include <Password.h>


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  //indirizzo mac
byte ip[] = { 192,168,1, 130 };   //indirizzo IP
byte gateway[] = { 192,168,1, 1 };
byte dns1[] = { 192,168,1,1 };
byte subnet[] = { 255,255,255,0 };

EthernetServer server(80);       //porta in ascolto su arduino
EthernetUDP Udp;     // utilizzato per la richiesta Udp dns, ntp
IPAddress timeServer(0, 0, 0, 0); // pool.ntp.org NTP server 

SdFat sd; //per accedere all'SD, inizializza e indica l'SD attiva

String msg;                 //variabile per memorizzare la parte url della pagina 
String msg1, msg2, setstr;//variavili utili   //msg2  memorizza la pagina da aprire
String str, sdata;
short unsigned int dimensione=6;
unsigned long int d=0;
char line[6];     //char line1[3];
int y=0, z=0;
unsigned long n;//variabile utulizzata per il passaggio della posizione del cursore all'interno del file
boolean fineriga;
short int pin[54];  //indica se è high/low utilizzato per leggere i pin settati sui file.txt
short int pinrw[54];  //indica se è read/write
byte sincdataora[7];// viene memorizzato il valore della data e dell'ora   //sincdataora[GIORNO][MESE][ANNO][ORA][MINUTI][SECONDI][GIORNOSETTIMANA]
char weekday[7][4] = {"GIO","VEN","SAB","DOM","LUN","MAR","MER"};

unsigned long temp=0;

//ArduinoOutStream cout(Serial);

struct Pin {
  int short orario[1];
  boolean pinstate[54];
  boolean pinio[54];
  short int duratamin[54];
  boolean giornisettimana[7];
}pinhw, pinhwtest;
byte temp1=0;
short int timesessione=0;
//byte oraprecedente=0;
//
void setup(){
    
   Serial.println(F("inizio debug seriale Versione 4.9 del 06/11/2013 "));    //commentati i Serilia.print + funzione magicpacket 
   Serial.begin(9600); 
   Serial1.flush();
   Serial2.flush();
   Serial3.flush(); //inizializza la seriale, la seriale viene utilizzata solo per il debug del programma
   
   Wire.begin();
    
   if (!sd.begin(4, SPI_FULL_SPEED)){  //HALF  //SPI_FULL_SPEED  //verifico che sia andata a buon fine l'inizzializzazione del dell'SD 
                    Serial.println(F("riavviare dispositivo, togliere la corrente"));  //creare anche la versione pagina Web
                    //reboot();
                    sd.initErrorHalt("Errore lettura scheda SD");                 //blocca la continuazione del programma attivare ......................................
   }
   
   Serial.println(F("File presenti sulla SD e dimensione :\n"));                   //cout << pstr("File presenti sulla SD e dimensione :\n");
   sd.ls(LS_SIZE||LS_DATE||LS_R);  // elenca file presenti sull' SD con la dimensione in byte e la data di modifica.
   //delay(2000);
  
  //Reset dell'arduino con i parametri di base rimuovendo i file della configurazione dell'interfaccia ethernet e degli utenti 
  //int buttonPin=8;     // the number of the pushbutton pin
  //int ledPin=13;      // the number of the LED pin
  int buttonState=0;         // variable for reading the pushbutton status
  // initialize the LED pin as an output:
  pinMode(7, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(8, INPUT);
  buttonState=digitalRead(8); //reset account e ethernet
  
  
  
  int buttonState10=0;         // variable for reading the pushbutton status
  // initialize the LED pin as an output:
  pinMode(9, OUTPUT);      
  // initialize the pushbutton pin as an input:
//Attenzione settaggio pin problema con ethernet shield*************** 
 pinMode(10, INPUT);
 buttonState10=digitalRead(10); //reset file dei giorni della settimana
 // pinMode(10, OUTPUT);     // change this to 53 on a mega
 //pinMode(53, OUTPUT);     // change this to 53 on a mega
 //Fine Attenzione settaggio pin problema con ethernet shield*************** 

  if (buttonState==HIGH) { //contolla sei il pulsante è premuto 
              int i=0;
              Serial.print(F(".....................Reset di arduino dell'interfaccia ethernet e degli utenti...............................pulsante premuto"));
              do{  
                 digitalWrite(7, HIGH);   // turn LED on:
                 delay(100); 
                 digitalWrite(7, LOW);
                 delay(100); 
                 i++; 
              }while(i<20);  
              sd.remove("eth.tcp");
              sd.remove("LOGIN.PWD");
  }
//  
//  if (buttonState10==HIGH) {///da finire____________________
//              int i=0;
//              Serial.print(F(".....................Reset di arduino dei file settimanale e i rispettivi pin dell'Arduino...............................pulsante premuto"));
//              do{  
//                 digitalWrite(7, HIGH);   // turn LED on:
//                 //letturafilepinreset(SdFat sd1, int pin);  // si deve creare un file con i pin che si possono resettare leggerlo e passarlo alla funzione 
//                 delay(100); 
//                 digitalWrite(7, LOW);
//                 delay(100); 
//                 //i++; 
//              }while(i<54); 
//
//  }
  
  //Fine Reset arduino
  
   configurazionetcpip(sd, "eth.tcp", mac, ip, gateway, dns1, subnet);
   Ethernet.begin(mac, ip, dns1,  gateway, subnet);      //inizializza server con mac e IP dns gateway + pagina login
   server.begin();   
   
   if(!sd.exists("login.pwd")){
         Serial.println(F("  File login.pwd non esiste, creazione del file con utente admin e password adminn     "));
         adminpwd(sd, "LOGIN.PWD", "admin", "admin", "admin.htm");
  //       adminpwd(sd, "LOGIN.PWD", "davide", "admin1111", "admin.htm");
  //       adminpwd(sd, "LOGIN.PWD", "domenico", "admin", "admin.htm");
   }
   
  //Serial.println(F("Lista dei files di configurazione:"));
  //letturafile(sd,".TXT");
  //letturafile(sd,".BCK");
  letturafileprova(sd,".TXT"); 
  letturafileprova1(sd,".TXT");
  //letturafileprova2(sd,".TXT");
    
   //inizzializzazione degli array
   Serial.println(F("                                               Inizzializzazione:struct pinhw"));
   azzera(pinhw.orario, 1, -1);
   azzera(pinhw.pinstate, 54, 0);
   azzera(pinhw.pinio, 54, 1);
   azzera(pinhw.duratamin, 54, -1); 
   // modifica webserver_SDfat_10_14
   Serial.println(F("                                               Inizzializzazione: struct pinhwtest"));
   azzera(pinhwtest.orario, 1, -1);
   azzera(pinhwtest.pinstate, 54, 0);
   azzera(pinhwtest.pinio, 54, 1);
   azzera(pinhwtest.duratamin, 54, -1);
   // modifica webserver_SDfat_10_14 fine
   azzera(sincdataora, 6, 0);                                         //inizzializazzioni array giorno settimana
   azzera(pin, 54, 0);
    
  // pinMode(4,OUTPUT);  //pin per la lettura e scrittura sull'SD
  // digitalWrite(4,HIGH);
    
  // delay(2000); 
   temp = synctime(sd, sincdataora); //sincronizzazione orario con l NTP e successivamente con RTC
   Serial.println(F("::::::::::::::::::       Inizializzazione Arduino Terminata       ::::::::::::"));
   
   for(int i=0; i<54; i++){  //test pin 
             if(i==33 || i==34 || i==35){
                              pinMode(i, OUTPUT);
                              digitalWrite(i, HIGH);
                              delay(1500);
                              digitalWrite(i, LOW);
              }     
   }
}


void loop() {

  Serial1.flush();
  Serial2.flush();
  Serial3.flush();
  msg=" ";
  msg1=" ";
  msg2=" ";
  setstr=" ";
  str=" ";
  int num=0;
  int i=0;  
  short int error=0;
  
  leggids1307(sincdataora);        //aggiorna l'array sincdataora[] con ora minuti secondi leggendolo dall'RTC
//  if(sincdataora[3]!=oraprecedente){
//         oraprecedente=sincdataora[3];
//         PgmPrint("Free RAM: ");
//         Serial.println(FreeRam());         
//  }

 if(temp1<sincdataora[4]){
         n=1;  
         n=n*sincdataora[3]*10200+(170*sincdataora[4])+6; //6 sono i primi byte dell'orario, 170 sono i byte in ogni riga dei file del giorno settimana e 60 i minuti in 1 ora
         readfiletime(sd, n, sincdataora[6], pin, pinrw);
         temp1=sincdataora[4];
         Serial.print(F("Orario:      "));
         if(sincdataora[3] < 10) {
          //Serial.print(('0');
         }
         Serial.print(sincdataora[3]);
         Serial.print(F(":"));
         if(sincdataora[4] < 10) {
          Serial.print(F("0"));
         }
         Serial.print(sincdataora[4]);
         Serial.print(F(":"));
         if(sincdataora[5] < 10) {
          Serial.print(F("0"));
         }
         Serial.println(sincdataora[5]);
         Serial.print(F("Date:   "));
         Serial.print(weekday[sincdataora[6]]);
         Serial.print(F("  "));
         if(sincdataora[0] < 10) {
          Serial.print(F("0"));
         }
         Serial.print(sincdataora[0]);
         Serial.print(F("\\"));
         if(sincdataora[1] < 10) {
          Serial.print(F("0"));
         }
         Serial.print(sincdataora[1]);
         Serial.print(F("\\"));
         Serial.print(F("20"));
         Serial.println(sincdataora[2]);
         for(int i=0; i<54; i++){
               if(i==33 || i==34 || i==35){
                            pinMode(i, OUTPUT);
                            digitalWrite(i, pin[i]);
               }     
         }  
  }    
  
 else if(sincdataora[4]==0 && sincdataora[4]!=temp1){   
        //Serial.println("             ingresso settaggio pin fisici su Arduino  minuto 0       ");
        //Serial.println(sincdataora[4]);
        n=1;
        n=n*sincdataora[3]*10200+6; 
        readfiletime(sd, n, sincdataora[6], pin, pinrw);
        temp1=0;
        Serial.print(F("Orario:      "));
        Serial.print(sincdataora[3]);
        Serial.print(F(":"));
        Serial.println(sincdataora[4]);
        for(int i=0; i<54; i++){
             if(i==33 || i==34 || i==35){//(i!=4 && i!=50 && i!=51 && i!=52 && i!=53 && i!=10)
                          pinMode(i, OUTPUT);
                          digitalWrite(i, pin[i]);
             }     
      }
      // modifica webserver_SDfat_10_14
      for(int i=0; i<54; i++){ //per il test                                                          
            if(pinhwtest.pinstate[i] && pinhwtest.duratamin[i]){
                  pinhwtest.duratamin[i]=pinhwtest.duratamin[i]-1;
                  pinMode(i, OUTPUT);
                  digitalWrite(i, pin[i]);
            }
      }
      if(timesessione>0){// sessione di 10 minuti per utenti autenticati
              timesessione=timesessione-1;
              Serial.print(F("Scadenza sessione fra:  "));
              Serial.println(timesessione);        
      }
      // modifica webserver_SDfat_10_14 fine
 }
       
     
 if(sincdataora[3]==17 && sincdataora[4]==2){  // una volta al giorno si sincronizza rtc col valore 
        Serial.println(F("Ora di aggiornamento Orario 17:02"));
        temp = synctime(sd, sincdataora);       //Sincronizazione dell'oraio con l'NTP  con la chiamata dns per risolvere il nome del server NTP 

 } 




//UUUUUUUJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJ 
// Udp.begin(8888);
// int packetSize = Udp.parsePacket();
//  if(packetSize)
//  {
//    Serial.print("Received packet of size ");
//    Serial.println(packetSize);
//    Serial.print("From IP : ");
//    IPAddress remote = Udp.remoteIP();
//    //print out the remote connection's IP address
//    Serial.print(remote);
//
//    Serial.print(" on port : ");
////    //print out the remote connection's port
//    Serial.println(Udp.remotePort());
//  } 
//  server.begin();
//UUUUUUUJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJ 

//INIZIO VERIFICA
 EthernetClient client = server.available();  //Verifica se il server è disponibile per la connessione, true se è disponibile,
                                       // false se non disponibilene oppure è già connesso un altro client.
 if (client) {
      Serial.println(F("     Apertura connessione Client   "));
      boolean currentLineIsBlank = true;
      while (client.connected()) {              //legge la richiesta html carattere per carattere inviata dal client
 //     Serial.println(F("lettura della richiesta html da parte del client"));
        char c;   
        if (client.available()) {
            c= client.read();
            msg += c;                             //lettura della header richiesto dal client carattere per carattere dentro alla variabile msg 
//        //Serial.println("*********************************************************valore c ");
//        //Serial.print(c);
//        //Serial.println("*********************************************************valore msg ");
//        //Serial.print(msg);
        if (c == '\n' && currentLineIsBlank) {     //quando c=='\n' richiesta terminata da parte del client
//         
//          Serial.print(msg);                      //tutta la richiesta la richiesta html inviata dal client
//          Serial.print(F(" Header client di richiesta al server "));
//        
           statuscode(&client);    // invia una risposta al client (standard http response header)
 
           msg1=msg.substring(6,20);   //dentro msg è presente tutta la richiesta html inviata dal client e dal 6 al 20 l'indirizzo richiesto che viene inserito nella variabile msg1
//                                      //il nome file presente nell'SD non puo superarare 8.3 caratteri  esempio prova123.txt
//          //msg="";
           Serial.println(F("il nome file presente nell'SD non puo superarare 8.3 caratteri  esempio prova123.txt"));
//           Serial.println(msg);
           if(msg1.charAt(0)!=' '){    //SE invece msg1.charAt(0)==' '  apre subito la pagina index.htm presente sulla SD, se non recupera il corretto url ed il file associato presente sull'SD      
                 i=0;
                 msg2=readurl(msg1, i, &i, 9);  
//               Serial.print(F("*****************valoreuscitafuz**************"));
//               Serial.print(i);
//               Serial.print(msg2.charAt(i));  
//               Serial.print(' ');   
//               Serial.print(F("*****************uscitafunzione**************i:"));  
//               Serial.println(i);
                 switch (msg2.charAt(i)) {                                                //msg.charAt(i)
//
                            case '?':{
                                      Serial.print(F("***************** CASE ?**************"));
//                                      Serial.print(msg2);
//                                      Serial.print(F("************"));
                                      msg2=msg2.substring(0,i);   //paginaweb da aprire senza ?
////                                      Serial.print(msg2);
////                                      Serial.println("************");

////                                      //creazione del file per il salvataggio dei dati inviati dal form web
////                                      // delete the file:
//
                                       char contenitore[13];   //ho provato a mettere i+2 mi va in errore, ho messo la massima lunghezza del file  8.3 = 12 + 1 /n
//                                                                //L'array contenitore[13] conterra' il nome del file da creare sull'SD dove memorizzare la stringa passata dalla pagina web dopo il ?            
//                                        //Serial.print("************crea il nome del file dove salvare i dati con lo stesso nome della pagina ma con estensione .txt  prima array:");
//                                        //Serial.print(contenitore);
//                                        //Serial.println("************");
//                                        
                                       msg1=msg2.substring(0,i-3);  //crea il nome del file dove salvare i dati con lo stesso nome della pagina ma con estensione .txt
                                       msg1.concat("bck");
//                                        Serial.print(F("************crea il nome del file stringa .txt"));
//                                        Serial.print(i);
//                                        Serial.print(F("************"));
//                                        Serial.print(msg1);
//                                        Serial.println(F("************"));
                                       msg1.toCharArray(contenitore,i+1);
//                                        Serial.println(F("************"));
                                       //Serial.print(F("************crea il nome del file dove salvare i dati con lo stesso nome della pagina ma con estensione .txt  array:"));
                                       //Serial.print(contenitore);
                                       //Serial.println(F("************"));
                             

                                        //fine creazione file per dati
                                       msg1="";
                                        //msg="";
                                       i+=7;
                                       int q, t, x;
                                       x=i+350;    //lunghezza massima per l'invio dei dati tramite il metodo get 255
                                       q=0;
                                       t=0;
//                                        
                                       str=(String)sincdataora[0]+" "+(String)sincdataora[1]+" "+(String)sincdataora[2]+"    "+(String)sincdataora[3]+':'+(String)sincdataora[4]+':'+(String)sincdataora[5]+"    ";

                                       //int short salvataggio=0;
                                       //int short pinnunber=0 ;
                                       //setstr="s";
                                       String tempstring="";
                                       //azzera(pinhw.pinstate, 54, 0);
                                       do{   
                                           setstr="s";                                       
                                           i+=q;
                                           msg1=readurl(msg.substring(i,x), t, &q, 9);           //passa la stringa letta dopo il ? con i dati inviati con il metodo post finche non trova il pulsante invio etichettato con salva+ lo spazio                                                                     
                                           str+=msg1; ///da verificare
                                           t=1;
//                                           Serial.println("                          Stringa da anlizzare     ");
//                                           Serial.println(msg);
//                                           Serial.print("     lunghezza della stringa:  ");
//                                           Serial.println(msg.length());
//                                           Serial.println("                            Seconda stringa passata");
//                                           Serial.println(msg1);
//                                           Serial.print("       lunghezza seconda stringa   ");
//                                           Serial.println(msg1.length());
//                                           
//       
//                                             Serial.print(F("salvataggio  dati passati:      "));
//                                             Serial.print(msg1);
//                                             Serial.print(F("             "));
//                                             Serial.print(setstr);
//                                             Serial.println(F("      fine dati passati************"));
                                             

                                             if(msg1=="on&" ){  //|| salvataggio==1
                                                       do{   
                                                           //inviaMagicPacket();//____________________________________________________________________________da testare                                                         
                                                           //onoff=3;
                                                           //salvataggio=1;
                                                           Serial.println(F("            ON    <---------------------------------------------"));
                                                           //azzera(pinhw.pinstate, 54, 0);
                                                           for(int f=0; f<54 ; f++ ){  
                                                              if(msg1==(setstr+f+'=')){
                                                                    Serial.print(F("                <--------------------- pin da settare vero        ------------     "));
                                                                    Serial.println(msg1);
                                                                    tempstring=msg1;
                                                                    i+=q;
                                                                    msg1=readurl(msg.substring(i,x), t, &q, 9);           //passa la stringa letta dopo il ? con i dati inviati con il metodo post finche non trova il pulsante invio etichettato con salva+ lo spazio                                                                     
                                                                    t=1;
                                                                    //onoff=pinhw.pinstate[f] =( (msg1=="on&" ? true: false));
                                                                    //Serial.print("settaggio pin:  "+ tempstring);
                                                                    //Serial.println(pinhw.pinstate[f]);
//                                                                    if(onoff=0){
//                                                                              //Serial.println("fare ricerca sul file .txt e cancellare tutti i pin settati come off");
//                                                                              onoff=0;                    
//                                                                    }
                                                                    pinhw.pinstate[f]=1;
                                                                    //pinon[f]=1;
//                                                                    Serial.print(F("Valore di onoff:  "));
//                                                                    Serial.print(onoff);
//                                                                    Serial.print(F("  valore di onoff=pinhw.pinstate[f]:   "));
//                                                                    Serial.print(pinhw.pinstate[f]);
//                                                                    Serial.println(F("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"));
                                                                    i+=q;
                                                                    msg1=readurl(msg.substring(i,x), t, &q, 9);           //passa la stringa letta dopo il ? con i dati inviati con il metodo post finche non trova il pulsante invio etichettato con salva+ lo spazio                                                                     
                                                                    t=1;
                                                                   // pin=f;
                                                                    
//                                                                   
//                                                                    for(int i=0; i<7 ; i++ ){
//                                                                             //Serial.print("reset giorni settimana");
//                                                                             pinhw.giornisettimana[i]=false; //char weekday[7][4] = {"GIO","VEN","SAB","DOM","LUN","MAR","MER"};                                                                             
//                                                                             //Serial.println(pinhw.giornisettimana[i]);
//                                                                    }
                                                                 }
                                                             }
                                                           //int xc=0;
                                                           for(int f=0; f<54 ; f++ ){
                                                              // modifica webserver_SDfat_10_15
                                                                    if(pinhw.pinstate[f]){
                                                                       Serial.print(F("                                   fare ricerca sul file .txt e cancellare tutti i pin settati alla fine del ciclo "));                                       
                                                                       if(!letturafilepinreset(sd, f)) Serial.println(F(" Errore nel restet dei file"));                                                     
                                                                    } 
                                                                              
                                                               // modifica webserver_SDfat_10_15 fine  
                                                          } 
                                                          for(int i=0; i<7 ; i++ ){
                                                                             //Serial.print("reset giorni settimana");
                                                                             pinhw.giornisettimana[i]=false; //char weekday[7][4] = {"GIO","VEN","SAB","DOM","LUN","MAR","MER"};                                                                             
                                                                             //Serial.println(pinhw.giornisettimana[i]);
                                                          }
                                                           //do{
                                                              for(int f=0; f<54  ; f++ ){
                                                                 for(int g=0; g<54  ; g++ ){
                                                                   while((msg1==(setstr+f+"s=")) || (msg1==(setstr+f+"s"+g+"="))){  //recupera il giorno della settimana che è stato settato dal interfaccia web
                                                                       //Serial.print("                              "+set+f+"s=");
                                                                       i+=q;
                                                                       msg1=readurl(msg.substring(i,x), t, &q, 9);           //passa la stringa letta dopo il ? con i dati inviati con il metodo post finche non trova il pulsante invio etichettato con salva+ lo spazio                                                                     
                                                                       t=1;
                                                                       
                                                                       if(msg1=="lun&"){
                                                                          //Serial.println(F(" LUNEDI "));
                                                                          pinhw.giornisettimana[4]=true;                                                                          
                                                                       }
                                                                       else if(msg1=="mar&"){
                                                                           //Serial.println(F(" MARTEDI"));
                                                                           pinhw.giornisettimana[5]=true;
                                                                       }
                                                                       else if(msg1=="mer&"){
                                                                            //Serial.println(F(" MERCOLEDI"));
                                                                            pinhw.giornisettimana[6]=true;
                                                                       }
                                                                       else if(msg1=="gio&"){
                                                                             //Serial.println(F(" GIOVEDI"));
                                                                             pinhw.giornisettimana[0]=true;
                                                                       }
                                                                       else if(msg1=="ven&"){
                                                                             //Serial.println(F(" VENERDI"));
                                                                             pinhw.giornisettimana[1]=true;
                                                                       }
                                                                       else if(msg1=="sab&"){
                                                                             //Serial.println(F(" SABATO"));
                                                                             pinhw.giornisettimana[2]=true;
                                                                       }
                                                                       else if(msg1=="dom&"){
                                                                             //Serial.println(F(" DOMENICA"));
                                                                             pinhw.giornisettimana[3]=true;
                                                                       }
                                                                       else {
                                                                             Serial.println(F(" Errore nel valutare il giorno della settimana"));
                                                                       }
                                                                       i+=q;
                                                                       msg1=readurl(msg.substring(i,x), t, &q, 9);           //passa la stringa letta dopo il ? con i dati inviati con il metodo post finche non trova il pulsante invio etichettato con salva+ lo spazio                                                                     
                                                                       t=1;
                                                                    }
                                                                   }
                                                                 }                                                                   
                                                                 for(int f=0; f<54 ; f++ ){ //recupera l'ora della settimana che è stato settato dal interfaccia web
                                                                   //for(int g=0; g<54 ; g++ ){
                                                                    if(msg1==(setstr+f+"h=")){   //if((msg1==(setstr+f+"h="))||(msg1==(setstr+f+"h"+g+"=")))
                                                                      //Serial.print("                                        settaggio ora dopo passaggio dati");
                                                                       i+=q;
                                                                       msg1=readurl(msg.substring(i,x), t, &q, 9);           //passa la stringa letta dopo il ? con i dati inviati con il metodo post finche non trova il pulsante invio etichettato con salva+ lo spazio                                                                     
                                                                       t=1;
                                                                       //Serial.print("                                           "+set+f+"ora=");
                                                                       String prova =msg1.substring(0,2);
                                                                       //Serial.println(prova);
                                                                       char pot[2];
                                                                       prova.toCharArray(pot,3);
                                                                       pinhw.orario[0]=((int)pot[0]-'0')*10+((int)pot[1]-'0');
                                                                       Serial.print(F("pinhw.orario[0]       "));
                                                                       Serial.println(pinhw.orario[0]);
                                                                       i+=q;
                                                                       msg1=readurl(msg.substring(i,x), t, &q, 9);           //passa la stringa letta dopo il ? con i dati inviati con il metodo post finche non trova il pulsante invio etichettato con salva+ lo spazio                                                                     
                                                                       t=1;
                                                                    }
                                                                   //}
                                                                 }
                                                                 for(int f=0; f<54  ; f++ ){     //recupera i minuti della settimana che è stato settato dal interfaccia web
                                                                   //for(int g=0; g<54 ; g++ ){
                                                                    if(msg1==(setstr+f+"m=")){ //if((msg1==(setstr+f+"m="))||(msg1==(setstr+f+"m"+g+"="))) 
                                                                      //Serial.print("                                        settaggio minuti dopo passaggio dati");
                                                                       i+=q;
                                                                       msg1=readurl(msg.substring(i,x), t, &q, 9);           //passa la stringa letta dopo il ? con i dati inviati con il metodo post finche non trova il pulsante invio etichettato con salva+ lo spazio                                                                     
                                                                       t=1;
                                                                       //Serial.print("                                           "+set+f+"minuti=");
                                                                       String prova =msg1.substring(0,2);
                                                                       //Serial.println(prova);
                                                                       char pot[2];
                                                                       prova.toCharArray(pot,3);
                                                                       pinhw.orario[1]=((int)pot[0]-'0')*10+((int)pot[1]-'0');
                                                                       Serial.print(F("pinhw.orario[1]       "));
                                                                       Serial.println(pinhw.orario[1]);
                                                                       i+=q;
                                                                       msg1=readurl(msg.substring(i,x), t, &q, 9);           //passa la stringa letta dopo il ? con i dati inviati con il metodo post finche non trova il pulsante invio etichettato con salva+ lo spazio                                                                     
                                                                       t=1;
                                                                    }
                                                                   //}
                                                                 }
                                                                 for(int f=0; f<54  ; f++ ){  //recupera la durata  che è stato settato dal interfaccia web
                                                                    if(msg1==(setstr+f+"t=")){
                                                                      //Serial.print("                                        settaggio durata dopo passaggio dati");
                                                                       i+=q;
                                                                       msg1=readurl(msg.substring(i,x), t, &q, 9);           //passa la stringa letta dopo il ? con i dati inviati con il metodo post finche non trova il pulsante invio etichettato con salva+ lo spazio                                                                     
                                                                       t=1;
                                                                       //Serial.print("                                           "+set+f+"durata=");
                                                                       Serial.print(F("Durata dell'irrigazione------------------------------------:"));
                                                                       Serial.println(msg1);
                                                                       Serial.print(F("lunghezza della stringa:"));
                                                                       Serial.println(msg1.length());
//                                                                       String prova =msg1.substring(0,2);
//                                                                       char pot[2];
//                                                                       prova.toCharArray(pot,3);
//                                                                       pinhw.duratamin[0]=((int)pot[0]-'0')*10+((int)pot[1]-'0');
//                                                                       Serial.print(F("duratamin[0]       "));
//                                                                       Serial.println(pinhw.duratamin[0]);
                                                                       //String prova =msg1.substring(0,msg1.length());
                                                                       
                                                                       char pot[4]={'0', '0', '0', '0'};
                                                                       String prova =msg1.substring(0,msg1.indexOf('&'));
                                                                       //Serial.print(F("Stringa prova    :"));
                                                                       //Serial.println(prova);
                                                                       //Serial.print(F("lunghezza della stringa:"));
                                                                       //Serial.println(prova.length());
                                                                       int lung=prova.length();
                                                                       //Serial.print(F("stampa sringa caratter per carattere:"));
                                                                       int j=3;
                                                                       lung--;
                                                                       for(int i=lung; i>=0 ; i--) {   //&& lung<4
                                                                                         pot[j]=prova.charAt(i);
                                                                                         j--;
                                                                       }

                                                                       pinhw.duratamin[0]=((int)pot[0]-'0')*1000+((int)pot[1]-'0')*100+((int)pot[2]-'0')*10+((int)pot[3]-'0');
                                                                       Serial.print(F("duratamin[0]       "));
                                                                       Serial.println(pinhw.duratamin[0]);

                                                                    }
                                                                  }
                                                                  for(int j=0; j<54; j++){ 
                                                                     if(pinhw.pinstate[j]){                                
                                                                              //Serial.println(F(" giorni settimana settati nel ciclo on:"));
                                                                              //for(int i=0; i<7; i++) Serial.println(pinhw.giornisettimana[i]); //char weekday[7][4] = {"GIO","VEN","SAB","DOM","LUN","MAR","MER"};
                                                                              for(int f=0; f<54 ; f++ ){                                                              
                                                                                         if(pinhw.pinstate[f]){
                                                                                                  Serial.print(F("  fare ricerca sul file .txt e scrive i nuovi valori, pin:"));                      
                                                                                                  Serial.println(f);
                                                                                                  if(!letturafilepinset(sd, pinhw.orario[0], pinhw.orario[1], pinhw.duratamin[0],pinhw.giornisettimana, f)) Serial.println(" Errore nel settaggio dei pin dei file");
                                                                                          }
                                                                             } 
                                                                      }                                         

                                                                      else if (j==53){
                                                                             Serial.println(F("                                               alla fine inizzializza Inizzializzazione:"));
                                                                             azzera(pinhw.orario, 1, -1);
                                                                             azzera(pinhw.pinstate, 54, 0);
                                                                             azzera(pinhw.pinio, 54, 1);
                                                                             azzera(pinhw.duratamin, 54, -1);
                                                                             azzera(pinhw.giornisettimana, 7, 0);
                                                                      }
                                                                      else{  //else if(pinhw.pinstate[j]==0)
                                                                              Serial.print(F("                                   fare ricerca sul file .txt     NON FARE NULLA ciclo: "));
                                                                              Serial.println(j);
                                                                      }
                                                                  }
                                                                  i+=q;
                                                                  msg1=readurl(msg.substring(i,x), t, &q, 9);           //passa la stringa letta dopo il ? con i dati inviati con il metodo post finche non trova il pulsante invio etichettato con salva+ lo spazio                                                                                                                                   
                                                                  t=1;                              
                                                              
                                           }while(!msg1.endsWith(" "));
                                           }
                                           else if(msg1=="off&" ){                  //|| salvataggio==2                                         
                                                         do{
                                                            //salvataggio=2;
                                                            Serial.print(F("            OFF    <---------------------------------------------   "));
                                                            //Serial.println(msg1);                           
                                                            for(int f=0; f<54 ; f++ ){  
                                                               if(msg1==(setstr+f+'=')){
                                                                    //Serial.print(F("                <--------------------- pin cancellare vero nel ciflo OFF        ------------     "));
                                                                    //Serial.println(msg1);
                                                                    tempstring=msg1;
                                                                    if(!letturafilepinreset(sd, f)) Serial.println(F(" Errore nel restet dei file")); 
                                                                }
                                                                else if(f==53){ 
                                                                       azzera(pinhw.orario, 1, -1);
                                                                       azzera(pinhw.pinstate, 54, 0);
                                                                       azzera(pinhw.pinio, 54, 1);
                                                                       azzera(pinhw.duratamin, 54, -1);
                                                                       azzera(pinhw.giornisettimana, 7, 0);
                                                                }   
                                                              }
                                                              //tempstring=msg1;
                                                              i+=q;
                                                              msg1=readurl(msg.substring(i,x), t, &q, 9);           //passa la stringa letta dopo il ? con i dati inviati con il metodo post finche non trova il pulsante invio etichettato con salva+ lo spazio                                                                     
                                                              t=1;                                                             
                                                            }while(!msg1.endsWith(" "));       
                                           }
                                           // modifica webserver_SDfat_10_14
                                           else if(msg1=="test&" ){  //|| salvataggio==3
                                           // modifica webserver_SDfat_10_14
                                                           //Serial.println(F("    Reset------   contatori ed altro per la funzione TEST"));
//                                                           for(int i=0; i<54; i++){                                                           
//                                                                pinhwtest.pinstate[i]=0;
//                                                                pinhwtest.duratamin[i]=0;
//                                                           }
                                          // modifica webserver_SDfat_10_14 fine
                                                           setstr="i";
                                                           do{
                                                           //salvataggio=3;
                                                           //Serial.print(F("            TEST    <---------------------------------------------   "));
                                                           //Serial.println(msg1); 
                                                           
                                                           for(int f=0; f<54 ; f++ ){  
                                                               if(msg1==(f+setstr+'=')){
                                                                         //Serial.print(F("                <--------------------- pin da settare vero  in TEST      ------------     "));
                                                                         //Serial.println(msg1);
                                                                         tempstring=msg1;
                                                                         i+=q;
                                                                         msg1=readurl(msg.substring(i,x), t, &q, 9);           //passa la stringa letta dopo il ? con i dati inviati con il metodo post finche non trova il pulsante invio etichettato con salva+ lo spazio                                                                     
                                                                         t=1;
                                                                         pinhwtest.pinstate[f] =( (msg1=="on&" ? true : false));
                                                                         //Serial.print("settaggio pin:  "+ tempstring);
                                                                         //Serial.println(pinhw.pinstate[f]);                                                                          
                                                                         i+=q;
                                                                         msg1=readurl(msg.substring(i,x), t, &q, 9);           //passa la stringa letta dopo il ? con i dati inviati con il metodo post finche non trova il pulsante invio etichettato con salva+ lo spazio                                                                     
                                                                         t=1;
                                                                }
                                                            }                                                           
                                                            for(int f=0; f<54  ; f++ ){  //recupera la durata  che è stato settato dal interfaccia web
                                                                    if(msg1==("im=")){
                                                                       //Serial.print(F("                                        settaggio durata dopo passaggio dati in TEST"));
                                                                       i+=q;
                                                                       msg1=readurl(msg.substring(i,x), t, &q, 9);           //passa la stringa letta dopo il ? con i dati inviati con il metodo post finche non trova il pulsante invio etichettato con salva+ lo spazio                                                                     
                                                                       t=1;
                                                                       //Serial.print("                                           "+set+f+"durata=");

                                                                       char pot[4]={'0', '0', '0', '0'};
                                                                       String prova =msg1.substring(0,msg1.indexOf('&'));
                                                                       int lung=prova.length();
                                                                       int j=3;
                                                                       lung--;
                                                                       for(int i=lung; i>=0 ; i--) {   //&& lung<4
                                                                                         pot[j]=prova.charAt(i);
                                                                                         j--;
                                                                       }
                                                                       pinhw.duratamin[0]=((int)pot[0]-'0')*1000+((int)pot[1]-'0')*100+((int)pot[2]-'0')*10+((int)pot[3]-'0');
                                                                       Serial.print(F("duratamin[0]       "));
                                                                       Serial.println(pinhw.duratamin[0]);
                                                                       //Serial.print("duratamin[0]       ");
                                                                       //Serial.println(pinhw.duratamin[0]);
                                                                    }
                                                            }                                                          
                                                                                                                     
                                                            i+=q;
                                                            msg1=readurl(msg.substring(i,x), t, &q, 9);           //passa la stringa letta dopo il ? con i dati inviati con il metodo post finche non trova il pulsante invio etichettato con salva+ lo spazio                                                                     
                                                            t=1;
                                                           
                                                         }while(!msg1.endsWith(" ")); 
                                                         //Serial.println(F("    Reset------   contatori  TEST Pin settati: "));
                                                         for(int i=0; i<54; i++){                                                           
                                                                //Serial.println(pinhw.pinstate[i]);
                                                                
                                                         } 
                                                         //Serial.println(F("    Reset------   contatori TEST   Durata:   "));
                                                         for(int i=0; i<54; i++){                                                         
                                                                //Serial.println(pinhw.duratamin[i]);
                                                         }                                                     
                                                                                                                                                                                
                                              }
                                              // modifica webserver_SDfat_10_14 fine
                                              else if(msg1=="login=" ){
                                                                       Serial.print(F("            LOGIN <--------------------------------------------------- "));
                                                                       String user="";
                                                                       i+=q;
                                                                       msg1=readurl(msg.substring(i,x), t, &q, 15);           //passa la stringa letta dopo il ? con i dati inviati con il metodo post finche non trova il pulsante invio etichettato con salva+ lo spazio                                                                     
                                                                       t=1;
                                                                       user=msg1;
                                                                       
                                                                       Serial.println(F(""));
                                                                       Serial.print(F("            USER <---------------------------------------------------"));
                                                                       Serial.print(user);
                                                                       Serial.println(F("......"));
                                                                       i+=q;
                                                                       msg1=readurl(msg.substring(i,x), t, &q, 15);           //passa la stringa letta dopo il ? con i dati inviati con il metodo post finche non trova il pulsante invio etichettato con salva+ lo spazio                                                                     
                                                                       t=1;
                                                                       Serial.println(F(""));
 
                                                                       i+=q;
                                                                       msg1=readurl(msg.substring(i,x), t, &q, 15);           //passa la stringa letta dopo il ? con i dati inviati con il metodo post finche non trova il pulsante invio etichettato con salva+ lo spazio                                                                     
                                                                       t=1;
                                                                       Serial.print(F("            PASSWORD <---------------------------------------------------"));
                                                                       Serial.print(msg1);
                                                                       Serial.println(F("......"));
                                                                       char paginapersonale[13];
                                                                       ricercautente(sd, "Login.pwd",user, msg1, paginapersonale);
                                                                       Serial.print(F("Pagina personale da aprire:  "));
                                                                       msg2="";
                                                                       if(paginapersonale[0]!='\0'){
                                                                            for(int i=0; i<13 && (paginapersonale[i]!=NULL); i++){                                                                                      //Serial.print(paginapersonale[i]);
                                                                                                   msg2+=paginapersonale[i];                                                                                    
                                                                            }
                                                                            timesessione=10;
                                                                       }
                                                                       else error=2;   
                                                                       Serial.println(msg2); 
                                                                       Serial.println(F("------------------------------------------------->  Fine Login "));
                                                                       //msg1=" ";
                                              }
                                              else if(msg1=="mac0=" ){                                                                                                                                                                                                                                              
                                                                       //recupero parametri tcp/ip passati dall'interfaccia web
                                                                       Serial.println(F("           MODIFICA PARAMETRI RETE <--------------------------------------------------- "));
                                                                       Serial.println(F("Parametro recuperato MAC:"));
                                                                       setstr="mac";
                                                                       msg1=paramac(mac, setstr, msg, msg1, &q, &i, x);
                                                                       Serial.println(F("Parametro recuperato IP:"));
                                                                       setstr="ip";
                                                                       msg1=paratcp(ip, setstr, msg, msg1, &q, &i, x);
                                                                       Serial.println(F("Parametro recuperato SUBNET:"));
                                                                       setstr="subnet";
                                                                       msg1=paratcp(subnet,setstr, msg, msg1, &q, &i, x);
                                                                       Serial.println(F("Parametro recuperato GATEWAY:"));
                                                                       setstr="gateway";
                                                                       msg1=paratcp(gateway, setstr, msg, msg1, &q, &i, x);
                                                                       Serial.println(F("Parametro recuperato DNS1:"));
                                                                       setstr="dns1";
                                                                       msg1=paratcp(dns1, setstr, msg, msg1, &q, &i, x);
                                                                       sd.remove("eth.tcp");
                                                                       configurazionetcpip(sd, "eth.tcp", mac, ip, gateway, dns1, subnet);
                                                                       reboot();
                                                                       //fine recupero parametri passati dall'interfaccia web e riavvio di Arduino
                                              }
                                              else if(msg1=="cmp0="){ 
                                                                      //recupero modifiche utenti  passati dall'interfaccia web
                                                                      Serial.println(F("          MODIFICA CREDENZIALI UTENTE <--------------------------------------------------- "));
                                                                      setstr="cmp";                                              
                                                                      msg1=parausr(sd, setstr, msg, msg1, &q, &i, x); //String parausr(SdFat sd1, String alfa, String msgw, String msgt, int* s, int* j, int x)
                                                                      
                                              }
                                              else if(msg1=="agg0="){
                                                                      Serial.println(F("          AGGIUNTO NUOVO UTENTE <--------------------------------------------------- "));
                                                                      error=0;
                                                                      setstr="agg0";
                                                                      String user, password, homepage;
                                                                      char cnamefile[12];
                                                                      user = parausragg(setstr, msg, &msg1, &q, &i, x);
                                                                      Serial.print(F("                User    "));
                                                                      Serial.println(user);
                                                                      if (ricercautenteagg(sd, "Login.pwd", user) || user=="") {  //Verifica se è presente un utente con stesso username
                                                                           Serial.print(F(" Nome utente esistente o errore nel nome vuoto"));
                                                                           error=3;
                                                                      }
                                                                      else{
                                                                           Serial.print(F("Username non presente  nel  File di login"));
                                                                           setstr="agg1";
                                                                           Serial.print(msg1);
                                                                           Serial.println(F("<---"));
                                                                           password = parausragg(setstr, msg, &msg1, &q, &i, x);
                                                                           Serial.print(F("                Password    "));
                                                                           Serial.print(password);
                                                                           Serial.println((password.length()));
                                                                           if((password.length())<5 || password.equalsIgnoreCase(user)){
                                                                                  Serial.println(F("Errore  password  assegnata all'utente troppo corta o uguale all username"));
                                                                                  error=3;      
                                                                           }
                                                                           setstr="agg2";
                                                                           homepage = parausragg(setstr, msg, &msg1, &q, &i, x);
                                                                           homepage.trim();
                                                                           Serial.print(F("                Homepage    "));
                                                                           Serial.println(homepage);
                                                                           homepage.toCharArray(cnamefile,12);
                                                                           if(!sd.exists(cnamefile)){
                                                                                 Serial.println(F("Errore homepage assegnata all'utente non è ptresente sull SD"));
                                                                                 error=3;
                                                                           }  
                                                                      }
                                                                      if(error!=3) adminpwd(sd, "LOGIN.PWD",user ,password ,homepage); 
                                                                      //msg1=" ";                                                                    
                                                                      
                                              }
                                              else if(msg1=="userrm="){
                                                                      Serial.println(F("          RIMOZIONE UTENTE DAL FILE DI LOGIN.PWD    <--------------------------------------"));
                                                                      String user="";
                                                                      i+=q;
                                                                      msg1=readurl(msg.substring(i,x), t, &q, 9);           //passa la stringa letta dopo il ? con i dati inviati con il metodo post finche non trova il pulsante invio etichettato con salva+ lo spazio                                                                     
                                                                      t=1;
                                                                      
                                                                      user=msg1.substring(0,msg1.length()-1);
                                                                      Serial.print(F(" Eliminazione utente ..........."));
                                                                      Serial.print(user);
                                                                      Serial.println(F("..........."));
                                                                      rimoveusr(sd ,user ,"Login.pwd" ,"Logint.pwd");
 
                                              }
                                              else if(msg1=="timez="){
                                                                      Serial.println(F("          MODIFICA TIME ZONE       <--------------------------------------"));
                                                                      String timezone=""; 
                                                                      char pot[7];
                                                                      
                                                                      memset(pot,'&',7);
                                                                      i+=q;
                                                                      msg1=readurl(msg.substring(i,x), t, &q, 9);           //passa la stringa letta dopo il ? con i dati inviati con il metodo post finche non trova il pulsante invio etichettato con salva+ lo spazio                                                                     
                                                                      t=1;                                                                      
                                                                      timezone=msg1.substring(0,msg1.length()); //msg1.length()-1
                                                                      
                                                                      timezone.toCharArray(pot,7);
                                                                      long seconditimezone = synctimezone(pot);
                                                                      
                                                                      SdFile file;
                                                                      sd.chdir(1);
                                                                      if(sd.exists("tzone.ntp")){
                                                                                  Serial.println(F("Rimozione del file timezone.ntp"));
                                                                                  if(!sd.remove("tzone.ntp")){
                                                                                       Serial.println(F("Errore nella rimozione del file timezone.ntp"));
                                                                                       reboot();
                                                                                  } 
                                                                      } 
                                                                      else if(file.createContiguous(sd.vwd(),"tzone.ntp",1)){
                                                                                                Serial.println(F("Creazione del file tzone.ntp  "));
                                                                                                file.print(timezone);
                                                                                                file.close();
                                                                      }
                                                                      else Serial.println(F(" Errore nella creazione del file tzone.ntp "));
                                                                      file.close();
                                                                      temp = synctime(sd, sincdataora);
                                                                                      
                                              } 
                                              else if(msg1=="resetpin="){
                                                                       Serial.print(F("          Reset Dei Pin Di Arduino "));                                       
                                                                       int i=0;
                                                                       do{
                                                                           if(!letturafilepinreset(sd, i)) Serial.println(F(" Errore nel restet dei file"));
                                                                           i++;
                                                                       }while(i<54);
                                                
                                                
                                              } 
                                              else{
                                                          Serial.println(F("            FUORI CAMPO    <--------------------------------------"));
                                              }

                                       }while(!msg1.endsWith(" "));   // while(!msg1.equals("salva ")); da provare                                         

                                       if (!scritturafile(sd, contenitore,str+'\n')){//Verifica se il file esiste se non presente lo crea e scrive i la stringa passata dal metodo get                                                                   
                                                   Serial.print(F("Errore creazione file o scrittura"));
                                                   error =1;
                                                   statuscoderrsd(&client);                   
                                       }

                                       break;
                                       }

                           case ' ':
                                      Serial.println(F("*****************CASE spazio**************"));
                                      msg2.trim();
                                      Serial.print(F("stringa di richiesta:    "));
                                      Serial.println(msg2);
                                      if(msg2.endsWith("ico") || msg2.endsWith("css") || msg2.endsWith("jsq") || msg2.endsWith("htm") ||  msg2.endsWith("TXT") ||  msg2.endsWith("BCK")|| msg2.endsWith("png") || msg2.endsWith("jpg")){
                                      }
                                      else{
                                           msg2+=".htm"; 
                                           i+=4; 
                                      }                              
                                      break;          
                           default: 
                                    Serial.println(F("*******************CASE default****"));
                                   
                } 
                
                if(error==1){ 
                      statuscoderrsd(&client);  
                }
                else if(error==2){
                      statuscoderrautenticazione(&client);
                      Serial.println(F("statuscoderrautenticazione(&client)"));
                } 
                else if(error==3){
                      statuscoderrautenticazione(&client);
                      Serial.println(F("Errore nella creazione dell'utente, problema con utente password o homepage assegnata"));
                }
                else{
                      // modifica webserver_SDfat_10_14
                    if(timesessione>0){                    
                          msg2.trim();
                          if(msg2.equals("admin.htm")) admin(&client,sd);
                          else if(msg2.equals("ethad.htm")) ethad(&client);
                          else if(msg2.equals("usrad.htm")) usrad(&client, sd);
                          else if(msg2.equals("usragg.htm")) usragg(&client);
                          else if(msg2.equals("userrm.htm")) userrm(&client, sd);
                          else if(msg2.equals("timezone.htm"))timezone(&client);
                          else if(msg2.equals("resetpin.htm"))resetpin(&client);
                          else if(msg2.equals("info.htm"))info(&client);
                          else if(msg2.equals("salvatag.htm"))salvatag(&client);                      
                          else if(!(letturafileinvioclient(sd, msg2, &client)))statuscoderr(&client);
                          else ;
                    }
                    else {
                         sessionescaduta(&client);
                         Serial.print(F("Sessione scaduta o ancora da autenticarsi"));
                    }
                      // modifica webserver_SDfat_10_14 fine
                }             

          }
          if(msg1.charAt(0)==' '){ // pagina web da aprire se inseriamosolo l'ip
                      Serial.println(F("Pagina web da aprire :    autenticazione"));
                      autenticazione(&client);
          }                  

          break;
        }
        if (c == '\n') {
               currentLineIsBlank = true;
        }
        else if (c != '\r') {
               currentLineIsBlank = false;
        }
      }
    }
    
    Serial.println(F("           Chiusura connessione client                                   "));
    client.stop();
  }

  delay(1000);

}


//-------------------------------------------------------------------Inizio Funzioni-----------------------------------------------------------------------------------------------------------------


boolean letturafile(SdFat sd1 ,String testo){ //La variabile testo è utilizzata per verificare l'estensione del file. 
              
              SdFile file,sdfile;
              sd1.chdir(1); // si sposta sulla root dell'SD
              int c;
              String sdata;
              char cnamefile[12];
              while(file.openNext(sd1.vwd(), O_READ )) {
                    sdata=" ";                    
                    file.getFilename(cnamefile);                 
                    sdata=cnamefile;
                    sdata.trim();
                    if(sdata.endsWith(testo)){    //lettura dei solo file in base all'estensione (*.txt) 
                         Serial.println(cnamefile);      
                         sdata.toCharArray(cnamefile,12);
                         if (sdfile.open(cnamefile, O_READ)){                         
                                  //sdfile.errorHalt("open failed");
                                 while ((c = sdfile.read()) >= 0){
                                         Serial.print((char)c);
                                 }                                
                                 Serial.println();
                          }
                          else return(0); //errore apertura file
                          sdfile.close();
                     }
                     file.close();                             
              }           
              return(1);
}


unsigned long int letturafile(SdFat sd1 ,char cnamefile[12], char delim, unsigned long int pos){ 

              SdFile file;
              char line[50];
              int n;
              unsigned long int posizionefile;
              
              sd1.chdir(1);
              if (file.open(cnamefile, O_READ)){ 
                file.seekSet(pos);                    
                while ((n = file.fgets(line, sizeof(line))) > 0) {                     
                    Serial.print(line);                    
              }
              posizionefile=file.curPosition();
              Serial.println("posizione del cursore nel file");
              Serial.println(posizionefile);
              if(n==0) posizionefile=0;
              file.close();
              return(posizionefile); 
              }            
              return(0);             

}







unsigned long sendNTPpacket(IPAddress& address){
       
        unsigned long epoch=0;
        byte packetBuffer[48]; //buffer to hold incoming and outgoing packets 
        // set all bytes in the buffer to 0
        memset(packetBuffer, 0, 48); 
        // Initialize values needed to form NTP request
        // (see URL above for details on the packets)
        packetBuffer[0] = 0b11100011;   // LI, Version, Mode
        packetBuffer[1] = 0;     // Stratum, or type of clock
        packetBuffer[2] = 6;     // Polling Interval
        packetBuffer[3] = 0xEC;  // Peer Clock Precision
        // 8 bytes of zero for Root Delay & Root Dispersion
        packetBuffer[12]  = 49; 
        packetBuffer[13]  = 0x4E;
        packetBuffer[14]  = 49;
        packetBuffer[15]  = 52;
      
        // all NTP fields have been given values, now
        // you can send a packet requesting a timestamp: 		   
        Udp.beginPacket(address, 123); //NTP requests are to port 123
        Udp.write(packetBuffer,48);
        Udp.endPacket();
        if ( Udp.parsePacket() ) {  
              // We've received a packet, read the data from it
              Udp.read(packetBuffer,48);  // read the packet into the buffer
          
              //the timestamp starts at byte 40 of the received packet and is four bytes,
              // or two words, long. First, esxtract the two words:
          
              unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
              unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);  
              // combine the four bytes (two words) into a long integer
              // this is NTP time (seconds since Jan 1 1900):
              //Serial.print(packetBuffer[40],DEC);
              //Serial.print(" ");
              //Serial.print(packetBuffer[41],DEC);
              //Serial.print(" ");
              //Serial.print(packetBuffer[42],DEC);
              //Serial.print(" ");
              //Serial.println(packetBuffer[43],DEC);
              
              unsigned long secsSince1900 = highWord << 16 | lowWord;  
              Serial.print(F("Secondi dal 1 Gennaio 1900 = " ));
              Serial.println(secsSince1900);               
          
              // now convert NTP time into everyday time:
              Serial.print(F("Unix time = "));
              // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
              const unsigned long seventyyears = 2208988800UL;  //secondi in 70 anni   
              // subtract seventy year1s:
              epoch = secsSince1900 - seventyyears;  
            
              //Serial.println(epoch);                               
          
              unsigned long dayCount = epoch / 86400UL;
              
              Serial.print(F("Giorni trascorsi dal 1 gennaio 1970= "));
              Serial.println(dayCount);              
        }
      
        // wait another 599 seconds (one second delay was above)  10 minutes
        for(int y=0;y<20 ;y++){
          // count off the minutes
             if(y%60 == 0) Serial.println(y/60);
             delay(1000); 
        }        
        if(epoch>0) return(epoch);
        //return(sendNTPpacket(timeServer));
        return(0);
}



//calcolo del unixtime in data e ora italiana
byte* unixtimetodate(unsigned long unixtimedate, byte arraydati[]){  
  
        char weekday[7][4] = {"GIO","VEN","SAB","DOM","LUN","MAR","MER"};
        //short int oraitaliana=1;     //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------   
        unsigned long int tmp=0;
        unsigned  int mth[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
        int annoattbisestile =0;    
        int anno1970=1970;
        int giornit;
        int mese =0;
        int giornom=0; 
        unsigned long giornidal70=unixtimedate/86400UL;        
        unsigned long dayt = unixtimedate / 86400UL;
        
        for (int i=0;tmp<=dayt;anno1970++){       //anni bisestili dal 1970 fino ad oggi presenti in i
                giornit = dayt-tmp;
        	if(anno1970%4==0 &&(anno1970%100!=0 || anno1970%400==0)){ 
        		i=i+1;
                        tmp+=366;
                        annoattbisestile=1;        	
        	}
        	else{
                    tmp+=365;
                    annoattbisestile=0;
               }                      
        }
        
        Serial.println(F("Conversione unixtime in data ed ora"));
        anno1970--;
        Serial.print(F("Anno:     "));
        Serial.println(anno1970);
        
        Serial.print(F("Anno attulale bisestile: "));
        Serial.println(annoattbisestile);

        for(int plus=0;  annoattbisestile && (plus<13); plus++){
        						if(mth[plus]>31){	
                                                                     mth[plus]=mth[plus]+1;
                                                                     Serial.println(mth[plus]);
                                                       }
        }
        
        giornit++;  //unixtime parte dal 1 gennaio 1970 ed assegna il valore 0
        
        Serial.print(F("Giorni trascorsi da inizio anno:   "));
        Serial.println(giornit);
        
        for(int a=0; giornit>mth[a];a++){  
                        mese=a+1;
                        giornom=giornit-mth[a];	          //calcolo giorno del mese		
        }
                
        Serial.print(F("mese   "));
        Serial.println(mese);
        Serial.print(F("giorno   "));
        Serial.println(giornom);
        Serial.print(F("Giorno della Settimana = "));
        arraydati[6]=giornidal70%7UL;
        Serial.println(weekday[arraydati[6]]);
        
        int settdainizioanno=(giornit/7)+1;
        Serial.print(F("Settimane trascorse da inizio anno:    "));
        Serial.println(settdainizioanno);
        
        //Serial.print(F("Attuale ora italiana "));       // UTC is the time at Greenwich Meridian (GMT)    print the hour, minute and second:  //ora legale-orasolare  ultima domenica di marzo ultima domenica di ottobre      ora solare da marzo a ottobre   solare=utc+1  legale=utc +2
//        if((3<=mese) && (mese<=10)){
//                            if(!((giornom+7<31) && (mese==3 || mese==10)&& (weekday[giornidal70%7UL]=="DOM"))){                              
//                                       Serial.println(F("          Ora solare         "));
//                                       oraitaliana=2;
//                             }
//                             else{
//                                       Serial.println(F("          Ora legale         "));
//                             }
//        }
        //fine calcolo ora legale-orasolare
        arraydati[3]=((unixtimedate  % 86400L) / 3600);//+oraitaliana; //ora
        arraydati[4]=(unixtimedate  % 3600) / 60;//minuti
        arraydati[5]=unixtimedate %60;//secondi
        
        Serial.print(arraydati[3]); // print the hour (86400 equals secs per day)    + 1 per l'Italia
        Serial.print(':');  
        if ( arraydati[4] < 10 ) {
          // In the first 10 minutes of each hour, we'll want a leading '0'
          Serial.print('0');
        }
        Serial.print(arraydati[4]); // print the minute (3600 equals secs per minute)
        Serial.print(':'); 
        if ( arraydati[5] < 10 ) {
          // In the first 10 seconds of each minute, we'll want a leading '0'
          Serial.print('0');
        }
        Serial.println(arraydati[5]); // print the second 
        arraydati[0]=giornom;
        arraydati[1]=mese;
        arraydati[2]=anno1970;    
        return(arraydati);       
}


//richiesta dns per la risoluzione del nome pool.ntp.org
IPAddress dnsresolve(){
            IPAddress timeServer(0, 0, 0, 0);
            Udp.begin(8888);  // local port to listen for UDP packets
            DNSClient dns;
            dns.begin(Ethernet.dnsServerIP());
            
            if(dns.getHostByName("pool.ntp.org",timeServer) && (!(timeServer ==((IPAddress) (1, 1, 1, 1))))) {
                                       //if(!(timeServer ==((IPAddress) (1, 1, 1, 1)))){   
                                                                Serial.print(F("NTP server ip :"));   //da capire perchè il dns se non risolve il nome da 1.1.1.1
                                                                Serial.println(timeServer);
                                       //}
                                        
            }
            else{
                  Serial.println(F("dns lookup failed"));
                  Serial.println(F("verifiare connessione di rete"));
                  timeServer = (0,0,0,0);
                  }
                  
           Serial.println(F("Ready"));                   
           return (timeServer);
}

int scrivids1307(byte arraydati[]){
  
          //Wire.begin();
          Wire.beginTransmission(0x68);
          //il primo byte stabilisce il registro iniziale da scivere
          Wire.write((byte)0x00);          
                       //specifico il tempo e la data
          Wire.write(decToBcd(sincdataora[5])); //1° byte SECONDI da 0x00 a 0x59
          Wire.write(decToBcd(sincdataora[4])); //2° byte MINUTI da 0x00 a 0x59
          Wire.write(decToBcd(sincdataora[3])); //3° byte ORE da 0x00 a 0x24
          Wire.write(decToBcd(sincdataora[6])); //4° byte GIORNO della settimana da 0x01 a 0x07   Da ricalcolare corettamente
          Wire.write(decToBcd(sincdataora[0])); //5° byte GIORNO del mese da 0x00 a 0x31
          Wire.write(decToBcd(sincdataora[1])); //6° byte MESE da 0x00 a 0x12
          Wire.write(decToBcd((sincdataora[2]-2000))); //7° byte ANNO 0x00 a 0x99
          Wire.endTransmission();
          return (0);
}

                                            
byte* leggids1307(byte arraydati[]){
  
        //Wire.begin();
        Wire.beginTransmission(0x68);
        Wire.write((byte)0x00);
        Wire.endTransmission(); 
        
        Wire.requestFrom(0x68, 7);//richiedo 7 byte dal dispositivo conindirizzo 0x68
        
        //regupero i 7 byte relativi ai corrispondenti registri
        arraydati[5] = bcdToDec(Wire.read());      //Secondi
        arraydati[4] = bcdToDec(Wire.read());    //minuti
        arraydati[3] = bcdToDec(Wire.read() & 0b111111);  //ora
        arraydati[6] = bcdToDec(Wire.read());     //giorno_sett
        arraydati[0] = bcdToDec(Wire.read());      //giorno_mese
        arraydati[1] = bcdToDec(Wire.read());     //mese
        arraydati[2] = bcdToDec(Wire.read());     //anno       
               
//        Serial.print(F("Orario corrente su RTC DS1307: "));
//        Serial.print(arraydati[3]);
//        Serial.print(":");
//        Serial.print(arraydati[4]);
//        Serial.print(":");
//        Serial.println(arraydati[5]);
//
//        Serial.print(F("Giorno della settimana: "));
//        Serial.println(arraydati[6]);
//       
//        Serial.print(F("Data corrente: "));
//        Serial.print(arraydati[0]);  
//        Serial.print("/");
//        Serial.print(arraydati[1]);
//
//        Serial.print("/");
//        Serial.println(arraydati[2]);
//        Serial.println();
        return(arraydati);
        
}

byte decToBcd(byte val){
// Convert normal decimal numbers to binary coded decimal
  return ( (val/10*16) + (val%10) );
}

byte bcdToDec(byte val)  {
// Convert binary coded decimal to normal decimal numbers
  return ( (val/16*10) + (val%16) );
}

                                                 

String readurl(String urlindirizzo, int s, int* p, int offset){    //legge la parte dell'url e lo splitta in base ai caratteri e spazi
      Serial.println(F("                 Funzione readurl     "));
      //int j=*p;
      int j=s;
//      Serial.print(F("funzione stringa******"));
//      Serial.print(j);
//      Serial.print(F("++++++++++++"));
//      Serial.print(urlindirizzo);
      Serial.println(F("****"));
      String msgb="";
      do{
               Serial.print(F("funzione stringa*****dentro do*****"));
               Serial.print(j);
               Serial.print(F("*****"));         
               msgb+=urlindirizzo.charAt(j);
               Serial.println(msgb);               
               j+=1;                
            
      }while(urlindirizzo.charAt(j)!='.' && urlindirizzo.charAt(j)!=' ' && urlindirizzo.charAt(j)!='=' && urlindirizzo.charAt(j)!='&' && (j-s<offset)); //&& j<300   j-s<9 per l'homepage
      msgb+=urlindirizzo.charAt(j);
      
      Serial.print(F("Pagina web da aprire      :  "));
      Serial.println(msgb);
      if(urlindirizzo.charAt(j)=='&' || urlindirizzo.charAt(j)=='='){
                  //j+=1;
//                  Serial.println(F("if(urlindirizzo.charAt(j)=='&' || urlindirizzo.charAt(j)=='=')"));
}
//      if(j-s>=9){
//         Serial.println("if(j-s>9)");
//      }
      if (urlindirizzo.charAt(j)!=' ' && !(j-s>=9) ) {
//            Serial.println(F(" while(urlindirizzo.charAt(j)!=' ' && urlindirizzo.charAt(j)!='?' && urlindirizzo.charAt(j)!='=' && urlindirizzo.charAt(j)!='&' ))"));
            while(urlindirizzo.charAt(j)!=' ' && urlindirizzo.charAt(j)!='?' && urlindirizzo.charAt(j)!='=' && urlindirizzo.charAt(j)!='&' ){   //   && (urlindirizzo.charAt(j)!='=' && urlindirizzo.charAt(j)!='&')
                       j++;
                       msgb+=urlindirizzo.charAt(j);                            					
//                         Serial.print(F("funzione stringa*****+++++++++++++alla fine++++++++++++++++++++*****"));
//                         Serial.print(j);
//                         Serial.print(F("*****"));
//                         Serial.println(msgb);                                            
            }
       }
            
      *p=j;
      return msgb;
}

boolean letturafileinvioclient(SdFat sd1, String filestring, EthernetClient* client1){  //lettura del file sull SD e invio al browser 
              Serial.print(F("Funzione letturafileinvioclient: "));
              Serial.println(filestring);
              SdFile file;
              int n=0,p,s=1400;
              byte line[s];        //1400
              int i= filestring.length()+1;
              char cnamefile[i];
              filestring.toCharArray(cnamefile,i);
              sd1.chdir(1);
              if (file.open(cnamefile, O_READ)){
                        while ((n=file.available())) {
                               if(n>s){           
                                     file.read(line, s);     
                                     client1->write(line, s);
                                     delay(150); //<--------------------------------------------------------------------------------------------------------------------------------------
                               }
                               else{
                                    file.read(line, n);                              
                                    client1->write(line, n); // send web page to client
                               }                   
                       }
                       file.close();
                       return (1);
             } 
             else {
              	// if the file didn't open, print an error:
                    Serial.println(F("errore apertura file"));
                  return (0);
             }
}

boolean scritturafile(SdFat sd1, char cnamefile[],String data){
              Serial.println(F("scritturafile:"));
              SdFile file;
              sd1.chdir(1);
              //int i=0;              
                       
              if(file.open(sd1.vwd(),cnamefile,O_WRITE | O_APPEND)){
                        //Serial.println("file esiste");
                        //i=data.length(); 
                        //Serial.print("Lunghazza stringa:");
                        //Serial.println(i);
                        file.print(data);
                        file.close();
                        
                        return 1;             
              }
              else{
                     Serial.println(F("file non esiste"));
                     if(file.createContiguous(sd1.vwd(),cnamefile,data.length())){
                           file.print(data);     
                           file.close();
                           Serial.println(F("file creato"));
                           return 1;                 
                     }                   
                    else{	             
                         Serial.println(F("Errore creazione scrittura file"));
                         return 0;
                   }        
              }                        
}

char* ricercautente(SdFat sd1, String namefile,String user, String password, char line3[13]){  // char* Ritorna la pagina da aprire per ogni utente
              Serial.println(F("Ricerca utente per accesso:"));
              SdFile file;
              sd1.chdir(1);
              int n=0,  dimensione=100, c=0;  //fineriga=0,
              char cnamefile[13], line2[15];      //15 massima lunghezza del nome utente inpostato su readurl(  ,  ,  ,   ,15)
              String nomeutente, pwd="";
              namefile.toCharArray(cnamefile, 13);
              Serial.print(F("dati passati:  "));
              Serial.println(user+" "+password);              
              if(file.open(sd1.vwd(),cnamefile, O_READ)){  //O_WRITE | O_APPEND
                             Serial.println(F("Apertura file riuscita Login.pwd"));              
                             do{ 
                                 memset(line3,'\0',13); //reset array del nome della pagina da aprire 
                                 nomeutente="";      
                                 Serial.print(F("Utente:    "));
                                       while(((c = file.read()) > 0) && c!=32){  //&& c!='32'   //Ascii 13 = Spazio
                                         if(c!=32){
                                           Serial.print((char)c);
                                           nomeutente+=(char)c;
                                         }
                                 }  
                                 nomeutente+="&";
                                 Serial.println("....."+nomeutente+"....."+user);                              
                                 Serial.println();
 
                                 if(nomeutente.equalsIgnoreCase(user)){                                        
                                           Serial.print(F("Password:    "));
                                           while(((c = file.read()) > 0) && c!=32){//&& c!='32'
                                               if(c!=32){
                                                 Serial.print((char)c);
                                                 pwd+=(char)c;                                               
                                              }
                                           }
                                           pwd+="&";
                                           Serial.println("....."+pwd+"....."+password);                                         
                                           if (pwd.equals(password)){       
                                                       Serial.println(F("Utente autenticato"));
                                                       Serial.println(F("Pagina da aprire per l'utente:"));
                                                       int v=0;
                                                       while (((c = file.read()) > 0)  && v <13 && c!=10 ){ //&& c!=13 && c!=10 && c!=32)   //recupero pagina da aprire per singolo utente
                                                                                    Serial.print((char)c);
                                                                                    line3[v]=(char)c;
                                                                                    v++;
                                                       }
                                                       Serial.println();
                                                       file.close();
                                                       return line3;                                                     
                                           }
                                           else{ 
                                              Serial.println(F("Utente non autenticato password non riconosciuta"));
                                              while(c != 13 && c>0 && c!=10){                                               //Ascii 13 = CR
                                                      c = file.read();   //arriva alla fine della riga
                                                      Serial.print((char)c); 
                                              }
                                              Serial.println(F("fineriga dentro 1  "));                                              
                                           }                   
                                    }     
                                    else{ 
                                         Serial.println(F("Utente non autenticato"));
                                         while(c != 13 && c!=10 && c>0){
                                                      c = file.read();   //arriva alla fine della riga                                                      
                                                      Serial.print((char)c);  
                                       } 
                                      Serial.println(F("fineriga")); 
                                 }
                               }while(c>0);
                               file.close();
                               Serial.println(F("fine Ricerca utente per accesso:"));
                               return line3;
              }             
}

boolean ricercautenteagg(SdFat sd1, String namefile,String user){
              Serial.println(F("   Funzione ricercautenteagg      "));
              SdFile file;
              sd1.chdir(1);
              int n=0,  dimensione=100, c=0;  //fineriga=0,
              char line2[15],cnamefile[13];      //15 massima lunghezza del nome utente inpostato su readurl(  ,  ,  ,   ,15)
              String nomeutente="";
              namefile.toCharArray(cnamefile, 13);
              //Serial.print("dati passati:  ");
              //Serial.println(user);              
              if(file.open(sd1.vwd(),cnamefile, O_READ)){  //O_WRITE | O_APPEND
                             Serial.println(F("Apertura file riuscita Login.pwd"));              
                             do{ 
                                 //memset(line3,'\0',13); //reset array del nome della pagina da aprire 
                                 nomeutente="";      
                                 //Serial.print(F("Utente:    "));
                                       while(((c = file.read()) > 0) && c!=32){  //&& c!='32'   //Ascii 13 = Spazio
                                         if(c!=32){
                                           //Serial.print((char)c);
                                           nomeutente+=(char)c;
                                         }
                                 }
                                 if(nomeutente.equalsIgnoreCase(user)){
                                     file.close();
                                     return (1);
                                 }
                                 do{
                                 
                                 }while((c = file.read()) > 0 && c!='\n');
				 
                              }while(c>0);                              
              }
              file.close();
              return (0);
}

boolean adminpwd(SdFat sd1, String namefile,String user, String password, String homepage){
              Serial.println(F("creazione utente per accesso:"));
              SdFile file;
              sd1.chdir(1);
              char cnamefile[13];
              Serial.println(namefile+" "+user+" "+password+" "+homepage);
              namefile.trim();
              namefile.toCharArray(cnamefile, 13);
//              Serial.print("file passato::::::::::::::::");
//              for (int i=0; i<13; i++){
//                          Serial.print(i);
//                          Serial.print(":");
//                          Serial.print(cnamefile[i]);
//                          Serial.print("......");            
//              }
              Serial.println();
              if(sd1.exists(cnamefile)){
                  Serial.println(F("file esiste"));            
                  //aprie il file in append e aggiungere l'utente creato
                  Serial.println("Creazione del file delle password e dell'utente GENERICO");
                  if(file.open(cnamefile, O_WRITE | O_APPEND)){ //sd1.vwd(),   O_WRITE | O_APPEND
                                                    Serial.println(F("utente creato --------------------------"));
                                                    file.print(user+" "+password+" "+homepage+"\n");
                                                    file.close();
                                                    return 1;
                  }
                  return 0;
             } 
             else{  
                   Serial.println(F("file non esiste"));
                   Serial.println(F("Creazione del file delle password e dell'utente administrator"));
                   //user+=(user+" "+password+" "+"homepage");
                   if(file.createContiguous(sd1.vwd(),cnamefile,user.length())){
                           Serial.println(F("file creato"));
                           file.print(user+" "+password+" "+homepage+"\n");     
                           file.close();
                           return 1;                 
                   }                   
                   return 0;  
             }
             //return 1;
}

unsigned long int letturarigafilepin(SdFat sd1 ,char cnamefile[12],char *line2,short unsigned dimensione,boolean *fineriga, unsigned long int pos){   //d=letturarigafilepin(sd ,"MAR.TXT",line1, 3, &fineriga, d);

              SdFile file;              
              int n;
              unsigned long int posizionefile;
              sd1.chdir(1);
              if (file.open(cnamefile, O_READ | O_WRITE)){ 
                file.seekSet(pos);
                Serial.print("Posozione cursore file");
                Serial.println(file.curPosition());            
                if((n = file.fgets(line2, dimensione)) > 0) {                        
                         if (line2[n-1] == '\0' || line2[n-1] == '\n') { //line2[n-1] == ' '|| 
                         file.close();
                        *fineriga=1;                     
                   }                                     
                }
                //Serial.print("Lettura dell'array:  ");
                //Serial.print(line2[0]);
                //Serial.println(line2[1]);
                //Serial.print(line2[2]);
                //Serial.println(line2[3]);
                // cancellazione -------------------------------------------
                
                Serial.println(file.curPosition());
                //file.seekSet(pos); 
                if(file.write("07")) Serial.println("scrittura");
                // cancellazione -------------------------------------------
                *fineriga=0;
                while(!*fineriga ){
                   if((n = file.fgets(line2, dimensione)) > 0) {  
                         //Serial.println("dentro il ciclo wuhile nella -----------funzione");
                         if (line2[n-1] == '\0' || line2[n-1] == '\n') {
                         file.close();
                        *fineriga=1;                     
                   }                                     
                }
                }
                posizionefile=file.curPosition();
                //Serial.print("posizione del cursore nel file nella funzione :  ");
                //Serial.println(posizionefile);
                if(n==0) posizionefile=0;
                file.close();
                return(posizionefile); 
              }            
              return(0);         
}





boolean letturafileprova(SdFat sd1 ,String testo){ //La variabile testo è utilizzata per verificare l'estensione del file. 
              
              SdFile file,sdfile;
              sd1.chdir(1); // si sposta sulla root dell'SD
              int c;
              String sdata;
              char cnamefile[12];
              while(file.openNext(sd1.vwd(), O_READ )) {
                    sdata=" ";                    
                    file.getFilename(cnamefile);                 
                    sdata=cnamefile;
                    sdata.trim();
                    if(sdata.startsWith("LOGIN")){    //lettura dei solo file in base all'estensione (*.txt) 
                         Serial.println(cnamefile);      
                         sdata.toCharArray(cnamefile,12);
                         if (sdfile.open(cnamefile, O_READ)){                         
                                  //sdfile.errorHalt("open failed");                                  
                                   while ((c = sdfile.read()) >= 0){
                                         Serial.print((char)c);                                         
                                  }                                
                                Serial.println();
                          }
                          else return(0); //errore apertura file
                          //Serial.print("Rimozione file...........................................................");
                          //sdfile.remove();
                          sdfile.close();
                     }
                     
                     file.close();                             
              }           
              return(1);
}


boolean letturafileprova1(SdFat sd1 ,String testo){ //La variabile testo è utilizzata per verificare l'estensione del file. 
              
              SdFile file,sdfile;
              sd1.chdir(1); // si sposta sulla root dell'SD
              int c;
              String sdata;
              char cnamefile[12];
              while(file.openNext(sd1.vwd(), O_READ )) {
                    sdata=" ";                    
                    file.getFilename(cnamefile);                 
                    sdata=cnamefile;
                    sdata.trim();
                    if(sdata.startsWith("ETH")){    //lettura dei solo file in base all'estensione (*.txt) 
                         Serial.println(cnamefile);      
                         sdata.toCharArray(cnamefile,12);
                         if (sdfile.open(cnamefile, O_READ)){                         
                                  //sdfile.errorHalt("open failed");                                  
                                   while ((c = sdfile.read()) >= 0){
                                         Serial.print((char)c);                                         
                                  }                                
                                Serial.println();
                          }
                          else return(0); //errore apertura file
                          sdfile.close();
                     }
                     file.close();                             
              }           
              return(1);
}

boolean letturafileprova2(SdFat sd1 ,String testo){ //La variabile testo è utilizzata per verificare l'estensione del file. 
              
              SdFile file,sdfile;
              sd1.chdir(1); // si sposta sulla root dell'SD
              int c;
              String sdata;
              char cnamefile[12];
              while(file.openNext(sd1.vwd(), O_READ )) {
                    sdata=" ";                    
                    file.getFilename(cnamefile);                 
                    sdata=cnamefile;
                    sdata.trim();
                    if(sdata.startsWith("MER")){    //lettura dei solo file in base all'estensione (*.txt) 
                         Serial.println(cnamefile);      
                         sdata.toCharArray(cnamefile,12);
                         if (sdfile.open(cnamefile, O_READ)){                         
                                  //sdfile.errorHalt("open failed");                                  
                                   while ((c = sdfile.read()) >= 0){
                                         Serial.print((char)c);                                         
                                  }                                
                                Serial.println();
                          }
                          else return(0); //errore apertura file
                          sdfile.close();
                     }
                     file.close();                             
              }           
              return(1);
}


unsigned long int letturafilepinreset(SdFat sd1, int pin){ //La variabile testo è utilizzata per verificare l'estensione del file. 
              Serial.println(F("Reset pin configurati nei file"));
              SdFile file,sdfile;
              sd1.chdir(1); // si sposta sulla root dell'SD              
              String sdata;
              char cnamefile[12];
              unsigned long int n;
              unsigned long int posizionefile;
              unsigned long int pos=6+(pin*3); 
              while(sdfile.openNext(sd1.vwd(), O_READ )) {
                    n=pos;
                    sdata="";                    
                    sdfile.getFilename(cnamefile);                 
                    sdata=cnamefile;
                    sdata.trim();
                    //Serial.println(sdata);
                    if(sdata.equalsIgnoreCase("lun.txt") || sdata.equalsIgnoreCase("mar.txt") || sdata.equalsIgnoreCase("mer.txt") || sdata.equalsIgnoreCase("gio.txt") || sdata.equalsIgnoreCase("ven.txt") || sdata.equalsIgnoreCase("sab.txt") || sdata.equalsIgnoreCase("dom.txt")){
                    //if(sdata.endsWith("TXT")){    //lettura dei solo file in base all'estensione (*.txt)
                         sdata.toCharArray(cnamefile,12);
                         Serial.println(cnamefile);
                         if (file.open(cnamefile, O_READ | O_WRITE)){                         
                                  //sdfile.errorHalt("open failed");
			    for(int f=0; f<1440; f++) {
                            file.seekSet(n);
                            if(file.write("00")){
                                 n=file.curPosition()+168;                                 
                            }
                            else {
                                  Serial.println(F("Errore nella chiusura della scrittura del file"));
                                  file.close();
                                  return (0);                
                           }
                        }                               
                         }

                          else return(0); //errore apertura file
                          file.close();
                     }
                     sdfile.close();                             
              }           
              return(1);
}

unsigned short int letturafilepinset(SdFat sd1, int short ora, int short minuti, int short durata, boolean* a, short int pin){    //letturafilepinset(sd, d, pinhw.orario[0], pinhw.orario[1], pinhw.duratamin[0],pinhw.giornisettimana)
          //char weekday[7][4] = {"GIO","VEN","SAB","DOM","LUN","MAR","MER"};
          Serial.print(F("Funzione letturafilepinset------------------------" ));
          SdFile file;
          sd1.chdir(1);
          unsigned long n=0,sn;
          unsigned long posizionefile=0;
          Serial.print(ora);
          Serial.print(" : ");
          Serial.print(minuti);
          Serial.print("         durata: ");
          Serial.println(durata);
          Serial.print("Pin:   ");
          Serial.println(pin);
          unsigned long temp=1;
          //temp=ora*60+minuti+durata;
          //Serial.print("...........................................................Sforato giorno senza pre assegnazione ......................................................................."); 
          //Serial.println(temp);
          //temp=1;
          temp=(temp*ora*60)+minuti+durata;   //1440 minuti in 24 ore
          if(temp>1440){
            //Serial.print("...........................................................Sforato giorno.......................................................................");
            //Serial.println(temp);
            temp=temp-1440;
            durata=abs(durata-temp);
            //Serial.print("     Valore di sforamento temp  :");
            //Serial.print(temp);
            //Serial.print("      Valore di sforamento durata  :");            
            //Serial.print(durata);
            //Serial.print("       Somma:     ");
            //Serial.println(temp+durata);
          }
          else{
               temp=0;
          }
          pin=pin*3+6;
          //Serial.print("Scostamento:         ");
          //Serial.println(pin);
          //n=170*ora*60;
          n=10200;  //10030
          n=ora*n;
          //n=n-170;
          //Serial.print("(158*ora*60)    ");
          //Serial.println(n);
          //Serial.print("(158*minuti)+pin    :");
          //Serial.println((158*minuti)+pin);
          n=n+(170*minuti)+pin;
          //Serial.print("Posizione cursore file   :   ");
          //Serial.println(n);
          sn=n;
          for(int i=0; i<7 ; i++ ){                                
                     if(a[i]){
                          n=sn;                      
                          switch (i) {                             
                                    case 0:{
                                             Serial.println(F("GIO.TXT"));      //Serial.println(weekday[i]);
                                             if (file.open("GIO.TXT", O_READ | O_WRITE)){                        
                                 			       for(int f=0; f<durata; f++) {
                                                                          Serial.print("Posizione cursore file ciclo   :");
                                                                          Serial.print(f);
                                                                          Serial.print("      ");
                                                                          Serial.println(n);   
                                                                          file.seekSet(n);
                                                                          if(file.write("01")){
                                                                                      n=file.curPosition()+168;
                                                                                      //Serial.print("OK ..................................");
                                                                                   //Serial.println(n);                                 
                                                                           }
                                                                           else {
                                                                                   Serial.println(F("Errore nella chiusura della scrittura del file"));
                                                                                    return (0); 
                                                                                    file.close();                
                                                                          }
                                                                 }                               
                                            }
                                            else{
                                                  Serial.println(F("Errore ............................"));
                                            }
                                            file.close();
                                            if(temp>0){
                                                        Serial.println(F("VEN.TXT"));
                                                        n=pin;
                                                        if (file.open("VEN.TXT", O_READ | O_WRITE)){                        
                                 			       for(int f=0; f<temp; f++) {
                                                                          //Serial.print("Posizione cursore file ciclo   :");
                                                                          //Serial.print(f);
                                                                          //Serial.print("      ");
                                                                          //Serial.println(n);   
                                                                          file.seekSet(n);
                                                                          if(file.write("01")){
                                                                                      n=file.curPosition()+168;
                                                                                      //Serial.print("OK ..................................");
                                                                                      //Serial.println(n);                                 
                                                                           }
                                                                            else {
                                                                                   Serial.println(F("Errore nella chiusura della scrittura del file"));
                                                                                    return (0); 
                                                                                    file.close();                
                                                                             }
                                                                 }                               
                                                        }
                                                        else{
                                                             Serial.println(F("Errore ............................"));                                                              
                                                              file.close();
                                                              return (0);
                                                        }
                                            }
                                            file.close();
                                            break;
                                    }
                                    case 1:{
                                             Serial.println(F("VEN.TXT"));      //Serial.println(weekday[i]);
                                             if (file.open("VEN.TXT", O_READ | O_WRITE)){                        
                                 			       for(int f=0; f<durata; f++) {
                                                                          //Serial.print("Posizione cursore file ciclo   :");
                                                                          //Serial.print(f);
                                                                          //Serial.print("      ");
                                                                          //Serial.println(n);   
                                                                          file.seekSet(n);
                                                                          if(file.write("01")){
                                                                                      n=file.curPosition()+168;
                                                                                      //Serial.print("OK ..................................");
                                                                                      //Serial.println(n);                                 
                                                                           }
                                                                           else {
                                                                                   Serial.println(F("Errore nella chiusura della scrittura del file"));
                                                                                    return (0); 
                                                                                    file.close();                
                                                                          }
                                                                 }                               
                                            }
                                            else{
                                                  Serial.println(F("Errore ............................"));
                                            }
                                            file.close();
                                            if(temp>0){
                                                        Serial.println(F("SAB.TXT"));
                                                        n=pin;
                                                        if (file.open("SAB.TXT", O_READ | O_WRITE)){                        
                                 			       for(int f=0; f<temp; f++) {
                                                                          //Serial.print("Posizione cursore file ciclo   :");
                                                                          //Serial.print(f);
                                                                          //Serial.print("      ");
                                                                          //Serial.println(n);   
                                                                          file.seekSet(n);
                                                                          if(file.write("01")){
                                                                                      n=file.curPosition()+168;
                                                                                      //Serial.print("OK ..................................");
                                                                                      //Serial.println(n);                                 
                                                                           }
                                                                            else {
                                                                                   Serial.println(F("Errore nella chiusura della scrittura del file"));
                                                                                    return (0); 
                                                                                    file.close();                
                                                                             }
                                                                 }                               
                                                        }
                                                        else{
                                                             Serial.println(F("Errore ............................"));                                                              
                                                              file.close();
                                                              return (0);
                                                        }
                                            }
                                            file.close();
                                            break;
                                    }
                                     case 2:{
                                             Serial.println(F("SAB.TXT"));      //Serial.println(weekday[i]);
                                             if (file.open("SAB.TXT", O_READ | O_WRITE)){                        
                                 			       for(int f=0; f<durata; f++) {
                                                                          //Serial.print("Posizione cursore file ciclo   :");
                                                                          //Serial.print(f);
                                                                          //Serial.print("      ");
                                                                          //Serial.println(n);   
                                                                          file.seekSet(n);
                                                                          if(file.write("01")){
                                                                                      n=file.curPosition()+168;
                                                                                      //Serial.print("OK ..................................");
                                                                                      //Serial.println(n);                                 
                                                                           }
                                                                           else {
                                                                                   Serial.println(F("Errore nella chiusura della scrittura del file"));
                                                                                    return (0); 
                                                                                    file.close();                
                                                                          }
                                                                 }                               
                                            }
                                            else{
                                                  Serial.println(F("Errore ............................"));
                                            }
                                            file.close();
                                            if(temp>0){
                                                        Serial.println(F("DOM.TXT"));
                                                        n=pin;
                                                        if (file.open("DOM.TXT", O_READ | O_WRITE)){                        
                                 			       for(int f=0; f<temp; f++) {
                                                                          //Serial.print("Posizione cursore file ciclo   :");
                                                                          //Serial.print(f);
                                                                          //Serial.print("      ");
                                                                          //Serial.println(n);   
                                                                          file.seekSet(n);
                                                                          if(file.write("01")){
                                                                                      n=file.curPosition()+168;
                                                                                      //Serial.print("OK ..................................");
                                                                                      //Serial.println(n);                                 
                                                                           }
                                                                            else {
                                                                                   Serial.println(F("Errore nella chiusura della scrittura del file"));
                                                                                    return (0); 
                                                                                    file.close();                
                                                                             }
                                                                 }                               
                                                        }
                                                        else{
                                                             Serial.println(F("Errore ............................"));                                                              
                                                              file.close();
                                                              return (0);
                                                        }
                                            }
                                            file.close();
                                            break;
                                    }
                                     case 3:{
                                             Serial.println(F("DOM.TXT"));      //Serial.println(weekday[i]);
                                             if (file.open("DOM.TXT", O_READ | O_WRITE)){                        
                                 			       for(int f=0; f<durata; f++) {
                                                                          //Serial.print("Posizione cursore file ciclo   :");
                                                                          //Serial.print(f);
                                                                          //Serial.print("      ");
                                                                          //Serial.println(n);   
                                                                          file.seekSet(n);
                                                                          if(file.write("01")){
                                                                                      n=file.curPosition()+168;
                                                                                      //Serial.print("OK ..................................");
                                                                                      //Serial.println(n);                                 
                                                                           }
                                                                           else {
                                                                                   Serial.println(F("Errore nella chiusura della scrittura del file"));
                                                                                    return (0); 
                                                                                    file.close();                
                                                                          }
                                                                 }                               
                                            }
                                            else{
                                                  Serial.println(F("Errore ............................"));
                                            }
                                            file.close();
                                            if(temp>0){
                                                        Serial.println(F("LUN.TXT"));
                                                        n=pin;
                                                        if (file.open("LUN.TXT", O_READ | O_WRITE)){                        
                                 			       for(int f=0; f<temp; f++) {
                                                                          //Serial.print("Posizione cursore file ciclo   :");
                                                                          //Serial.print(f);
                                                                          //Serial.print("      ");
                                                                          //Serial.println(n);   
                                                                          file.seekSet(n);
                                                                          if(file.write("01")){
                                                                                      n=file.curPosition()+168;
                                                                                      //Serial.print("OK ..................................");
                                                                                      //Serial.println(n);                                 
                                                                           }
                                                                            else {
                                                                                   Serial.println(F("Errore nella chiusura della scrittura del file"));
                                                                                    return (0); 
                                                                                    file.close();                
                                                                             }
                                                                 }                               
                                                        }
                                                        else{
                                                             Serial.println(F("Errore ............................"));                                                              
                                                              file.close();
                                                              return (0);
                                                        }
                                            }
                                            file.close();
                                            break;
                                    }
                                     case 4:{
                                             Serial.println(F("LUN.TXT"));      //Serial.println(weekday[i]);
                                             if (file.open("LUN.TXT", O_READ | O_WRITE)){                        
                                 			       for(int f=0; f<durata; f++) {
                                                                          //Serial.print("Posizione cursore file ciclo   :");
                                                                          //Serial.print(f);
                                                                          //Serial.print("      ");
                                                                          //Serial.println(n);   
                                                                          file.seekSet(n);
                                                                          if(file.write("01")){
                                                                                      n=file.curPosition()+168;
                                                                                      //Serial.print("OK ..................................");
                                                                                      //Serial.println(n);                                 
                                                                           }
                                                                           else {
                                                                                   Serial.println(F("Errore nella chiusura della scrittura del file"));
                                                                                    return (0); 
                                                                                    file.close();                
                                                                          }
                                                                 }                               
                                            }
                                            else{
                                                  Serial.println(F("Errore ............................"));
                                            }
                                            file.close();
                                            if(temp>0){
                                                        Serial.println(F("MAR.TXT"));
                                                        n=pin;
                                                        if (file.open("MAR.TXT", O_READ | O_WRITE)){                        
                                 			       for(int f=0; f<temp; f++) {
                                                                          //Serial.print("Posizione cursore file ciclo   :");
                                                                          //Serial.print(f);
                                                                          //Serial.print("      ");
                                                                          //Serial.println(n);   
                                                                          file.seekSet(n);
                                                                          if(file.write("01")){
                                                                                      n=file.curPosition()+168;
                                                                                      //Serial.print("OK ..................................");
                                                                                      //Serial.println(n);                                 
                                                                           }
                                                                            else {
                                                                                   Serial.println(F("Errore nella chiusura della scrittura del file"));
                                                                                    return (0); 
                                                                                    file.close();                
                                                                             }
                                                                 }                               
                                                        }
                                                        else{
                                                             Serial.println(F("Errore ............................"));                                                              
                                                              file.close();
                                                              return (0);
                                                        }
                                            }
                                            file.close();
                                            break;
                                    } 
                                    case 5:{
                                             Serial.println(F("MAR.TXT"));      //Serial.println(weekday[i]);
                                             if (file.open("MAR.TXT", O_READ | O_WRITE)){                        
                                 			       for(int f=0; f<durata; f++) {
                                                                          //Serial.print("Posizione cursore file ciclo   :");
                                                                          //Serial.print(f);
                                                                          //Serial.print("      ");
                                                                          //Serial.println(n);   
                                                                          file.seekSet(n);
                                                                          if(file.write("01")){
                                                                                      n=file.curPosition()+168;
                                                                                      //Serial.print("OK ..................................");
                                                                                      //Serial.println(n);                                 
                                                                           }
                                                                           else {
                                                                                   Serial.println(F("Errore nella chiusura della scrittura del file"));
                                                                                    return (0); 
                                                                                    file.close();                
                                                                          }
                                                                 }                               
                                            }
                                            else{
                                                  Serial.println(F("Errore ............................"));
                                            }
                                            file.close();
                                            if(temp>0){
                                                        Serial.println(F("MER.TXT"));
                                                        Serial.println(temp);
                                                        n=pin;
                                                        if (file.open("MER.TXT", O_READ | O_WRITE)){                        
                                 			       for(int f=0; f<temp; f++) {
                                                                          //Serial.print("Posizione cursore file ciclo   :");
                                                                          //Serial.print(f);
                                                                          //Serial.print("      ");
                                                                          //Serial.println(n);   
                                                                          file.seekSet(n);
                                                                          if(file.write("01")){
                                                                                      n=file.curPosition()+168;
                                                                                      //Serial.print("OK ..................................");
                                                                                      //Serial.println(n);                                 
                                                                           }
                                                                            else {
                                                                                   Serial.println(F("Errore nella chiusura della scrittura del file"));
                                                                                    return (0); 
                                                                                    file.close();                
                                                                             }
                                                                 }                               
                                                        }
                                                        else{
                                                             Serial.println(F("Errore ............................"));                                                              
                                                              file.close();
                                                              return (0);
                                                        }
                                            }
                                            file.close();
                                            break;
                                     }
                                     case 6:{
                                             Serial.println(F("MER.TXT"));      //Serial.println(weekday[i]);
                                             if (file.open("MER.TXT", O_READ | O_WRITE)){                        
                                 			       for(int f=0; f<durata; f++) {
                                                                          Serial.print("Posizione cursore file ciclo   :");
                                                                          Serial.print(f);
                                                                          Serial.print("      ");
                                                                          Serial.println(n);   
                                                                          file.seekSet(n);
                                                                          if(file.write("01")){
                                                                                      n=file.curPosition()+168;
                                                                                      //Serial.print("OK ..................................");
                                                                                      //Serial.println(n);                                 
                                                                           }
                                                                           else {
                                                                                   Serial.println(F("Errore nella chiusura della scrittura del file"));
                                                                                    return (0); 
                                                                                    file.close();                
                                                                          }
                                                                 }                               
                                            }
                                            else{
                                                  Serial.println(F("Errore ............................"));
                                            }
                                            file.close();
                                            if(temp>0){
                                                        Serial.println(F("GIO.TXT"));
                                                        n=pin;
                                                        if (file.open("GIO.TXT", O_READ | O_WRITE)){                        
                                 			       for(int f=0; f<temp; f++) {
                                                                          //Serial.print("Posizione cursore file ciclo   :");
                                                                          //Serial.print(f);
                                                                          //Serial.print("      ");
                                                                          //Serial.println(n);   
                                                                          file.seekSet(n);
                                                                          if(file.write("01")){
                                                                                      n=file.curPosition()+168;
                                                                                      //Serial.print("OK ..................................");
                                                                                      //Serial.println(n);                                 
                                                                           }
                                                                            else {
                                                                                   Serial.println(F("Errore nella chiusura della scrittura del file"));
                                                                                    return (0); 
                                                                                    file.close();                
                                                                             }
                                                                 }                               
                                                        }
                                                        else{
                                                             Serial.println(F("Errore ............................"));                                                              
                                                              file.close();
                                                              return (0);
                                                        }
                                            }
                                            file.close();
                                            break;
                                    }                                    
                                    default:{
                                       Serial.println(F("Errore"));
                                   }
                                }
               
                     }                
          }
          return(1);
}


boolean readfiletime(SdFat sd1,unsigned long n, byte giornosett, int short pin[], int short pinrw[]){
  Serial.println(F("Lettura dei pin settaggi da file"));
  //char weekday[7][4] = {"GIO","VEN","SAB","DOM","LUN","MAR","MER"};
  char buffer[162];
  SdFile file;
  sd1.chdir(1);
  //giornosett=0; //da eliminare settato temporaneamente per accedere allo switch
  Serial.print(F("Giorno della settimana:      "));
  //Serial.println(giornosett);
  //for (int j=0; j<3; j++)  Serial.print(weekday[giornosett][j]);  
//  Serial.println();
  switch (giornosett) {  
    
                        case 0:{
                                Serial.println(F("GIO.TXT"));      //Serial.println(weekday[i]);
                                if (file.open("GIO.TXT", O_READ | O_WRITE)){                   
                                                 file.seekSet(n);
                                                 if(file.read(buffer,162)){ //da rivedere
                                                              Serial.print(F("lettura OK .................................."));
//                                                              Serial.println(n);
//                                                              for(int i=0; i<162; i++){  
//                                                                                Serial.print(buffer[i]);
//                                                              }
                                                              Serial.println();
                                                              for(int j=0, i=0; i<162; j++, i=i+3 ){
//                                                                                     Serial.print(j); 
//                                                                                     Serial.print("    :      ");
//                                                                                     Serial.print(i);
//                                                                                     Serial.print("      ");
//                                                                                     Serial.println("assegnazione");
                                                                                     //pin[j]=((((int)buffer[i]-'0')*10)+((int)buffer[i+1]-'0'));
                                                                                     pin[j]=((int)buffer[i+1]-'0');
                                                                                     pinrw[j]=((int)buffer[i]-'0');
                                                                                                                                           
                                                               } 
//                                                             Serial.println();                               
                                                  }                                                                      
                                                  else {
                                                               Serial.println(F("Errore nella lettura del file"));                                                                                    
                                                               file.close(); 
                                                               return (0);               
                                                  } 
                                                  file.close();                                                      
                                 }
                                 else{
                                     Serial.println(F("Errore apertura file in  Funzione readfiletime()   " ));
                                 }                                 
                                 break;
                          } 
                          case 1:{
                                  Serial.println(F("VEN.TXT"));      //Serial.println(weekday[i]);
                                  if (file.open("VEN.TXT", O_READ | O_WRITE)){                   
                                                 file.seekSet(n);
                                                 if(file.read(buffer,162)){ //da rivedere
                                                              Serial.print(F("lettura OK .................................."));
//                                                              Serial.println(n);
//                                                              for(int i=0; i<162; i++){  
//                                                                                Serial.print(buffer[i]);
//                                                              }
                                                              Serial.println();                                                              
                                                              for(int j=0, i=0; i<162; j++, i=i+3 ){
//                                                                                     Serial.print(j); 
//                                                                                     Serial.print("    :      ");
//                                                                                     Serial.print(i);
//                                                                                     Serial.print("      ");
//                                                                                     Serial.println("assegnazione");
                                                                                   //pin[j]=((((int)buffer[i]-'0')*10)+((int)buffer[i+1]-'0'));
                                                                                     pin[j]=((int)buffer[i+1]-'0');
                                                                                     pinrw[j]=((int)buffer[i]-'0');                                                    
                                                               } 
//                                                             Serial.println();                               
                                                  }                                                                      
                                                  else {
                                                               Serial.println(F("Errore nella lettura del file"));                                                                                    
                                                               file.close(); 
                                                               return (0);               
                                                  } 
                                                  file.close();                                                      
                                  }
                                  else{
                                     Serial.println(F("Errore apertura file in  Funzione readfiletime()   " ));
                                  }                                 
                                  break;
                           }
                           case 2:{
                                   Serial.println(F("SAB.TXT"));      //Serial.println(weekday[i]);
                                   if (file.open("SAB.TXT", O_READ | O_WRITE)){                   
                                                 file.seekSet(n);
                                                 if(file.read(buffer,162)){ //da rivedere
                                                              Serial.print(F("lettura OK .................................."));
//                                                              Serial.println(n);
//                                                              for(int i=0; i<162; i++){  
//                                                                                Serial.print(buffer[i]);
//                                                              }
                                                              Serial.println();
                                                              for(int j=0, i=0; i<162; j++, i=i+3 ){
//                                                                                     Serial.print(j); 
//                                                                                     Serial.print("    :      ");
//                                                                                     Serial.print(i);
//                                                                                     Serial.print("      ");
//                                                                                     Serial.println("assegnazione");
                                                                                   //pin[j]=((((int)buffer[i]-'0')*10)+((int)buffer[i+1]-'0'));
                                                                                     pin[j]=((int)buffer[i+1]-'0');
                                                                                     pinrw[j]=((int)buffer[i]-'0');                                                    
                                                               } 
//                                                             Serial.println();                               
                                                  }                                                                      
                                                  else {
                                                               Serial.println(F("Errore nella lettura del file"));                                                                                    
                                                               file.close(); 
                                                               return (0);               
                                                  } 
                                                  file.close();                                                      
                                 }
                                 else{
                                     Serial.println(F("Errore apertura file in  Funzione readfiletime()   " ));
                                 }
                                 
                                 break;
                           }
                           case 3:{
                                   Serial.println(F("DOM.TXT"));      //Serial.println(weekday[i]);
                                   if (file.open("DOM.TXT", O_READ | O_WRITE)){                   
                                                 file.seekSet(n);
                                                 if(file.read(buffer,162)){ //da rivedere
                                                              Serial.print(F("lettura OK .................................."));
//                                                              Serial.println(n);
//                                                              for(int i=0; i<162; i++){  
//                                                                                Serial.print(buffer[i]);
//                                                              }
                                                              Serial.println();
                                                              for(int j=0, i=0; i<162; j++, i=i+3 ){
//                                                                                     Serial.print(j); 
//                                                                                     Serial.print("    :      ");
//                                                                                     Serial.print(i);
//                                                                                     Serial.print("      ");
//                                                                                     Serial.println("assegnazione");
                                                                                   //pin[j]=((((int)buffer[i]-'0')*10)+((int)buffer[i+1]-'0'));
                                                                                     pin[j]=((int)buffer[i+1]-'0');
                                                                                     pinrw[j]=((int)buffer[i]-'0');                                                    
                                                              } 
//                                                             Serial.println();                               
                                                  }                                                                      
                                                  else {
                                                               Serial.println(F("Errore nella lettura del file"));                                                                                    
                                                               file.close(); 
                                                               return (0);               
                                                  } 
                                                  file.close();                                                      
                                 }
                                 else{
                                     Serial.println(F("Errore apertura file in  Funzione readfiletime()   " ));
                                 }                                 
                                 break;
                           }
                           case 4:{
                                Serial.println(F("LUN.TXT"));      //Serial.println(weekday[i]);
                                if (file.open("LUN.TXT", O_READ | O_WRITE)){                   
                                                 file.seekSet(n);
                                                 if(file.read(buffer,162)){ //da rivedere
                                                              Serial.print(F("lettura OK .................................."));
//                                                              Serial.println(n);
//                                                              for(int i=0; i<162; i++){  
//                                                                                Serial.print(buffer[i]);
//                                                              }
                                                              Serial.println();
                                                              for(int j=0, i=0; i<162; j++, i=i+3 ){
//                                                                                     Serial.print(j); 
//                                                                                     Serial.print("    :      ");
//                                                                                     Serial.print(i);
//                                                                                     Serial.print("      ");
//                                                                                     Serial.println("assegnazione");
                                                                                   //pin[j]=((((int)buffer[i]-'0')*10)+((int)buffer[i+1]-'0'));
                                                                                     pin[j]=((int)buffer[i+1]-'0');
                                                                                     pinrw[j]=((int)buffer[i]-'0');                                                   
                                                              } 
//                                                             Serial.println();                               
                                                  }                                                                      
                                                  else {
                                                               Serial.println(F("Errore nella lettura del file"));                                                                                    
                                                               file.close(); 
                                                               return (0);               
                                                  } 
                                                  file.close();                                                      
                                 }
                                 else{
                                     Serial.println(F("Errore apertura file in  Funzione readfiletime()   " ));
                                 }                                 
                                 break;
                           }
                           case 5:{
                                   Serial.println(F("MAR.TXT"));      //Serial.println(weekday[i]);
                                   if (file.open("MAR.TXT", O_READ | O_WRITE)){                   
                                                 file.seekSet(n);
                                                 if(file.read(buffer,162)){ //da rivedere
                                                              Serial.print(F("lettura OK .................................."));
//                                                              Serial.println(n);
//                                                              for(int i=0; i<162; i++){  
//                                                                                Serial.print(buffer[i]);
//                                                              }
                                                              Serial.println();
                                                              for(int j=0, i=0; i<162; j++, i=i+3 ){
//                                                                                     Serial.print(j); 
//                                                                                     Serial.print("    :      ");
//                                                                                     Serial.print(i);
//                                                                                     Serial.print("      ");
//                                                                                     Serial.println("assegnazione");
                                                                                   //pin[j]=((((int)buffer[i]-'0')*10)+((int)buffer[i+1]-'0'));
                                                                                     pin[j]=((int)buffer[i+1]-'0');
                                                                                     pinrw[j]=((int)buffer[i]-'0');                                                    
                                                              } 
//                                                             Serial.println();                               
                                                  }                                                                      
                                                  else {
                                                               Serial.println(F("Errore nella lettura del file"));                                                                                    
                                                               file.close(); 
                                                               return (0);               
                                                  } 
                                                  file.close();                                                      
                                 }
                                 else{
                                     Serial.println(F("Errore apertura file in  Funzione readfiletime()   " ));
                                 }                                 
                                 break;
                           }
                           case 6:{
                                   Serial.println(F("MER.TXT"));      //Serial.println(weekday[i]);
                                   if (file.open("MER.TXT", O_READ | O_WRITE)){                   
                                                 file.seekSet(n);
                                                 if(file.read(buffer,162)){ //da rivedere
                                                              Serial.print(F("lettura OK .................................."));
//                                                              Serial.println(n);
//                                                              for(int i=0; i<162; i++){  
//                                                                                Serial.print(buffer[i]);
//                                                              }
                                                              Serial.println();
                                                              for(int j=0, i=0; i<162; j++, i=i+3 ){
//                                                                                     Serial.print(j); 
//                                                                                     Serial.print("    :      ");
//                                                                                     Serial.print(i);
//                                                                                     Serial.print("      ");
//                                                                                     Serial.print("assegnazione :  ");
//                                                                                     Serial.print(buffer[i]);
//                                                                                     Serial.println(buffer[i+1]);
                                                                                   //pin[j]=((((int)buffer[i]-'0')*10)+((int)buffer[i+1]-'0'));
                                                                                     pin[j]=((int)buffer[i+1]-'0');
                                                                                     pinrw[j]=((int)buffer[i]-'0');                                                    
                                                               } 
//                                                             Serial.println();                               
                                                  }                                                                      
                                                  else {
                                                               Serial.println(F("Errore nella lettura del file"));                                                                                    
                                                               file.close(); 
                                                               return (0);               
                                                  } 
                                                  file.close();                                                      
                                 }
                                 else{
                                     Serial.println(F("Errore apertura file in  Funzione readfiletime()   " ));
                                 }
                                 
                                 break;
                           }
                           default:{
                                   Serial.println(F("Caso DEFAULT dentro alla Funzione readfiletime()"));          
                           } 
 }                                         
//  Serial.println(F("pin configuarti dopo lettura del file:  "));
//  for(int i=0; i<54; i++){ 
//    
//   Serial.print(i); 
//   Serial.print(":      ");          
//   Serial.println(pin[i]);
//  }
// Serial.println();
// Serial.println();
// Serial.println();
// Serial.println();
 return(1);
}








void inviaMagicPacket(){
    //Porta UDP locale
    int portaLocale = 7; 
    //Indirizzo broadcast della rete locale
    byte broadCastIp[] = { 192, 168, 1, 255 };
    //Mac address della scheda lan del pc da avviare
    byte remote_MAC_ADD[] = { 0x00, 0x13, 0x8F, 0x11, 0x3D, 0x1B };
    //Porta UDP WOL
    int wolPort = 9;  
    Udp.begin(portaLocale);
    //definisco un array da 102 byte
    byte magicPacket[102];
    //variabili per i cicli
    int Ciclo = 0, CicloMacAdd = 0, IndiceArray = 0;
 
    for( Ciclo = 0; Ciclo < 6; Ciclo++)
    {
    //i primi 6 byte dell'array sono settati al valore 0xFF
    magicPacket[IndiceArray] = 0xFF;
    //incremento l'indice dell'array
    IndiceArray++;
   }
 
  //eseguo 16 cicli per memorizzare il mac address del pc
  //da avviare
  for( Ciclo = 0; Ciclo < 16; Ciclo++ )
  {
    //eseguo un ciclo per memorizzare i 6 byte del
    //mac address
    for( CicloMacAdd = 0; CicloMacAdd < 6; CicloMacAdd++)
    {
      magicPacket[IndiceArray] = remote_MAC_ADD[CicloMacAdd];
      //incremento l'indice dell'array
      IndiceArray++;
    }
  }
 
  //spedisco il magic packet in brodcast sulla porta prescelta
  Udp.beginPacket(broadCastIp, wolPort);
  Udp.write(magicPacket, sizeof magicPacket);
  Udp.endPacket();
 
}





boolean configurazionetcpip(SdFat sd1, String namefile, byte mac2[], byte ip2[], byte gateway2[], byte dns2[], byte subnet2[]){
    
     Serial.println(F("\nConfigurazione Tcp/ip"));
     SdFile file;
     sd1.chdir(1);
     int c;
     char cnamefile[13];
     namefile.trim();
     namefile.toCharArray(cnamefile, 13);
     
     if(sd1.exists(cnamefile)){
                 Serial.println(F("file eth.tcp esiste"));            
                 Serial.println(F("Lettura dei parametri da configurare"));    
                 if(file.open(cnamefile, O_READ)){ //sd1.vwd(),   O_WRITE | O_APPEND

                                   byte pot[3]={'0','0','0'};
                                   int ciclo=0; 
                                   int i=0; 
                                   int ind=0;
                                   Serial.print(F("macaddress------------------------------------:   "));
                                   while ((c = file.read()) >= 0 && c!=10 && ciclo<6){          ////       10 = /n    44 = ,   61 = =   59 = ;                                      
                                          if(c>64 && c<71){
                                            pot[i]=c - 'A' + 10;
                                            i++;
                                          }
                                          else if(c>96 && c<103){
                                            pot[i]=c - 'a' + 10;                                            
                                            i++;
                                          }
                                          else if(c>47 && c<58){
                                            pot[i]=c - '0';                                         
                                            i++;
                                          }
                                          else{
                                              i=0;
                                          }
                                          if(c==44 || c==59){                                  
                                             ind=(pot[0])*16+(pot[1]);
                                             mac2[ciclo]=ind;
                                             //Serial.print("mac  :");
                                             Serial.print(mac2[ciclo],HEX);
                                             Serial.print(F(":"));
                                             ciclo++;
                                             pot[0]=pot[1]=pot[2]='0';
                                          }
                                   }
//                                   for(int g=0; g<6; g++){
//                                   Serial.print(mac2[g],HEX);
//                                   Serial.print("    ");
//                                   Serial.print(mac2[g]);
//                                   }
                                   Serial.println();
 
                                   Serial.print(F("ip------------------------------------:"));
                                   pot[0]=pot[1]=pot[2]='0';
                                   ciclo=i=ind=0;                  
                                   while ((c = file.read()) >= 0 && c!=10 &&  ciclo<4){
                                     
                                                 if(c==44 || c==59){                                                        
                                                       if(i==1) {
                                                                 pot[2]=pot[0];
                                                                 pot[0]='0';
                                                       }
                                                       if(i==2){
                                                                 pot[2]=pot[1];
                                                                 pot[1]=pot[0];
                                                                 pot[0]='0';
                                                       }
                                                       ind =((int)pot[0]-'0')*100+((int)pot[1]-'0')*10+((int)pot[2]-'0');
                                                       ip2[ciclo]=ind; 
                                                       Serial.print(ip2[ciclo]);
                                                       Serial.print(".");
                                                       ciclo++;
                                                       i=0; 
                                                       pot[0]=pot[1]=pot[2]='0';                                                      

                                                 }
                                                 if((c>47 && c<58)){ //i numeri da 1 a 9
                                                              pot[i]=c;
                                                              i++;                                                                                                                               
                                                 }
                                   }                                
                                   Serial.println();
                                   
                                   Serial.print(F("subnet------------------------------------:"));
                                   pot[0]=pot[1]=pot[2]='0';
                                   i=ind=ciclo=0;                  
                                   while ((c = file.read()) >= 0 && c!=10 &&  ciclo<4){
                                     
                                                 if(c==44 || c==59){                                                        
                                                       if(i==1) {
                                                                 pot[2]=pot[0];
                                                                 pot[0]='0';
                                                       }
                                                       if(i==2){
                                                                 pot[2]=pot[1];
                                                                 pot[1]=pot[0];
                                                                 pot[0]='0';
                                                       }
                                                       ind =((int)pot[0]-'0')*100+((int)pot[1]-'0')*10+((int)pot[2]-'0');
                                                       subnet2[ciclo]=ind; 
                                                       Serial.print(subnet2[ciclo]);
                                                       Serial.print(F("."));
                                                       ciclo++;
                                                       i=0; 
                                                       pot[0]=pot[1]=pot[2]='0';                                                      

                                                 }
                                                 if((c>47 && c<58)){ //i numeri da 1 a 9
                                                              pot[i]=c;
                                                              i++;                                                                                                                               
                                                 }
                                   }                                
                                   Serial.println();
                                    
                                   Serial.print(F("gateway------------------------------------:"));
                                   pot[0]=pot[1]=pot[2]='0';
                                   i=ind=ciclo=0;                  
                                   while ((c = file.read()) >= 0 && c!=10 &&  ciclo<4){
                                     
                                                 if(c==44 || c==59){                                                        
                                                       if(i==1) {
                                                                 pot[2]=pot[0];
                                                                 pot[0]='0';
                                                       }
                                                       if(i==2){
                                                                 pot[2]=pot[1];
                                                                 pot[1]=pot[0];
                                                                 pot[0]='0';
                                                       }
                                                       ind =((int)pot[0]-'0')*100+((int)pot[1]-'0')*10+((int)pot[2]-'0');
                                                       gateway2[ciclo]=ind; 
                                                       Serial.print(gateway2[ciclo]);
                                                       Serial.print(".");
                                                       ciclo++;
                                                       i=0; 
                                                       pot[0]=pot[1]=pot[2]='0';
                                                     }
                                                 if((c>47 && c<58)){ //i numeri da 1 a 9
                                                              pot[i]=c;
                                                              i++;                                                                                                                               
                                                 }
                                   }
                                   Serial.println();
                                    
                                   Serial.print(F("dns------------------------------------:"));
                                   pot[0]=pot[1]=pot[2]='0';
                                   i=ind=ciclo=0;                  
                                   while ((c = file.read()) >= 0 && c!=10 &&  ciclo<4){
                                     
                                                 if(c==44 || c==59){                                                        
                                                       if(i==1) {
                                                                 pot[2]=pot[0];
                                                                 pot[0]='0';
                                                       }
                                                       if(i==2){
                                                                 pot[2]=pot[1];
                                                                 pot[1]=pot[0];
                                                                 pot[0]='0';
                                                       }
                                                       ind =((int)pot[0]-'0')*100+((int)pot[1]-'0')*10+((int)pot[2]-'0');
                                                       dns2[ciclo]=ind; 
                                                       Serial.print(dns2[ciclo]);
                                                       Serial.print(F("."));
                                                       ciclo++;
                                                       i=0; 
                                                       pot[0]=pot[1]=pot[2]='0';                                                      

                                                 }
                                                 if((c>47 && c<58)){ //i numeri da 1 a 9
                                                              pot[i]=c;
                                                              i++;                                                                                                                               
                                                 }
                                    }
                                    Serial.println();
                                    
                                    file.close();
                                    return 1;
                  }
                  else return 0; //errore apertura file
     }
     else{  
                  Serial.println(F("file eth.tcp non esiste"));
                  if(file.createContiguous(sd1.vwd(),cnamefile,1)){
                         Serial.println(F("Creazione del file eth.tcp della configurazione tcp/ip con i parametri di default"));
                         String temp1="";                                
                         for(int f=0; f<6; f++){
                                          Serial.print(String(mac2[f], HEX));
                                          temp1+="0x";
                                          temp1+=(String(mac2[f], HEX));
                                          if(f<5)   temp1+=",";                                                         
                                          else temp1+=";\n";                                      
                         }    
                         Serial.println(temp1);
                         file.print(temp1);
                         temp1=""; 
                         for(int f=0; f<4; f++){  
                                           temp1+=ip2[f];
                                           if(f<3)  temp1+=",";                                 
                                           else   temp1+=";\n";                                      
                         }   
                         Serial.println(temp1);
                         file.print(temp1);
                         temp1=""; 
                         for(int f=0; f<4; f++){ 
                                           temp1+=subnet2[f];
                                           if(f<3) temp1+=",";
                                           else  temp1+=";\n";  
                         }
                         Serial.println(temp1);
                         file.print(temp1);
                         temp1=""; 
                         for(int f=0; f<4; f++){
                                           temp1+=gateway2[f];
                                           if(f<3) temp1+=",";                                
                                           else   temp1+=";\n";                                       
                         }   
                         Serial.println(temp1);
                         file.print(temp1);
                         temp1=""; 
                         for(int f=0; f<4; f++){
                                           temp1+=dns2[f];
                                           if(f<3) temp1+=",";                                 
                                           else temp1+=";\n";                                       
                         }   
                         Serial.println(temp1);
                         file.print(temp1);
                         file.close();
                         return 1;               
                   } 
                   else{
                         Serial.println(F("Errore nella creazione del file eth.tcp  della configurazione tcp/ip"));                  
                         return 0;
                   }     
   }
}




//char* ricercautente(SdFat sd1, String namefile,String user, String password, char line3[13])
//ricercautente(sd, "Login.pwd",user, msg1, paginapersonale);

void reboot(){
void(* resetFunc) (void) = 0; //declare reset function @ address 0
resetFunc();  //call reset
}

String paratcp(byte a[], String alfa, String msgw, String msgt, int* s, int* j, int x){
               int ind;
               int t=1;
               for(int f=0; f<4  ; f++ ){  
                            if(msgt==(alfa+f+"=")){
                                   ind=0;                              
                                   *j=*j+*s;
                                   msgt=readurl(msgw.substring(*j,x), t, s, 9);          
                                   t=1;                                                        
                                   String prova =msgt.substring(0,3);                                                                                                                                                     
                                   char pot[3];
                                   prova.toCharArray(pot,4);
                                   if(pot[1]=='&'){                                                                                                           
                                         pot[2]=pot[0];
                                         pot[1]=pot[0]='0';
                                   }            
                                   if(pot[2]=='&'){                                                                                                           
                                         pot[2]=pot[1];
                                         pot[1]=pot[0];
                                         pot[0]='0';
                                   }
                                   ind =((int)pot[0]-'0')*100+((int)pot[1]-'0')*10+((int)pot[2]-'0');
                                   Serial.print(ind);
                                   a[f]=ind;
                                   if(f<3) Serial.print(F(","));
                                   else Serial.print(F(";"));
                              }
                              *j=*j+*s;
                              msgt=readurl(msgw.substring(*j,x), t, s, 9);           //passa la stringa letta dopo il ? con i dati inviati con il metodo post finche non trova il pulsante invio etichettato con salva+ lo spazio                                                                     
                              t=1; 
                 }  
                 Serial.print(F("\n"));
                 return(msgt);
}



String paramac(byte a[], String alfa, String msgw, String msgt, int* s, int* j, int x){  // msg1=paratcp(ip, setstr, msg,msg1, &q, &i, x);
      int ind,z;
      int t=1;
      for(int f=0; f<6; f++ ){  
                   if(msgt==(alfa+f+"=")){ 
                                    ind=0;                     
                                    *j=*j+*s;
                                    msgt=readurl(msgw.substring(*j,x), t, s, 9); //msgt=readurl(msgw.substring(*j,x), t, s, 9);         
                                    t=1;                                                        
                                    String prova =msgt.substring(0,2);                                                                                                                                                     
                                    char pot[3];
                                    prova.toCharArray(pot,3);
                                    Serial.print(prova);
                                    Serial.print(F("Valore array :"));
                                    Serial.print(pot[0]);
                                    Serial.print(F("......"));
                                    Serial.println(pot[1]);
                                    z=0;
                                    do{  
                                       if((int)pot[z]>64 && (int)pot[z]<71){
                                             pot[z]=pot[z]-'A' + 10;
                                       }
                                       else if((int)pot[z]>96 && (int)pot[z]<103){
                                             pot[z]=pot[z]-'a' + 10;         
                                       }
                                       else{
                                             pot[z]=pot[z]-'0';
                                       }
                                       z++;
                                    }while(z!=2);
 
                                    ind=((int)pot[0])*16+((int)pot[1]);
                                    Serial.print("=");
                                    Serial.println(ind);
                                    a[f]=ind;           
                       }
                       *j=*j+*s;
                       msgt=readurl(msgw.substring(*j,x), t, s, 9);           //passa la stringa letta dopo il ? con i dati inviati con il metodo post finche non trova il pulsante invio etichettato con salva+ lo spazio                                                                     
                       t=1;
    }
    Serial.print(F("\n"));
    return(msgt);               
}



String parausr(SdFat sd1, String alfa, String msgw, String msgt, int* s, int* j, int x){
            Serial.println(F("          Funzione  parausr                       "));
            SdFile file;
            int t=1,f=0, p=0;
            //setstr="cmp";
            if(sd1.remove("LOGIN.PWD")){
            		Serial.println(F("file LOGIN.PWD è stato cancellato per ricrearlo con le modifiche"));
            		if(file.createContiguous(sd1.vwd(),"LOGIN.PWD",1)){
            		  do{  
                       	     if(msgt==(alfa+f+"=")){                              
                                        *j=*j+*s;
                                        p++;
                                        msgt=readurl(msgw.substring(*j,x), t, s, 15);           //passa la stringa letta dopo il ? con i dati inviati con il metodo post finche non trova il pulsante invio etichettato con salva+ lo spazio                                                                     
                                        t=1;
                                        Serial.println(msgt);
                                        Serial.println(F("     -------------------------------------------------------------------------- "));
                                        Serial.println(msgw);
                                        String prova =msgt.substring(0, msgt.length()-1); 
                                        Serial.println(F("     +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ "));                                                                                             
                                        if(p<3) prova+=' ';
                                        else {
                                              prova+='\n';
                                              p=0;
                                        }  
                                        Serial.print(prova);
                                        Serial.println(F("*************************************************************************** "));
                                        f++;                                                                                        
                                        file.print(prova);
                              }                                                                                     
                              *j=*j+*s;
                              msgt=readurl(msgw.substring(*j,x), t, s, 15);           //passa la stringa letta dopo il ? con i dati inviati con il metodo post finche non trova il pulsante invio etichettato con salva+ lo spazio                                                                     
                              t=1;
                              Serial.print(msgt);
                              Serial.println(F("<<<<<<<<<---***************************************************************** "));
                   
                           }while(!msgt.endsWith(" "));//(!msgt.equalsIgnoreCase("Salva"));
                           file.close();
                           return(msgt);
                       }
                       else{ Serial.println(F("Errore nella creazione del file delle password utenti"));
                       return(msgt);
                       reboot();
                       }
		}
		else{
                      Serial.println(F("Errore nella rimozione del file LOGIN.PWD"));
                      return(msgt);
                      reboot();
                }      
} 

String parausragg(String alfa, String msgw, String* msgt, int* s, int* j, int x){
  
                       Serial.print(F("Funzione parausragg"));
                       Serial.print(*msgt);
                       Serial.print(F("   "));
                       Serial.println(alfa);
                       int t=1;
                       String prova="";

                       if(*msgt==(alfa+"=")){                              
                                        *j=*j+*s;
                                        *msgt=readurl(msgw.substring(*j,x), t, s, 15);           //passa la stringa letta dopo il ? con i dati inviati con il metodo post finche non trova il pulsante invio etichettato con salva+ lo spazio                                                                     
                                        t=1;
                                        prova=*msgt;
                                        prova =prova.substring(0, prova.length()-1);
                        }                                                                                     
                        *j=*j+*s;
                        *msgt=readurl(msgw.substring(*j,x), t, s, 15);           //passa la stringa letta dopo il ? con i dati inviati con il metodo post finche non trova il pulsante invio etichettato con salva+ lo spazio                                                                     
                        t=1;
                        return(prova); 
 }
 
 boolean rimoveusr(SdFat sd1 ,String user1 ,String login , String logint){
   
              Serial.println(F("Funzione  rimoveusr "));
              SdFile file, filet;
              sd1.chdir(1);
              user1=user1+' ';
              Serial.print(F("--->"));
              Serial.print(user1);
              Serial.print(F("<---"));
              String Temporaneo="",user="" ,password="" ,homepage="";
              char cnamefile[13],cnamefilet[13],c;      //15 massima lunghezza del nome utente inpostato su readurl(  ,  ,  ,   ,15)
              logint.toCharArray(cnamefilet, 13);
              login.toCharArray(cnamefile, 13);  
              if(sd1.exists(cnamefilet)){
                    Serial.println("Rimozione del file logint.pwd");
                    sd1.remove(cnamefilet);
              }

              if(!filet.createContiguous(sd1.vwd(),cnamefilet,1)){
                   Serial.print(F(" Errore nella creazione del file  "));            
                   return (0);
              }
              if(file.open(sd1.vwd(),cnamefile, O_READ)){  
                                do{
                                   while(((c = file.read()) > 0)){  //&& c!='32' spazio  //Ascii 13 = a capo   \n    && c!=32
                                         if(c!='\n'){
                                           Temporaneo+=(char)c;
                                         }
                                         if(c==32 && Temporaneo.equals(user1)){
                                             Serial.print(F(" Utente da cancellare trovato    "));
                                             Serial.println(Temporaneo);
                                             Temporaneo="";
                                             do{                               
                                             }while((c = file.read()) > 0 && c!='\n');                                         
                                        }
                                        else if(c=='\n'){
                                                         Serial.println(Temporaneo);
                                                         Temporaneo+=(char)c;
                                                         filet.print(Temporaneo);
                                                         Temporaneo="";
                                        } 
                                   }
				 
                               }while(c>0);                              
            }
            else{
                Serial.println(F("   Errore apertura file   "));   //adminpwd(sd, "LOGIN.PWD",user ,password ,homepage);
                return (0);
           }               
            
           file.close();
           filet.close();
           //letturafileprova2(sd1,".TXT");          
           sd1.remove(cnamefile);
           sd1.chdir(1);
           if(!sd1.rename(cnamefilet, cnamefile)){
             Serial.println(F("Errore nella rinominazione del file"));   //rinomina file logint.pwd in login.pwd 
             return(0);;
           }        
           return(1);
 }
   
   



long synctimezone(char pot[]){

                    Serial.println(F("Funzione  synctimezone "));
                    int z=0;
                    int x=1;
                    char mobile[2]={'0','0'};
                    do{ 
                       if(pot[z]=='&' || pot[z]=='\0'){                                                                                   
                               for(int i=z; i>0; i--){
                                          pot[i]=pot[i-1];
                                          pot[i-1]='0';
                               }                                                                           
                       }
                       if(pot[z]=='.'){                                                                                          
                                       mobile[0] = pot[z+1];
                                       if(pot[z+2]!='&'){
                                          mobile[1] = pot[z+2];
                                          pot[z+2]='&';                                                                                             
                                       }
                                       else{
                                          mobile[1]=mobile[0];
                                          mobile[0]='0';                                          
                                       } 
                                       pot[z]='&';
                                       pot[z+1]='&';
                                       z--;                                                                                                              
                       }
                       if(pot[0]==45){  //Se il fuso orario è negativo  ascii ---> 45 = - 
                                for(int i=0; i<7 && pot[i]!='&'; i++){                                                                                          
                                          pot[i]=pot[i+1];
                                }  
                                x=-1;
                       }
                       z++;
                         
                    }while(z!=7);
                    long ind =0;
                    ind=x*(((int)(pot[0]-'0'))*1000000+((int)(pot[1]-'0'))*100000+((int)(pot[2]-'0'))*10000+((int)(pot[3]-'0'))*1000+((int)(pot[4]-'0'))*100+((int)(pot[5]-'0'))*10+((int)(pot[6]-'0')));     
                    Serial.print(F("Time zone:"));
                    Serial.print(ind);
                    Serial.print(F("."));
                    short int s=((int)(mobile[0]-'0'))*10 +((int)(mobile[1]-'0'));
                    Serial.print(s);
                    Serial.println(F("h"));
                    
                    Serial.print(F("secondi da aggiungere o togliere per il timezone      :"));
                    s=s*60;
                    ind=ind*60*60;
                    if(ind<0) ind=ind-s;                                                                     
                    else  ind=ind+s;
                    Serial.println(ind);
                    return (ind);

}

String letturafilestring(SdFat sd1 ,String nomefilestring){ //La variabile testo è utilizzata per verificare l'estensione del file. 
              Serial.println(F("Funzione letturafilestring  "));
              SdFile file,sdfile;
              sd1.chdir(1); // si sposta sulla root dell'SD
              int c;
              String sdata,stringa="";
              char cnamefile[12];
              while(file.openNext(sd1.vwd(), O_READ )) {
                    sdata="";                    
                    file.getFilename(cnamefile);                 
                    sdata=cnamefile;
                    sdata.trim();
                    if(sdata.equalsIgnoreCase(nomefilestring)){    //lettura dei solo file in base all'estensione (*.txt)       
                         sdata.toCharArray(cnamefile,12);
                         if (sdfile.open(cnamefile, O_READ)){                                                            
                                  //sdfile.errorHalt("open failed");                                  
                                   while ((c = sdfile.read()) >= 0){
                                         //Serial.print((char)c);
                                         stringa+=(char)c;                                         
                                  }                                
                                Serial.println();
                          }
                          else return(stringa); //errore apertura file
                          sdfile.close();
                     }
                     
                     file.close();                             
              }           
              return(stringa);
}

int orolegale(byte sincdataora[]){     //sincdataora[GIORNO][MESE][ANNO][ORA][MINUTI][SECONDI][GIORNOSETTIMANA]
              int oraitaliana=0;        
              //if((3<=sincdataora[1]) && (sincdataora[1]<=10)){
                  //if(((sincdataora[0]+7>31) && (sincdataora[6]==4))) 
                   if((3==sincdataora[1]) && (sincdataora[1]==10) && ((sincdataora[0]+7>31) && (sincdataora[6]==4))||((3<sincdataora[1]) && (sincdataora[1]<10))){//(!((sincdataora[0]+7<31) && (sincdataora[1]==3 || sincdataora[1]==10) && (sincdataora[6]==3))) //(!((sincdataora[0]+7<31) && (sincdataora[1]==3 || sincdataora[1]==10)&& (sincdataora[6]=="DOM")))                             
                                     Serial.println(F("          Ora solare         "));
                                     oraitaliana=1;
                   }
                   else{
                                     Serial.println(F("          Ora legale         "));
                                     oraitaliana=1;
                   }
              //}
              return(oraitaliana*60*60);
}



unsigned long synctime(SdFat sd1, byte sincdataora[]){
 unsigned long temp=0;
 IPAddress timeServer(0, 0, 0, 0);
 long seconditimezone=0;
 if(sd.exists("tzone.ntp")){
             String timezone="";
             char pot[7];
             memset(pot,'&',7);             
             timezone=letturafilestring(sd1 ,"tzone.ntp");
             
             if(timezone==""){ 
                     Serial.println(F("Errore nella lettura del file     "));
             }
             else{
                  Serial.println(F("Apertura file tzone.ntp"));
                  timezone.toCharArray(pot,7);
                  seconditimezone = synctimezone(pot);
            }
 }
 timeServer = dnsresolve();

 if(!(timeServer ==((IPAddress) (0, 0, 0, 0)))){      // Se viene risolto l'ip dell NTP sincronizzo DS1307 
                Serial.println(F("Inizzializzazione del ds1307"));               
                for(int i=0; i<3 && temp==0 ; i++) temp=sendNTPpacket(timeServer); // send an NTP packet to a time server             
                temp+=seconditimezone;                
                unixtimetodate(temp,sincdataora);
                temp+=orolegale(sincdataora);
                unixtimetodate(temp,sincdataora);                
                scrivids1307(sincdataora);
                leggids1307(sincdataora);
 }     
 //temp=0;
 return(0);
}
