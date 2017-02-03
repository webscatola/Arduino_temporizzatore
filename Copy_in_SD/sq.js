$(document).ready(function(){
  $('#irrigazioneimmediata').hide();
  $('#progirrd').click(function(){
    $('#mainsettori').hide();
	$('#irrigazioneimmediata').hide();
  });
  $('#progirri').click(function(){
    $('#mainsettori').fadeOut();
	$('#irrigazioneimmediata').fadeIn();
  });
  $('#progirra').click(function(){
    $('#mainsettori').fadeIn();
	$('#irrigazioneimmediata').fadeOut();
  });

   $('#buttonreset').click(function(){
    $('#mainsettori').show();
	$('#irrigazioneimmediata').hide();
  });
    
  });









