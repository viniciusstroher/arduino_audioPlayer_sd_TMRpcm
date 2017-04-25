var net = require('net');
var client = new net.Socket();
var fs = require("fs");

function startServer(){
	try{
		var server = net.createServer(function(socket) {
				var connect_log = '[Log - '+new Date().toISOString()+']Conectando no servidor.\r\n';
				console.log(connect_log);
				socket.write(connect_log);
				
				socket.on('data', function (data) {
					console.log("OK",data);

						/*fs.readFile("1.wav", function (err, data) {
						    if (err) {
						    	throw err;
						    }
						    console.log('Enviando audio. ',data.length);
						    client.write('1');
						});*/
				});
		});
	server.listen(8090);
		}catch(ex){
			console.log("RESTART SERVER");
			startServer();
		}
	}
startServer();