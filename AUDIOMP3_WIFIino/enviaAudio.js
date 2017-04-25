var net = require('net');
var client = new net.Socket();
var fs = require("fs");

client.connect(8090, '192.168.0.43', function() {
	console.log('Connected');
	client.write('teste');

	/*fs.readFile("1.wav", function (err, data) {
	    if (err) {
	    	throw err;
	    }
	    console.log('Enviando audio. ',data.length);
	    client.write('1');
	});*/
});

client.on('data', function(data) {
	console.log('Received: ' + data);
});

client.on('close', function() {
	console.log('Connection closed');
});