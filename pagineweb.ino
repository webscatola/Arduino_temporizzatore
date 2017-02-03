
void statuscode(EthernetClient* client1){                                
                          client1->println("HTTP/1.1 200 OK");
                          client1->println("Content-Type: text/html");
                          client1->println();
}

void statuscoderr(EthernetClient* client1){
                          client1->println("HTTP/1.1 404 Not Found\nServer: arduino\nContent-Type: text/html\n\n<html><head><title>Arduino Web Server - Error 404</title></head><body><h1>Error 404: Sorry, that page cannot be found!</h1></body>");
                          Serial.print(F("***************************************************************404 Not Found\nServer:********************"));
}

void statuscoderrsd(EthernetClient* client1){
                          client1->println("HTTP/1.1 404 Not Found\nServer: arduino\nContent-Type: text/html\n\n<html><head><title>Arduino Web Server - Error 404</title></head><body><h1>Error 404: Sorry, that page cannot be found!</h1></body>");
                          Serial.println(F("***************************************************************Error 505:Errore Scrittura SD********************"));
                          Serial.print(F("***************************************************************Riavviare Il Dispositivo, se l'errore persiste cambiare SD********************"));
}


void info(EthernetClient* client1){
    client1->print(F("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//IT\">"));
    client1->print(F("<head>"));
    client1->print(F("<title>Info</title>"));
    client1->print(F("</head>"));
    client1->print(F("<body>"));
    client1->print(F("Arduino web v.9.7"));
    client1->print(F("</body>"));
    client1->print(F("</html>"));
}

void salvatag(EthernetClient* client1){
    client1->print(F("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//IT\">"));
    client1->print(F("<head>"));
    client1->print(F("<title>Info</title>"));
    client1->print(F("</head>"));
    client1->print(F("<body>"));
    client1->print(F("Salvattagio settaggi riuscita"));
    client1->print(F("</body>"));
    client1->print(F("</html>"));
}

void resetpin(EthernetClient* client1){


    client1->print(F("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//IT\">"));
    client1->print(F("<head>"));
    client1->print(F("<title>Reset Pin</title>"));
    client1->print(F("</head>"));
    client1->print(F("<body>"));      
    client1->print(F("<form action=\"salvatag.htm\" method=\"get\">"));
//    client1->print(F("<fieldset>"));
//    
//    client1->print(F("</fieldset>"));
    client1->print(F("<legend>Reset dei pin Arduino</legend>"));
//    client1->print(F("</fieldset>"));
    client1->print(F("<input type=\"submit\" name=\"button\" value=\"Salva \">"));
    client1->print(F("<input type=\"submit\" name=\"button\" value=\"Chiudi \">"));
    client1->print(F("</form>"));   
    client1->print(F("</body>"));
    client1->print(F("</html>"));
}

