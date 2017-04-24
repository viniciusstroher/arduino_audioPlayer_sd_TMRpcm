
var net = require('net');

var client = new net.Socket();
var fs = require("fs");


client.connect(8090, '192.168.0.43', function() {
	console.log('Connected');

	fs.readFile("1.wav", function (err, data) {
	    if (err) {
	    	throw err;
	    }
	    
	    console.log('Enviando audio.',data);
	    client.write(data);
	});

	
});

client.on('data', function(data) {
	console.log('Received: ' + data);
	client.destroy(); // kill client after server's response
});

client.on('close', function() {
	console.log('Connection closed');
});