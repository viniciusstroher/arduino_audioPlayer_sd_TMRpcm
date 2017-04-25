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
	var arraySlicePiece  = 1000;
	var pointerSlice 	 = 0;
	var piecesOFChunck 	 = 0;

	fs.readFile("1.wav", function (err, data) {
	    if (err) {
	    	throw err;
	    }

	    arrayBuffer = data;
	    piecesOFChunck = Math.ceil(arrayBuffer.length/arraySlicePiece);
	});

	try{
		var server = net.createServer(function(socket) {
				var connect_log = '[Log - '+new Date().toISOString()+']Conectando no servidor.\r\n';
				console.log('[Log]Connect: ',connect_log);

				/*for(i=0;i<piecesOFChunck;i++){
					console.log('chunck pointers',i*arraySlicePiece,(i*arraySlicePiece)+arraySlicePiece);
					var sliceChunck = arrayBuffer.slice(i*arraySlicePiece,(i*arraySlicePiece)+arraySlicePiece);
					console.log(sliceChunck);
					
					socket.write(sliceChunck);
				}*/
				
				//var i = 0;
				//var sliceChunck = arrayBuffer.slice(i*arraySlicePiece,(i*arraySlicePiece)+arraySlicePiece);
				
				socket.write('chunks:'+parseInt(piecesOFChunck));
				piecesOFChunck = parseInt(piecesOFChunck-1);

				socket.on('data', function (data){
					console.log('Data In: ',data.toString("utf8"));
				});

				socket.on('error', function (data) {
					console.log("Error: ",data);
				});
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