void timezone(EthernetClient* client1){


    client1->print(F("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//IT\">"));
    client1->print(F("<head>"));
    client1->print(F("<title>Time zone</title>"));
    client1->print(F("</head>"));
    client1->print(F("<body>"));      
    client1->print(F("<form action=\"salvatag.htm\" method=\"get\">"));
    client1->print(F("<fieldset>"));
    client1->print(F("<legend>Selezionare time zone</legend>")); 
    client1->print(F("<select name=\"timez\">"));
	client1->print(F("<option timeZoneId=\"1\" gmtAdjustment=\"GMT-12:00\" useDaylightTime=\"0\" value=\"-12\">(GMT-12:00) International Date Line West</option>"));
	client1->print(F("<option timeZoneId=\"2\" gmtAdjustment=\"GMT-11:00\" useDaylightTime=\"0\" value=\"-11\">(GMT-11:00) Midway Island, Samoa</option>"));
	client1->print(F("<option timeZoneId=\"3\" gmtAdjustment=\"GMT-10:00\" useDaylightTime=\"0\" value=\"-10\">(GMT-10:00) Hawaii</option>"));
	client1->print(F("<option timeZoneId=\"4\" gmtAdjustment=\"GMT-09:00\" useDaylightTime=\"1\" value=\"-9\">(GMT-09:00) Alaska</option>"));
	client1->print(F("<option timeZoneId=\"5\" gmtAdjustment=\"GMT-08:00\" useDaylightTime=\"1\" value=\"-8\">(GMT-08:00) Pacific Time (US & Canada)</option>"));
	client1->print(F("<option timeZoneId=\"6\" gmtAdjustment=\"GMT-08:00\" useDaylightTime=\"1\" value=\"-8\">(GMT-08:00) Tijuana, Baja California</option>"));
	client1->print(F("<option timeZoneId=\"7\" gmtAdjustment=\"GMT-07:00\" useDaylightTime=\"0\" value=\"-7\">(GMT-07:00) Arizona</option>"));
	client1->print(F("<option timeZoneId=\"8\" gmtAdjustment=\"GMT-07:00\" useDaylightTime=\"1\" value=\"-7\">(GMT-07:00) Chihuahua, La Paz, Mazatlan</option>"));
	client1->print(F("<option timeZoneId=\"9\" gmtAdjustment=\"GMT-07:00\" useDaylightTime=\"1\" value=\"-7\">(GMT-07:00) Mountain Time (US & Canada)</option>"));
	client1->print(F("<option timeZoneId=\"10\" gmtAdjustment=\"GMT-06:00\" useDaylightTime=\"0\" value=\"-6\">(GMT-06:00) Central America</option>"));
	client1->print(F("<option timeZoneId=\"11\" gmtAdjustment=\"GMT-06:00\" useDaylightTime=\"1\" value=\"-6\">(GMT-06:00) Central Time (US & Canada)</option>"));
	client1->print(F("<option timeZoneId=\"12\" gmtAdjustment=\"GMT-06:00\" useDaylightTime=\"1\" value=\"-6\">(GMT-06:00) Guadalajara, Mexico City, Monterrey</option>"));
	client1->print(F("<option timeZoneId=\"13\" gmtAdjustment=\"GMT-06:00\" useDaylightTime=\"0\" value=\"-6\">(GMT-06:00) Saskatchewan</option>"));
	client1->print(F("<option timeZoneId=\"14\" gmtAdjustment=\"GMT-05:00\" useDaylightTime=\"0\" value=\"-5\">(GMT-05:00) Bogota, Lima, Quito, Rio Branco</option>"));
	client1->print(F("<option timeZoneId=\"15\" gmtAdjustment=\"GMT-05:00\" useDaylightTime=\"1\" value=\"-5\">(GMT-05:00) Eastern Time (US & Canada)</option>"));
	client1->print(F("<option timeZoneId=\"16\" gmtAdjustment=\"GMT-05:00\" useDaylightTime=\"1\" value=\"-5\">(GMT-05:00) Indiana (East)</option>"));
	client1->print(F("<option timeZoneId=\"17\" gmtAdjustment=\"GMT-04:00\" useDaylightTime=\"1\" value=\"-4\">(GMT-04:00) Atlantic Time (Canada)</option>"));
	client1->print(F("<option timeZoneId=\"18\" gmtAdjustment=\"GMT-04:00\" useDaylightTime=\"0\" value=\"-4\">(GMT-04:00) Caracas, La Paz</option>"));
	client1->print(F("<option timeZoneId=\"19\" gmtAdjustment=\"GMT-04:00\" useDaylightTime=\"0\" value=\"-4\">(GMT-04:00) Manaus</option>"));
	client1->print(F("<option timeZoneId=\"20\" gmtAdjustment=\"GMT-04:00\" useDaylightTime=\"1\" value=\"-4\">(GMT-04:00) Santiago</option>"));
	client1->print(F("<option timeZoneId=\"21\" gmtAdjustment=\"GMT-03:30\" useDaylightTime=\"1\" value=\"-3.5\">(GMT-03:30) Newfoundland</option>"));
	client1->print(F("<option timeZoneId=\"22\" gmtAdjustment=\"GMT-03:00\" useDaylightTime=\"1\" value=\"-3\">(GMT-03:00) Brasilia</option>"));
	client1->print(F("<option timeZoneId=\"23\" gmtAdjustment=\"GMT-03:00\" useDaylightTime=\"0\" value=\"-3\">(GMT-03:00) Buenos Aires, Georgetown</option>"));
	client1->print(F("<option timeZoneId=\"24\" gmtAdjustment=\"GMT-03:00\" useDaylightTime=\"1\" value=\"-3\">(GMT-03:00) Greenland</option>"));
	client1->print(F("<option timeZoneId=\"25\" gmtAdjustment=\"GMT-03:00\" useDaylightTime=\"1\" value=\"-3\">(GMT-03:00) Montevideo</option>"));
	client1->print(F("<option timeZoneId=\"26\" gmtAdjustment=\"GMT-02:00\" useDaylightTime=\"1\" value=\"-2\">(GMT-02:00) Mid-Atlantic</option>"));
	client1->print(F("<option timeZoneId=\"27\" gmtAdjustment=\"GMT-01:00\" useDaylightTime=\"0\" value=\"-1\">(GMT-01:00) Cape Verde Is.</option>"));
	client1->print(F("<option timeZoneId=\"28\" gmtAdjustment=\"GMT-01:00\" useDaylightTime=\"1\" value=\"-1\">(GMT-01:00) Azores</option>"));
	client1->print(F("<option timeZoneId=\"29\" gmtAdjustment=\"GMT+00:00\" useDaylightTime=\"0\" value=\"0\">(GMT+00:00) Casablanca, Monrovia, Reykjavik</option>"));
	client1->print(F("<option timeZoneId=\"30\" gmtAdjustment=\"GMT+00:00\" useDaylightTime=\"1\" value=\"0\">(GMT+00:00) Greenwich Mean Time : Dublin, Edinburgh, Lisbon, London</option>"));
	client1->print(F("<option timeZoneId=\"31\" gmtAdjustment=\"GMT+01:00\" useDaylightTime=\"1\" value=\"1\">(GMT+01:00) Amsterdam, Berlin, Bern, Rome, Stockholm, Vienna</option>"));
	client1->print(F("<option timeZoneId=\"32\" gmtAdjustment=\"GMT+01:00\" useDaylightTime=\"1\" value=\"1\">(GMT+01:00) Belgrade, Bratislava, Budapest, Ljubljana, Prague</option>"));
	client1->print(F("<option timeZoneId=\"33\" gmtAdjustment=\"GMT+01:00\" useDaylightTime=\"1\" value=\"1\">(GMT+01:00) Brussels, Copenhagen, Madrid, Paris</option>"));
	client1->print(F("<option timeZoneId=\"34\" gmtAdjustment=\"GMT+01:00\" useDaylightTime=\"1\" value=\"1\">(GMT+01:00) Sarajevo, Skopje, Warsaw, Zagreb</option>"));
	client1->print(F("<option timeZoneId=\"35\" gmtAdjustment=\"GMT+01:00\" useDaylightTime=\"1\" value=\"1\">(GMT+01:00) West Central Africa</option>"));
	client1->print(F("<option timeZoneId=\"36\" gmtAdjustment=\"GMT+02:00\" useDaylightTime=\"1\" value=\"2\">(GMT+02:00) Amman</option>"));
	client1->print(F("<option timeZoneId=\"37\" gmtAdjustment=\"GMT+02:00\" useDaylightTime=\"1\" value=\"2\">(GMT+02:00) Athens, Bucharest, Istanbul</option>"));
	client1->print(F("<option timeZoneId=\"38\" gmtAdjustment=\"GMT+02:00\" useDaylightTime=\"1\" value=\"2\">(GMT+02:00) Beirut</option>"));
	client1->print(F("<option timeZoneId=\"39\" gmtAdjustment=\"GMT+02:00\" useDaylightTime=\"1\" value=\"2\">(GMT+02:00) Cairo</option>"));
	client1->print(F("<option timeZoneId=\"40\" gmtAdjustment=\"GMT+02:00\" useDaylightTime=\"0\" value=\"2\">(GMT+02:00) Harare, Pretoria</option>"));
	client1->print(F("<option timeZoneId=\"41\" gmtAdjustment=\"GMT+02:00\" useDaylightTime=\"1\" value=\"2\">(GMT+02:00) Helsinki, Kyiv, Riga, Sofia, Tallinn, Vilnius</option>"));
	client1->print(F("<option timeZoneId=\"42\" gmtAdjustment=\"GMT+02:00\" useDaylightTime=\"1\" value=\"2\">(GMT+02:00) Jerusalem</option>"));
	client1->print(F("<option timeZoneId=\"43\" gmtAdjustment=\"GMT+02:00\" useDaylightTime=\"1\" value=\"2\">(GMT+02:00) Minsk</option>"));
	client1->print(F("<option timeZoneId=\"44\" gmtAdjustment=\"GMT+02:00\" useDaylightTime=\"1\" value=\"2\">(GMT+02:00) Windhoek</option>"));
	client1->print(F("<option timeZoneId=\"45\" gmtAdjustment=\"GMT+03:00\" useDaylightTime=\"0\" value=\"3\">(GMT+03:00) Kuwait, Riyadh, Baghdad</option>"));
	client1->print(F("<option timeZoneId=\"46\" gmtAdjustment=\"GMT+03:00\" useDaylightTime=\"1\" value=\"3\">(GMT+03:00) Moscow, St. Petersburg, Volgograd</option>"));
	client1->print(F("<option timeZoneId=\"47\" gmtAdjustment=\"GMT+03:00\" useDaylightTime=\"0\" value=\"3\">(GMT+03:00) Nairobi</option>"));
	client1->print(F("<option timeZoneId=\"48\" gmtAdjustment=\"GMT+03:00\" useDaylightTime=\"0\" value=\"3\">(GMT+03:00) Tbilisi</option>"));
	client1->print(F("<option timeZoneId=\"49\" gmtAdjustment=\"GMT+03:30\" useDaylightTime=\"1\" value=\"3.5\">(GMT+03:30) Tehran</option>"));
	client1->print(F("<option timeZoneId=\"50\" gmtAdjustment=\"GMT+04:00\" useDaylightTime=\"0\" value=\"4\">(GMT+04:00) Abu Dhabi, Muscat</option>"));
	client1->print(F("<option timeZoneId=\"51\" gmtAdjustment=\"GMT+04:00\" useDaylightTime=\"1\" value=\"4\">(GMT+04:00) Baku</option>"));
	client1->print(F("<option timeZoneId=\"52\" gmtAdjustment=\"GMT+04:00\" useDaylightTime=\"1\" value=\"4\">(GMT+04:00) Yerevan</option>"));
	client1->print(F("<option timeZoneId=\"53\" gmtAdjustment=\"GMT+04:30\" useDaylightTime=\"0\" value=\"4.5\">(GMT+04:30) Kabul</option>"));
	client1->print(F("<option timeZoneId=\"54\" gmtAdjustment=\"GMT+05:00\" useDaylightTime=\"1\" value=\"5\">(GMT+05:00) Yekaterinburg</option>"));
	client1->print(F("<option timeZoneId=\"55\" gmtAdjustment=\"GMT+05:00\" useDaylightTime=\"0\" value=\"5\">(GMT+05:00) Islamabad, Karachi, Tashkent</option>"));
	client1->print(F("<option timeZoneId=\"56\" gmtAdjustment=\"GMT+05:30\" useDaylightTime=\"0\" value=\"5.5\">(GMT+05:30) Sri Jayawardenapura</option>"));
	client1->print(F("<option timeZoneId=\"57\" gmtAdjustment=\"GMT+05:30\" useDaylightTime=\"0\" value=\"5.5\">(GMT+05:30) Chennai, Kolkata, Mumbai, New Delhi</option>"));
	client1->print(F("<option timeZoneId=\"58\" gmtAdjustment=\"GMT+05:45\" useDaylightTime=\"0\" value=\"5.75\">(GMT+05:45) Kathmandu</option>"));
	client1->print(F("<option timeZoneId=\"59\" gmtAdjustment=\"GMT+06:00\" useDaylightTime=\"1\" value=\"6\">(GMT+06:00) Almaty, Novosibirsk</option>"));
	client1->print(F("<option timeZoneId=\"60\" gmtAdjustment=\"GMT+06:00\" useDaylightTime=\"0\" value=\"6\">(GMT+06:00) Astana, Dhaka</option>"));
	client1->print(F("<option timeZoneId=\"61\" gmtAdjustment=\"GMT+06:30\" useDaylightTime=\"0\" value=\"6.5\">(GMT+06:30) Yangon (Rangoon)</option>"));
	client1->print(F("<option timeZoneId=\"62\" gmtAdjustment=\"GMT+07:00\" useDaylightTime=\"0\" value=\"7\">(GMT+07:00) Bangkok, Hanoi, Jakarta</option>"));
	client1->print(F("<option timeZoneId=\"63\" gmtAdjustment=\"GMT+07:00\" useDaylightTime=\"1\" value=\"7\">(GMT+07:00) Krasnoyarsk</option>"));
	client1->print(F("<option timeZoneId=\"64\" gmtAdjustment=\"GMT+08:00\" useDaylightTime=\"0\" value=\"8\">(GMT+08:00) Beijing, Chongqing, Hong Kong, Urumqi</option>"));
	client1->print(F("<option timeZoneId=\"65\" gmtAdjustment=\"GMT+08:00\" useDaylightTime=\"0\" value=\"8\">(GMT+08:00) Kuala Lumpur, Singapore</option>"));
	client1->print(F("<option timeZoneId=\"66\" gmtAdjustment=\"GMT+08:00\" useDaylightTime=\"0\" value=\"8\">(GMT+08:00) Irkutsk, Ulaan Bataar</option>"));
	client1->print(F("<option timeZoneId=\"67\" gmtAdjustment=\"GMT+08:00\" useDaylightTime=\"0\" value=\"8\">(GMT+08:00) Perth</option>"));
	client1->print(F("<option timeZoneId=\"68\" gmtAdjustment=\"GMT+08:00\" useDaylightTime=\"0\" value=\"8\">(GMT+08:00) Taipei</option>"));
	client1->print(F("<option timeZoneId=\"69\" gmtAdjustment=\"GMT+09:00\" useDaylightTime=\"0\" value=\"9\">(GMT+09:00) Osaka, Sapporo, Tokyo</option>"));
	client1->print(F("<option timeZoneId=\"70\" gmtAdjustment=\"GMT+09:00\" useDaylightTime=\"0\" value=\"9\">(GMT+09:00) Seoul</option>"));
	client1->print(F("<option timeZoneId=\"71\" gmtAdjustment=\"GMT+09:00\" useDaylightTime=\"1\" value=\"9\">(GMT+09:00) Yakutsk</option>"));
	client1->print(F("<option timeZoneId=\"72\" gmtAdjustment=\"GMT+09:30\" useDaylightTime=\"0\" value=\"9.5\">(GMT+09:30) Adelaide</option>"));
	client1->print(F("<option timeZoneId=\"73\" gmtAdjustment=\"GMT+09:30\" useDaylightTime=\"0\" value=\"9.5\">(GMT+09:30) Darwin</option>"));
	client1->print(F("<option timeZoneId=\"74\" gmtAdjustment=\"GMT+10:00\" useDaylightTime=\"0\" value=\"10\">(GMT+10:00) Brisbane</option>"));
	client1->print(F("<option timeZoneId=\"75\" gmtAdjustment=\"GMT+10:00\" useDaylightTime=\"1\" value=\"10\">(GMT+10:00) Canberra, Melbourne, Sydney</option>"));
	client1->print(F("<option timeZoneId=\"76\" gmtAdjustment=\"GMT+10:00\" useDaylightTime=\"1\" value=\"10\">(GMT+10:00) Hobart</option>"));
	client1->print(F("<option timeZoneId=\"77\" gmtAdjustment=\"GMT+10:00\" useDaylightTime=\"0\" value=\"10\">(GMT+10:00) Guam, Port Moresby</option>"));
	client1->print(F("<option timeZoneId=\"78\" gmtAdjustment=\"GMT+10:00\" useDaylightTime=\"1\" value=\"10\">(GMT+10:00) Vladivostok</option>"));
	client1->print(F("<option timeZoneId=\"79\" gmtAdjustment=\"GMT+11:00\" useDaylightTime=\"1\" value=\"11\">(GMT+11:00) Magadan, Solomon Is., New Caledonia</option>"));
	client1->print(F("<option timeZoneId=\"80\" gmtAdjustment=\"GMT+12:00\" useDaylightTime=\"1\" value=\"12\">(GMT+12:00) Auckland, Wellington</option>"));
	client1->print(F("<option timeZoneId=\"81\" gmtAdjustment=\"GMT+12:00\" useDaylightTime=\"0\" value=\"12\">(GMT+12:00) Fiji, Kamchatka, Marshall Is.</option>"));
	client1->print(F("<option timeZoneId=\"82\" gmtAdjustment=\"GMT+13:00\" useDaylightTime=\"0\" value=\"13\">(GMT+13:00) Nuku'alofa</option>"));
    client1->print(F("</select>"));
    client1->print(F("</fieldset>"));
    
    client1->print(F("<input type=\"submit\" name=\"button\" value=\"Salva \">"));
    client1->print(F("</form>"));   
    client1->print(F("</body>"));
    client1->print(F("</html>"));

}




