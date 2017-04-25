var net = require('net');
var client = new net.Socket();
var fs = require("fs");

function toBuffer(ab) {
    var buf = new Buffer(ab.byteLength);
    var view = new Uint8Array(ab);
    for (var i = 0; i < buf.length; ++i) {
        buf[i] = view[i];
    }
    return buf;
}

function toArrayBuffer(buf) {
    var ab = new ArrayBuffer(buf.length);
    var view = new Uint8Array(ab);
    for (var i = 0; i < buf.length; ++i) {
        view[i] = buf[i];
    }
    return ab;
}

function startServer(){
	
	var arrayBuffer 	 = null;
	var arraySlicePiece  = 128;
	var pointerSlice 	 = 0;
	var piecesOFChunck 	 = 0;
	var piecesIncrement  = 0;
	var start 			 = false;

	var piecesOFChunckTotal = 0;
	var download 			= true;

	fs.readFile("1.wav", function (err, data) {
	    if (err) {
	    	throw err;
	    }

	    arrayBuffer 		 = data;
	    piecesOFChunck  	 = Math.ceil(arrayBuffer.length/arraySlicePiece);
	    piecesOFChunckTotal  = Math.ceil(arrayBuffer.length/arraySlicePiece);
	});

	try{
		var server = net.createServer(function(socket) {
				if(download){

					var connect_log = '[Log - '+new Date().toISOString()+']Conectando no servidor.\r\n';
					console.log(connect_log);

					if(!start){
						var start_log = '[Log - '+new Date().toISOString()+']Start record audio arduino.\r\n';
						console.log(start_log);

						start = true;
						socket.write('start');

					}else if(start && piecesOFChunck > 0){
						var chunk_log = '[Log - '+new Date().toISOString()+']Send chunk to '+piecesIncrement+'/'+piecesOFChunckTotal+' arduino.\r\n';
						console.log(chunk_log);

						//socket.write('chunks:'+parseInt(piecesOFChunck));
						var sliceChunck = arrayBuffer.slice(piecesIncrement*arraySlicePiece,(piecesIncrement*arraySlicePiece)+arraySlicePiece);
						socket.write(sliceChunck);

						piecesOFChunck   = parseInt(piecesOFChunck-1);
						piecesIncrement +=1;

					}else{
						var stop_log = '[Log - '+new Date().toISOString()+']Stop record , audio sent.\r\n';
						console.log(stop_log);

						socket.write('stop');
						start = false;
						download = false;

						client.end();
					}

					socket.on('data', function (data){
						console.log('Data In: ',data.toString("utf8"));
					});

					socket.on('error', function (data) {
						console.log("Error: ",data);
					});
				}
		});

		server.listen(8090);

	}catch(ex){
		console.log("RESTART SERVER");
		startServer();
	}
}
startServer();
/*
/*File myFile = SD.open(audios+".wav", FILE_WRITE);
         if (myFile) {
            for(uint32_t i = 0; i < len; i++) {
             myFile.write(buffer[i]); 
            }
            audios +=1;
            myFile.close();
            
         }*/
/*
  /*
        
        String paramName = str.substring(0,str.indexOf(":"));
        Serial.println(paramName);
        if(paramName.equals("start")){
          String paramValue = str.substring(str.indexOf(":")+1,-1);
          chunks = paramValue.toInt();
          
          if(chunks == 0){
            
          }else{
          
          }
          
        }
        
      }*/