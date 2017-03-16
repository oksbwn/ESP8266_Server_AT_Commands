//API http://192.168.4.1/?STATUSDATA=10   //STATUSDATA: 1 Load status 2 Load status 'X' No change only respond Status
//Response {"STATUS":"10"}//STATUS: 1 Load status 2 Load status


#define DEBUG true
int loadOne=0; //Load One Status
int loadTwo=0; //Load two status
void setup()
{
  Serial.begin(9600);
  Serial1.begin(115200); // your esp's baud rate might be different

  sendData("AT+RST\r\n",2000,DEBUG); // reset module
  sendData("AT+CWMODE=2\r\n",1000,DEBUG); // configure as access point
  sendData("AT+CIFSR\r\n",1000,DEBUG); // get ip address
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // configure for multiple connections
  sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // turn on server on port 80
}
 
void loop()
{
  if(Serial1.available()) // check if the esp is sending a message 
  {
    if(Serial1.find("+IPD,"))
    {
     delay(1000); 
     int connectionId = Serial1.read()-48; 
      Serial.print("STATUSDATA : ");
      String loadStatus;
      if(Serial1.find("STATUSDATA"))
      for(int i=0;i<3;i++)
            loadStatus+=(char)Serial1.read();
            
      Serial.println(loadStatus);
      changeLoad(loadStatus);
     
     String webpage = "{\"STATUS\":\"";
     webpage+=loadOne;
     webpage+=loadTwo;
     webpage+="\"}";
 
     String cipSend = "AT+CIPSEND=";
     cipSend += connectionId;
     cipSend += ",";
     cipSend +=webpage.length();
     cipSend +="\r\n";
     
     sendData(cipSend,1000,DEBUG);
     sendData(webpage,1000,DEBUG);
     
     /*webpage="{\"ID\":\"ON\"}";
     
     cipSend = "AT+CIPSEND=";
     cipSend += connectionId;
     cipSend += ",";
     cipSend +=webpage.length();
     cipSend +="\r\n";
     
     sendData(cipSend,1000,DEBUG);
     sendData(webpage,1000,DEBUG);
   */
     String closeCommand = "AT+CIPCLOSE="; 
     closeCommand+=connectionId; // append connection id
     closeCommand+="\r\n";
     
     sendData(closeCommand,3000,DEBUG);
    }
  }
  changeLoadStatus();
}
void changeLoadStatus(){
//loadOne loadTwo
}
void changeLoad(String loadStatus){
if(loadStatus.charAt(1)!='X'){
		if(loadStatus.charAt(1)=='0')
		loadOne=0;
	  else
		loadOne=1;
		
	  if(loadStatus.charAt(2)=='0')
		loadTwo=0;
	  else
		loadTwo=1;
	}
      
}
String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    
    Serial1.print(command); // send the read character to the esp8266
    
    long int time = millis();
    
    while( (time+timeout) > millis())
    {
      while(Serial1.available())
      {
        
        // The esp has data so display its output to the serial window 
        char c = Serial1.read(); // read the next character.
        response+=c;
      }  
    }
    
    if(debug)
    {
      Serial.print(response);
    }
    
    return response;
}
 