void userrm(EthernetClient* client1, SdFat sd1){  // rimozione utente
  
    SdFile file;
    sd1.chdir(1);
    int c, ciclo=0;
    String Temporaneo="",campo="rmvuser";

    client1->print(F("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//IT\">"));
    client1->print(F("<head>"));
    client1->print(F("<title>Users</title>"));
    client1->print(F("</head>"));
    client1->print(F("<body>"));
    
    
    client1->print(F("<form action=\"salvatag.htm\" method=\"get\">"));
    client1->print(F("<fieldset>"));
    client1->print(F("<legend>Selezionare utente da rimuovere</legend>")); 
    client1->print(F("<select name=\"userrm\" >"));
    if(file.open(sd1.vwd(),"LOGIN.PWD", O_READ)){  
                             Serial.println(F("Apertura file riuscita Login.pwd"));              
                             do{
                                 Temporaneo="";
                                   while(((c = file.read()) > 0) && c!=32){  //&& c!='32'   //Ascii 13 = Spazio
                                         if(c!=32){
                                           //Serial.print((char)c);
                                           Temporaneo+=(char)c;
                                         }
                                   }
                                   //campo="rmvuser";
                                   client1->print(F("<option value=\""));
                                   client1->print(Temporaneo);
                                   client1->print(F("\" selected=\"selected\">"));
                                   client1->print(Temporaneo);
                                   client1->print(F("</option>"));                               
                                   Temporaneo="";
                                   ciclo++;

                                   do{
                                 
                                   }while((c = file.read()) > 0 && c!='\n');
				 
                              }while(c>0);                              
                              client1->print(F("</select>"));
                              client1->print(F("</fieldset>"));
                              client1->print(F("<input type=\"submit\" name=\"button\" value=\"Salva \">"));
                              //client1->print(F("<input type=\"reset\" name=\"reset\" value=\"Cancella\">"));
                              client1->print(F("</form>"));
                              file.close();
    }
    else Serial.println(F("   Errore apertura file   "));
    
    
    client1->print(F("</body>"));
    client1->print(F("</html>")); 
    file.close();
}


