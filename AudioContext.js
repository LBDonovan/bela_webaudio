/***** AudioContext.js *****/
var EventEmitter = require('events').EventEmitter;
var spawn = require('child_process').spawn;
var dgram = require('dgram');
var osc = require('osc-min');

// port numbers
var OSC_RECEIVE = 3426;
var OSC_SEND = 3427;

var belaSetup = false;
var preSetupBuffer = {
	oscType 	: 'bundle',
	elements	: []
};

// socket to send and receive OSC messages from bela
var socket;

function parseMessage(msg){

	var address = msg.address.split('/');
	if (!address || !address.length || address.length <2){
		console.log('bad OSC address', address);
		return;
	}
	
	// setup handshake
	if (address[1] === 'osc-setup'){
		console.log('setup recieved');
		sendOSC({ address : '/osc-setup-reply' });
		belaSetup = true;
		if (preSetupBuffer.elements.length)
			sendOSC(preSetupBuffer);
	}
}

function sendOSC(msg){
	if (belaSetup){
		var buffer = osc.toBuffer(msg);
		socket.send(buffer, 0, buffer.length, OSC_SEND, '127.0.0.1', function(err) {
			if (err) console.log(err);
		});
	} else {
		preSetupBuffer.elements.push(msg);
	}
}

class AudioContext extends EventEmitter {
	
	constructor(){
		
		super();
		
		// spawn the Bela process
		var proc = spawn('stdbuf', ['-i0', '-e0', '-o0', 'make', '-C', '/root/Bela/', 'run', 'PROJECT=web_audio']);
		proc.stdout.setEncoding('utf-8');
		proc.stdout.on('data', data => console.log('Bela: ', data) );
		proc.stderr.setEncoding('utf-8');
		proc.stderr.on('data', data => console.log('stderr: ', data) );
		proc.on('close', () => {
			console.log('closing');
			process.exit();
		});
		
		// create an OSC socket for talking to Bela
		socket = dgram.createSocket('udp4');
		socket.bind(OSC_RECEIVE, '127.0.0.1');
		
		socket.on('message', (message, info) => {

			var msg = osc.fromBuffer(message);
			
			if (msg.oscType === 'message'){
				parseMessage(msg);
			} else if (msg.elements){
				for (let element of msg.elements){
					parseMessage(element);
				}
			}
			
		});

	}
}

module.exports = AudioContext;