void admin(EthernetClient* client1, SdFat sd1){

    
    client1->print(F("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//IT\">"));
    client1->print(F("<head>"));
    //client1->print(F("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">"));
    client1->print(F("<title>Amministrazione</title>"));
    client1->print(F("</head>"));
    client1->print(F("<body>"));
    //client1->print(F("<div style=\"position:absolute; top:50%; left:50%; margin-top:-250px; margin-left:-450px; width:900px; height:500px; \">"));
    client1->print(F("<table width=\"40%\" height=\"30%\" border=\"0\" align=\"center\" bgcolor=\"#e8e8e8\">"));
    
    client1->print(F("<tr>"));
      client1->print(F("<th><a href=\"usrad.htm\" target=\"_blank\"><img alt=\"\" src=\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABgAAAAYCAYAAADgdz34AAABUklEQVR42mNkoDFgHCwWyAJxGRA7ALEAED8F4nog3kkNC2SA+AwQi0HV/0eSA4n7AfELSizoAeJiqMHoFoD4c4A4lRILDgGxLR75a0CsTYkFp4HYBI8PbgGxOiUWbAZiHzwWgCLagxILWoG4Co98EwMkRZFtwT4gdsTjA5AP/SixYBEQx+KxYCYQZ1BiAShj3WDAng8eAbEuEH+mxAIQmAbEmVjE+xggeQQnINYCkOuvArEIkg9eM0DS/xtqWAACl4FYB4l/AYgNCWki1gImBkgBJ4Hkg2cMkELwPz6NxFggBMQTgTiaATOSQSmsEIjfk2OBAQMkYqOAmAePuk9AvIwBkhAuE2MBKBhmALE/kmthLseWD5D5a6GOeoPLAl4gPgfEymiaibEAxgalNjMg/o7NAlB49hERL4QAKGfPxGZBEhBbUsGCA0C8FJsFVAc0twAA++BKGdW04CAAAAAASUVORK5CYII=\" /></a></th>"));
      //delay(200);
      client1->print(F("<th><a href=\"timezone.htm\" target=\"_blank\"><img alt=\"\" src=\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABgAAAAYCAYAAADgdz34AAABsklEQVR42q3WTSgEYRzH8WcVpRw4IEcpLSlHocTFwcEerZIDOSk5eClLNqIsB7mSgxMnkRyUvEU5SW1oU47yFgelJHz/zTPbjHZ2Znb96tPWzDP732eetw0of5lGBC+4wgHWceP0QCCDAjlYQi3aEMY5hpH4jwKSCcu1fAxgCGNYTlegH0eI+yhgpgrbWLO0sxXowwjq8exQYAo/mHS4X4JTxMyemAUq9Y0mXKd5RWX68z5NG+nJMRqRMAts4QxzloY92MGj8p9RNCAkBYI4RDk+/nR3H+P6B/iJDPwdmqVAFIUYTNEwhE2s6vvvPoos4k0KyKyZwZ5DwxX04hZdypjzXtKKiBR4Qg0eHBoW4AIV+NKvLOahQCniAf1QHr7TNK7DCXKVMU07seFSQFb8p9cCElkj5iwL+yng9oqs6dCfbl9ue0Vug5xpkoMcVc7TNJskp6nTQssmtoUmSbVVZBPbViHxutl5ScrNTmJu13Ijkw1O4rhdm5GDpBvtGfTE9cCx9mQWC8o4e90G3teRaUbGZF4ZW4T8a9jFpTIWpaRY/dOhH9Rf0IJqFOnrr8rj35ZfUlFuuRTq27cAAAAASUVORK5CYII=\" /></a></th>"));
      //delay(300);
      client1->print(F("<th><a href=\"info.htm\" target=\"_blank\"><img alt=\"\" src=\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABgAAAAYCAYAAADgdz34AAABXklEQVR42r3WP0gDMRTH8auD4KaDOrsUFcFRUBC7KDjo4iAI3VzFoSooQkEU/DOIq1tBcHDRQdClVVBwFMRKF2d1aDdBRPw+eCmttE1ijQ8+BO5y90uau1xjUePqxSwS6EeHHi/iEVkc46neDWJ1jsexiyG9wTnu8abnOzGISR3AHZZQcAmYxxb2cIB3yyzbsIAUVnHYKGAdSUwhH/lVH86QwUatABn5Mkbw6nlzU124wY6ZiQmI64nRX4y81kyudaAFE3CKW2xbLs5om7T0W8EwpiVAHsUceiL7gh5pO2fpJwv/jDEJSKMdi03+ND9rHyUJuMImLh0umtD2wqHvONYkQF6eAbw4XJTVNuHQtxsPEvCJVnz9cUALPv4lIPhP5LPIPgHlRU5H7o+pT0D5MfV50VwDql40Kdet4kTbGUu/qq1CKvhmJxV0uzYV9INTOZNgn0xTQT/6ldX035ZvUKZnufJl4/MAAAAASUVORK5CYII=\" /></a></th>"));
      client1->print(F("</tr>"));
//    
    client1->print(F("<tr>"));
    delay(300);
    client1->print(F("<th><a href=\"usragg.htm\" target=\"_blank\"><img alt=\"\" src=\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABgAAAAYCAYAAADgdz34AAACeElEQVR42rWVTWgTQRTH39vNJs0mbWJDRGj8SKulKPhBEewhIKUUtR4EqYKiiAcvLZSC6EUQTwERpIpCUWKxqG1PFfXgST2pxIMHBQ8Kig3U0lBTN+km+/F80US2adgktXmn/8783/xm5u3MINQ50K5z/w4Z/bLgtrHQdFxZWhWgd5dHGjiw7i3LPXYAjttHriYGawb0dzWGT0aavrD8qWo0nlZNzdrv84gBScTTDPjGgNZVAwwTbh69lhgq53l8MfSeAT4GhG0BscFQqGen94RLEgLFDsMkfyqtn3NJGPc0OF6US06lNV4BuP1ex6h1rjmdki8/KA9PjXyfwdfR1n27w+7n3N5YAJIFXqqtbVDBv/gpofai8qCDC4l7q0ioFcCa4rg43q7zhwj1CQMXxrZShVn/xwpYJGPh+gLm7myuL2B2dKM1Ya2DMB5teRX0QkQSCRAB+b/+Sy7VbBYkL5KpE+kqlPVYtJJFmk3hU5y+EJrgtuN+maBZNsHnJhDKnG/Xpm5oPnQfzJwCP+5tZ5y5wqMZAAsZAZJphEwO86BJ5OM+wcf9GBTnxKkeJ4HsNLHBQeR0EEgioKftIPm6byAPTHMM0HQDcgZiVkdSNYR0DlFlzQYeBTF/C/JYU8sAlg6LzqcABtp7aFtfFPP355uRLt4Po1DMUj+sBOS3qFK1GoId0BI5D0b2F3x9NlxtkScLNbBbwbLVFK8AqNI/hY+GQzG3RGeqSbC2VePn+ozhpf4NfZ1bHE8Ewf75rDVME+hjwjj8Z9DrZ0MD65vwsihQENbgJBsmzs8rdGXo7sytf7PubJPR7bJ94KsONWcuvfucoSK1rvEbZuduczhsQ+gAAAAASUVORK5CYII=\" /></a></th>"));
    delay(300);
    client1->print(F("<th><a href=\"ethad.htm\" target=\"_blank\"><img alt=\"eth\" src=\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAABHNCSVQICAgIfAhkiAAAAAlwSFlzAAARCQAAEQkBwOWiGAAAABl0RVh0U29mdHdhcmUAd3d3Lmlua3NjYXBlLm9yZ5vuPBoAAAIvSURBVFiF7Zc/b9NQFMV/9yaVKkAqAxJqFgaG7M/+AFWHLEj9AM1IixBCqGwwdGKAjU4oAhaGsiJlzIDyAWzvHenQIoWhQ/pHovZlyAtYUW1hcNqlZ3r33etzzvN9kn3FzAAQEXHObYjIGhAAy9SLQyA2s36SJB/NC4uZ4ZxrqeousFKzaBGGWZZ1kyQ5aIqIBEGQF98DBsBBzaItoAO0gRVV3RWRVXHObYrIe1/0DtiKouhnzeIAhGG4AOwATwDM7JH6nsPk5HMTB/DcW14LEVlTJhcOYDBP8RkTAx8Gyp/bXnfPyzDVWm5WfdI5d09VnwO3Z1JHWZa9TZLkWxW+ygZUdRt4WJC7BWxU4qtqALjzj7naDNSKwhYEQbApIq+AxZnUzRK+B2EYHs3snZnZdhzHHyoZEJFnwN0SsSK+pZm9Jc91oYGyFixUFC9DIdeV34FrA9cGqhj4nlunwCgXj/zeRbW1GNgH7gPHACLSU9X13ySq6yLS8+Gxr92v08BpFEUnwDmAmY3TNB1Pk2majs1sGp/72tM6DcwNZQZ+1KhTyFX4LVDVp2maPhaRRSb/9ACfgRtALCIj4BOAX8c+PvG1X4BlMztrNBo9CiBBEJhfv4yi6M1fnui/EIbhC+A1TFowPV3rMsRntA6VyasD6Pj/9rnCa3R8GKuZ9X3QBnbmaSI3mLQBzKwvTO7BVy53NAMYxnG82jQzc851c8NpO1c0LwyzLOuamclVj+e/AH/KzxahRAbIAAAAAElFTkSuQmCC\" /></a></th>"));
    delay(300);
    client1->print(F("<th><a href=\"userrm.htm\" target=\"_blank\"><img alt=\"time3\" src=\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAABWklEQVQ4ja1TMUvDQBT+7iztWsSlHZLJZigFC12FiINzx26SKVtnHcxlcXcyixEnR107FPIDChXEoTpoMrRLkYIuLdjnYO+4tA1Y8YPj4H3v+97du3eMiKBj69SrzA2zBUAgDcGT+Pbr3H/Wg0w3YEHorREuQ5Dr+CsGLAgpU7IG5DoMALhWeSNIDeMnZ5W5YQ4AYG9nG8V8AZPZFA/j95RgHceT2MotGobrg30cW7tKEA1HaHa6AIC7o0PY5ZLi7l9jNDtdzA2zxXB5RaJRh9eogwWhfkdEwxEAwC6XVji/14fo9X960K5VUwmLO8Iul1Jich3FtWtVqCYWC3mVINcyZEzuUpMDgLePT+WcBRaEINdROZPpTJ1A3AxeVqrKHkTDUerokrt4fAIAoZ7xL6/Ak9hiRKRGeMM5EOQ6/v+MshYQv9AKKU4ZLEx8nsRWhpHgSWzpPxEAvgHST7pu8mrutQAAAABJRU5ErkJggg==\" /></a></th>"));
    client1->print(F("</tr>"));
    
    client1->print(F("<tr>"));
    delay(300);
    client1->print(F("<th><a href=\"resetpin.htm\" target=\"_blank\"><img alt=\"\" src=\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABgAAAAYCAYAAADgdz34AAACeElEQVR42rWVTWgTQRTH39vNJs0mbWJDRGj8SKulKPhBEewhIKUUtR4EqYKiiAcvLZSC6EUQTwERpIpCUWKxqG1PFfXgST2pxIMHBQ8Kig3U0lBTN+km+/F80US2adgktXmn/8783/xm5u3MINQ50K5z/w4Z/bLgtrHQdFxZWhWgd5dHGjiw7i3LPXYAjttHriYGawb0dzWGT0aavrD8qWo0nlZNzdrv84gBScTTDPjGgNZVAwwTbh69lhgq53l8MfSeAT4GhG0BscFQqGen94RLEgLFDsMkfyqtn3NJGPc0OF6US06lNV4BuP1ex6h1rjmdki8/KA9PjXyfwdfR1n27w+7n3N5YAJIFXqqtbVDBv/gpofai8qCDC4l7q0ioFcCa4rg43q7zhwj1CQMXxrZShVn/xwpYJGPh+gLm7myuL2B2dKM1Ya2DMB5teRX0QkQSCRAB+b/+Sy7VbBYkL5KpE+kqlPVYtJJFmk3hU5y+EJrgtuN+maBZNsHnJhDKnG/Xpm5oPnQfzJwCP+5tZ5y5wqMZAAsZAZJphEwO86BJ5OM+wcf9GBTnxKkeJ4HsNLHBQeR0EEgioKftIPm6byAPTHMM0HQDcgZiVkdSNYR0DlFlzQYeBTF/C/JYU8sAlg6LzqcABtp7aFtfFPP355uRLt4Po1DMUj+sBOS3qFK1GoId0BI5D0b2F3x9NlxtkScLNbBbwbLVFK8AqNI/hY+GQzG3RGeqSbC2VePn+ozhpf4NfZ1bHE8Ewf75rDVME+hjwjj8Z9DrZ0MD65vwsihQENbgJBsmzs8rdGXo7sytf7PubJPR7bJ94KsONWcuvfucoSK1rvEbZuduczhsQ+gAAAAASUVORK5CYII=\" /></a></th>"));
    delay(300);
    client1->print(F("<th><a href=\"ethad.htm\" target=\"_blank\"><img alt=\"eth\" src=\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgAAABHNCSVQICAgIfAzAAARCQAAWiGAAAABl0RVh0U29mdHdhcmUAd3d3Lmlua3NjYXBlLm9yZ5vuPBoAAAIvSURBVFiF7Zc/b9NQFMV/9yaVKkAqAxJqFgaG7M/+AFWHLEj9AM1IixBCqGwwdGKAjU4oAhaGsiJlzIDyAWzvHenQIoWhQ/pHovZlyAtYUW1hcNqlZ3r33etzzvN9kn3FzAAQEXHObYjIGhAAy9SLQyA2s36SJB/NC4uZ4ZxrqeousFKzaBGGWZZ1kyQ5aIqIBEGQF98DBsBBzaItoAO0gRVV3RWRVXHObYrIe1/0DtiKouhnzeIAhGG4AOwATwDM7JH6nsPk5HMTB/DcW14LEVlTJhcOYDBP8RkTAx8Gyp/bXnfPyzDVWm5WfdI5d09VnwO3Z1JHWZa9TZLkWxW+ygZUdRt4WJC7BWxU4qtqALjzj7naDNSKwhYEQbApIq+AxZnUzRK+B2EYHs3snZnZdhzHHyoZEJFnwN0SsSK+pZm9Jc91oYGyFixUFC9DIdeV34FrA9cGqhj4nlunwCgXj/zeRbW1GNgH7gPHACLSU9X13ySq6yLS8+Gxr92v08BpFEUnwDmAmY3TNB1Pk2majs1sGp/72tM6DcwNZQZ+1KhTyFX4LVDVp2maPhaRRSb/9ACfgRtALCIj4BOAX8c+PvG1X4BlMztrNBo9CiBBEJhfv4yi6M1fnui/EIbhC+A1TFowPV3rMsRntA6VyasD6Pj/9rnCa3R8GKuZ9X3QBnbmaSI3mLQBzKwvTO7BVy53NAMYxnG82jQzc851c8NpO1c0LwyzLOuamclVj+e/AH/KzxahRAbIAAAAAElFTkSuQmCC\" /></a></th>"));
    delay(300);
    client1->print(F("<th><a href=\"userrm.htm\" target=\"_blank\"><img alt=\"time3\" src=\"data:image/png;base64,iVBORw0KGgoAf8/9hAAABWklEQVQ4ja1TMUvDQBT+7iztWsSlHZLJZigFC12FiINzx26SKVtnHcxlcXcyixEnR107FPIDChXEoTpoMrRLkYIuLdjnYO+4tA1Y8YPj4H3v+97du3eMiKBj69SrzA2zBUAgDcGT+Pbr3H/Wg0w3YEHorREuQ5Dr+CsGLAgpU7IG5DoMALhWeSNIDeMnZ5W5YQ4AYG9nG8V8AZPZFA/j95RgHceT2MotGobrg30cW7tKEA1HaHa6AIC7o0PY5ZLi7l9jNDtdzA2zxXB5RaJRh9eogwWhfkdEwxEAwC6XVji/14fo9X960K5VUwmLO8Iul1Jich3FtWtVqCYWC3mVINcyZEzuUpMDgLePT+WcBRaEINdROZPpTJ1A3AxeVqrKHkTDUerokrt4fAIAoZ7xL6/Ak9hiRKRGeMM5EOQ6/v+MshYQv9AKKU4ZLEx8nsRWhpHgSWzpPxEAvgHST7pu8mrutQAAAABJRU5ErkJggg==\" /></a></th>"));
    client1->print(F("</tr>"));
    
    client1->print(F("</table>"));  //http://www.logicaprogrammabile.it/visualizzare-immagini-con-arduino-webserver/
    
    //client1->print(F("</div>"));
    client1->print(F("</body>"));
    client1->print(F("</html>")); 
    
}

void usragg(EthernetClient* client1){

    client1->print(F("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//IT\">"));
    client1->print(F("<head>"));
    client1->print(F("<title>New Users</title>"));
    client1->print(F("</head>"));
    client1->print(F("<body>"));
    
    client1->print(F("<table  width=\"40%\" border=\"0\" align=\"center\" bgcolor=\"#e8e8e8\">"));
    client1->print(F("<form action=\"salvatag.htm\" method=\"get\">"));
    
    client1->print(F("<tr>"));
    client1->print(F("<th>Username</th>"));
    client1->print(F("<th>Password</th>"));
    client1->print(F("<th>Homepage</th>"));
    client1->print(F("</tr>"));
    
    client1->print(F("<tr>"));        
    client1->print(F("<td><input type=\"text\" name=\"agg0\" value=\"\"></td>"));
    client1->print(F("<td><input type=\"text\" name=\"agg1\" value=\"\"></td>"));
    client1->print(F("<td><input type=\"text\" name=\"agg2\" value=\"\"></td>"));
    client1->print(F("</tr>"));
    
    client1->print(F("<tr>"));
    client1->print(F("<td>"));
    client1->print(F("<input type=\"submit\" name=\"button\" value=\"Salva \">"));
    //client1->print(F("<input type=\"reset\" name=\"reset\" value=\"Cancella\">"));
    client1->print(F("</td>"));
    client1->print(F("</tr>"));
    
    client1->print(F("</table>")); 
    
    client1->print(F("</body>"));
    client1->print(F("</html>"));

}


//void admin1(EthernetClient* client1){
//              
//              client1->print(F("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"> <html>"));
//              client1->print(F("<head>"));
//              client1->print(F("   <meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">"));
//              client1->print(F("   <title>Admin</title>"));
//              client1->print(F("</head>"));
//              client1->print(F("<body>"));
//              client1->print(F("<table width=\"40%\" border=\"0\" align=\"center\" bgcolor=\"#e8e8e8\">"));
//              //client1->print(F("<tr align=\"center\">"));
//              //client1->print(F("<td><form action=\"index.htm\" method=\"get\">"));   //post
//              client1->print(F("<form action=\"index.htm\" method=\"get\">"));
//              
//              client1->print(F("<td>"));
//              client1->print(F("<font face=\"Arial\" size=\"2\"><b>mac</b></font>"));
//              client1->print(F("</td>"));
//              client1->print(F("<td>"));
//              //client1->print(F("<input type=\"text\" name=\"mac\">"));
//              client1->print(F("<font face=\"Arial\" size=\"2\"><b>"));
//              client1->print(mac[0],HEX);
//              client1->print(".");
//              client1->print(mac[1],HEX);
//              client1->print(".");
//              client1->print(mac[2],HEX);
//              client1->print(".");
//              client1->print(mac[3],HEX);
//              client1->print(".");
//              client1->print(mac[4],HEX);
//              client1->print(".");
//              client1->print(mac[5],HEX);
//
//              client1->print(F("</b></font>"));
//              client1->print(F("</td></tr>"));
//              
//              client1->print(F("<tr><td>"));
//              client1->print(F("<font face=\"Arial\" size=\"2\"><b>Indirizzo IP</b></font>"));
//              client1->print(F("</td>"));
//              client1->print(F("<td>"));
//              client1->print(F("<font face=\"Arial\" size=\"2\"><b>"));
//              client1->print(ip[0]);
//              client1->print(".");
//              client1->print(ip[1]);
//              client1->print(".");
//              client1->print(ip[2]);
//              client1->print(".");
//              client1->print(ip[3]);
//              client1->print(F("</b></font>"));
//              client1->print(F("</td></tr>"));
//              
//              client1->print(F("<tr><td>"));            
//              client1->print(F("<font face=\"Arial\" size=\"2\"><b>Subnet Mask</b></font>"));
//              //client1->print(F("<br>"));
//              client1->print(F("</td>"));
//              client1->print(F("<td>"));
//              client1->print(F("<font face=\"Arial\" size=\"2\"><b>"));		
//              client1->print(subnet[0]);
//              client1->print(".");
//              client1->print(subnet[1]);
//              client1->print(".");
//              client1->print(subnet[2]);
//              client1->print(".");
//              client1->print(subnet[3]);
//	      client1->print(F("</b></font>"));
//              client1->print(F("</td></tr>"));
//
//              
//              client1->print(F("<tr><td>"));
//              client1->print(F("<font face=\"Arial\" size=\"2\"><b>Gateway</b></font>"));
//              client1->print(F("</td>"));
//              client1->print(F("<td>"));
//	      client1->print(F("<font face=\"Arial\" size=\"2\"><b>"));
//              client1->print(gateway[0]);
//              client1->print(".");
//              client1->print(gateway[1]);
//              client1->print(".");
//              client1->print(gateway[2]);
//              client1->print(".");
//              client1->print(gateway[3]);
//              client1->print(F("</b></font>"));
//              client1->print(F("</td></tr>"));
//
//              client1->print(F("<tr><td>"));
//              client1->print(F("<font face=\"Arial\" size=\"2\"><b>dns</b></font>"));
//              client1->print(F("</td>"));
//              //client1->print(F("<br>"));
//              client1->print(F("<td>")); 
//              client1->print(F("<font face=\"Arial\" size=\"2\"><b>"));
//              client1->print(dns1[0]);
//              client1->print(".");
//              client1->print(dns1[1]);
//              client1->print(".");
//              client1->print(dns1[2]);
//              client1->print(".");
//              client1->print(dns1[3]);
//	      client1->print(F("</b></font>"));                           
//              client1->print(F("</td></tr>"));            
//
//              client1->print(F("<tr ALIGN=CENTER><td COLSPAN=2>"));
//              client1->print(F("<input type=\"submit\" name=\"button\" value=\"Salva\">"));
//
//              client1->print(F("<input type=\"reset\" name=\"reset\" value=\"Cancella\">"));
//              client1->print(F("</td></tr>"));
//              client1->print(F("</form>"));
//              client1->print(F("</table>"));
//              client1->print(F("</body>"));
//              client1->print(F("</html>")); 
//}
//

void ethad(EthernetClient* client1){   //         i controlli verranno fatti direttamente sulla pagina WEB i dati inviati saranno del tipo 192.016.001.130
  
client1->print(F("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"> <html>"));
client1->print(F("<head>"));
client1->print(F("   <meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">"));
client1->print(F("   <title>Configurazione Ethernet</title>"));
client1->print(F("</head>"));
client1->print(F("<body>"));
client1->print(F("<table width=\"40%\" border=\"0\" align=\"center\" bgcolor=\"#e8e8e8\">"));
client1->print(F("<tr align=\"center\">"));
client1->print(F("<td><form action=\"salvatag.htm\" method=\"get\">"));   //post 

client1->print(F("<font face=\"Arial\" size=\"2\"><b>mac</b></font>"));
//client1->print(F("<br>"));
client1->print(F("<input type=\"text\" name=\"mac0\" size=\"2\" maxlength=\"2\""));
client1->print(F("value="));
client1->print(mac[0],HEX);
client1->print(F(">"));
client1->print(F("."));
client1->print(F("<input type=\"text\" name=\"mac1\" size=\"2\" maxlength=\"2\""));
client1->print(F("value="));
client1->print(mac[1],HEX);
client1->print(F(">"));
client1->print(F("."));
client1->print(F("<input type=\"text\" name=\"mac2\" size=\"2\" maxlength=\"2\""));
client1->print(F("value="));
client1->print(mac[2],HEX);
client1->print(F(">"));
client1->print(F("."));
client1->print(F("<input type=\"text\" name=\"mac3\" size=\"2\" maxlength=\"2\""));
client1->print(F("value="));
client1->print(mac[3],HEX);
client1->print(F(">"));
client1->print(F("."));
client1->print(F("<input type=\"text\" name=\"mac4\" size=\"2\" maxlength=\"2\""));
client1->print(F("value="));
client1->print(mac[4],HEX);
client1->print(F(">"));
client1->print(F("."));
client1->print(F("<input type=\"text\" name=\"mac5\" size=\"2\" maxlength=\"2\""));
client1->print(F("value="));
client1->print(mac[5],HEX);
client1->print(F(">"));
client1->print(F("<br>"));

delay(200);
client1->print(F("<font face=\"Arial\" size=\"2\"><b>IP</b></font>"));
//client1->print(F("<br>"));
client1->print(F("<input type=\"text\" name=\"ip0\" size=\"3\" maxlength=\"3\""));
client1->print(F("value="));
client1->print(ip[0]);
client1->print(F(">"));
client1->print(F("."));
client1->print(F("<input type=\"text\" name=\"ip1\" size=\"3\" maxlength=\"3\""));
client1->print(F("value="));
client1->print(ip[1]);
client1->print(F(">"));
client1->print(F("."));
client1->print(F("<input type=\"text\" name=\"ip2\" size=\"3\" maxlength=\"3\""));
client1->print(F("value="));
client1->print(ip[2]);
client1->print(F(">"));
client1->print(F("."));
client1->print(F("<input type=\"text\" name=\"ip3\" size=\"3\" maxlength=\"3\""));
client1->print(F("value="));
client1->print(ip[3]);
client1->print(F(">"));
client1->print(F("<br>"));

delay(200);
client1->print(F("<font face=\"Arial\" size=\"2\"><b>Subnet Mask</b></font>"));
//client1->print(F("<br>"));
client1->print(F("<input type=\"text\" name=\"subnet0\" size=\"3\" maxlength=\"3\""));
client1->print(F("value="));
client1->print(subnet[0]);
client1->print(F(">"));
client1->print(F("."));
client1->print(F("<input type=\"text\" name=\"subnet1\" size=\"3\" maxlength=\"3\""));
client1->print(F("value="));
client1->print(subnet[1]);
client1->print(F(">"));
client1->print(F("."));
client1->print(F("<input type=\"text\" name=\"subnet2\" size=\"3\" maxlength=\"3\""));
client1->print(F("value="));
client1->print(subnet[2]);
client1->print(F(">"));
client1->print(F("."));
client1->print(F("<input type=\"text\" name=\"subnet3\" size=\"3\" maxlength=\"3\""));
client1->print(F("value="));
client1->print(subnet[3]);
client1->print(F(">"));
client1->print(F("<br>"));

delay(200);
client1->print(F("<font face=\"Arial\" size=\"2\"><b>Gateway</b></font>"));
//client1->print(F("<br>"));
client1->print(F("<input type=\"text\" name=\"gateway0\" size=\"3\" maxlength=\"3\""));
client1->print(F("value="));
client1->print(gateway[0]);
client1->print(F(">"));
client1->print(F("."));
client1->print(F("<input type=\"text\" name=\"gateway1\" size=\"3\" maxlength=\"3\""));
client1->print(F("value="));
client1->print(gateway[1]);
client1->print(F(">"));
client1->print(F("."));
client1->print(F("<input type=\"text\" name=\"gateway2\" size=\"3\" maxlength=\"3\""));
client1->print(F("value="));
client1->print(gateway[2]);
client1->print(F(">"));
client1->print(F("."));
client1->print(F("<input type=\"text\" name=\"gateway3\" size=\"3\" maxlength=\"3\""));
client1->print(F("value="));
client1->print(gateway[3]);
client1->print(F(">"));
client1->print(F("<br>"));

delay(200);
client1->print(F("<font face=\"Arial\" size=\"2\"><b>dns</b></font>"));
//client1->print(F("<br>"));
client1->print(F("<input type=\"text\" name=\"dns10\" size=\"3\" maxlength=\"3\""));
client1->print(F("value="));
client1->print(dns1[0]);
client1->print(F(">"));
client1->print(F("."));
client1->print(F("<input type=\"text\" name=\"dns11\" size=\"3\" maxlength=\"3\""));
client1->print(F("value="));
client1->print(dns1[1]);
client1->print(F(">"));
client1->print(F("."));
client1->print(F("<input type=\"text\" name=\"dns12\" size=\"3\" maxlength=\"3\""));
client1->print(F("value="));
client1->print(dns1[2]);
client1->print(F(">"));
client1->print(F("."));
client1->print(F("<input type=\"text\" name=\"dns13\" size=\"3\" maxlength=\"3\""));
client1->print(F("value="));
client1->print(dns1[3]);
client1->print(F(">"));
client1->print(F("<br>"));

delay(200);
client1->print(F("<br>"));
client1->print(F("<input type=\"submit\" name=\"button\" value=\"Salva\">"));
client1->print(F("<input type=\"reset\" name=\"reset\" value=\"Cancella\">"));
client1->print(F("</form></td>"));
client1->print(F("</tr>"));
client1->print(F("</table>"));
client1->print(F("</body>"));
client1->print(F("</html>")); 

}


void usrad(EthernetClient* client1, SdFat sd1){
  
    SdFile file;
    sd1.chdir(1);
    int c, ciclo=0;
    String Temporaneo="",campo="cmp";
    
    client1->print(F("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//IT\">"));
    client1->print(F("<head>"));
    client1->print(F("<title>Users</title>"));
    client1->print(F("</head>"));
    client1->print(F("<body>"));
    
    client1->print(F("<table  width=\"40%\" border=\"0\" align=\"center\" bgcolor=\"#e8e8e8\">"));
    client1->print(F("<form action=\"salvatag.htm\" method=\"get\">"));
    if(file.open("LOGIN.PWD", O_READ)){
                client1->print(F("<tr>"));
                client1->print(F("<th>Username</th>"));
                client1->print(F("<th>Password</th>"));
                client1->print(F("<th>Homepage</th>"));
                client1->print(F("</tr>"));
                client1->print(F("<tr>"));
		while ((c = file.read()) >= 0 ){          ////       10 = /n    44 = ,   61 = =   59 = ;  && c!=10 && ciclo<2   32 = space
                                Serial.print((char)c);
				Temporaneo+=(char)c;
                                  
                                if(c==32 || c==10){
                                   client1->print(F("<td>"));		                   
                                   client1->print(F("<input type=\"text\""));
                                   client1->print(F( "name=\""));
                                   campo="cmp";
                                   client1->print(campo+=ciclo);
                                   client1->print(F("\""));
                                   client1->print(F("value="));
                                   Serial.println(Temporaneo);
                                   client1->print(Temporaneo);
                                   client1->print(F(">"));
                                   client1->print(F("</td>"));
                                   Temporaneo="";
                                   ciclo++;
                                 }                                 
                                 if(c==10){                                  
                                   client1->print(F("</tr>"));
                                   client1->print(F("<br>"));
                                   client1->print(F("<tr>"));
                                   Temporaneo="";
                                 }
                }
                client1->print(F("<td>"));
                client1->print(F("<input type=\"submit\" name=\"button\" value=\"Salva \">"));
                //client1->print(F("<input type=\"reset\" name=\"reset\" value=\"Cancella\">"));
                client1->print(F("</form></td>"));
                client1->print(F("</tr>"));
                file.close();
    }
    client1->print(F("</table>")); 
    
    client1->print(F("</body>"));
    client1->print(F("</html>"));  

}

void statuscoderrautenticazione(EthernetClient* client1){////risposta di autenticazione errata    inizio
     String ipstring="";//ip[0]+"."+ip[1]+'.'+ip[2]+'.'+ip[3];
     for (int i=0; i<4 ; i++){
              ipstring+=ip[i];
              if(i!=3) ipstring+="."; 
    }
    client1->print(F("HTTP/1.0 401 Authorization Required")); 
    client1->print(F("ARDDUINO_SERVER_HEADER"));
    client1->print(F("WWW-Authenticate: Basic realm=\"Arduino\"")); 
    client1->print(F("Content-Type: text/html\""));  
    client1->print(F("<html><head>  <meta http-equiv=\"refresh\" content=\"5; url=http://"));
    client1->print(ipstring);
    client1->print(F("\"> </head><body><h1>401 Unauthorized.</h1></body></html>"));
}

void autenticazione(EthernetClient* client1){
  
client1->print(F("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"> <html>"));
client1->print(F("<head>"));
client1->print(F("   <meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">"));
client1->print(F("   <title>Login Utente</title>"));
client1->print(F("</head>"));
client1->print(F("<body>"));
client1->print(F("<table width=\"40%\" border=\"0\" align=\"center\" bgcolor=\"#e8e8e8\">"));
client1->print(F("<tr align=\"center\">"));
client1->print(F("<td><form action=\"index.htm\" method=\"get\">"));   //post 
client1->print(F("<font face=\"Arial\" size=\"2\"><b>Login</b></font>"));
client1->print(F("<br>"));
client1->print(F("<input type=\"text\" name=\"login\">"));
client1->print(F("<br>"));
client1->print(F("<font face=\"Arial\" size=\"2\"><b>Password</b></font>"));
client1->print(F("<br>"));
client1->print(F("<input type=\"password\" name=\"password\">"));
client1->print(F("<br>"));
client1->print(F("<br>"));
client1->print(F("<input type=\"submit\" name=\"button\" value=\"Entra\">"));
client1->print(F("<input type=\"reset\" name=\"reset\" value=\"Cancella\">"));
client1->print(F("</form></td>"));
client1->print(F("</tr>"));
client1->print(F("</table>"));
client1->print(F("</body>"));
client1->print(F("</html>")); 

}


//void filesusd(EthernetClient* client1, SdFat sd1){
//  
//    SdFile file;
//    sd1.chdir(1);
//    String Temporaneo="";
//    //Temporaneo=sd1.ls(LS_SIZE||LS_DATE||LS_R);
//    client1->print(F("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//IT\">"));
//    client1->print(F("<head>"));
//    client1->print(F("<title>Users</title>"));
//    client1->print(F("</head>"));
//    client1->print(F("<body>"));
//    do{	
//      sd1.ls(LS_SIZE||LS_DATE||LS_R);
//        //client1->print();
//    }while(Temporaneo);
//
//
//    client1->print(F("</body>"));
//    client1->print(F("</html>"));  
//
//}

void sessionescaduta(EthernetClient* client1){
    client1->print(F("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//IT\">"));
    client1->print(F("<head>"));
    client1->print(F("<title>Info</title>"));
    client1->print(F("</head>"));
    client1->print(F("<body>"));
    client1->print(F("Sessione scaduta"));
    client1->print(F("</body>"));
    client1->print(F("</html>"));
}

