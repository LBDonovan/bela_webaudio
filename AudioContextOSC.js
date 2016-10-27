/***** AudioContextOSC.js *****/
var EventEmitter = require('events').EventEmitter;
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

var socket;

var time = 0, lastTime = 0;

class AudioContextOSC extends EventEmitter{
	
	constructor(){
		super();
	}
	
	init(callback){
		
		// create an OSC socket for talking to Bela
		socket = dgram.createSocket('udp4');
		socket.bind(OSC_RECEIVE, '127.0.0.1', callback);
		
		socket.on('message', (message, info) => {
			
			var msg = osc.fromBuffer(message);
			
			// console.log('received OSC', msg);
			
			if (msg.oscType === 'message'){
				parseMessage(msg, this);
			} else if (msg.elements){
				for (let element of msg.elements){
					parseMessage(element, this);
				}
			}
			
		});
	}
	
	createNode(name, node){
		console.log('creating node', name, node.ID, (node.gain || node.channelCount));
		sendOSC({
			address : '/create-node/'+name,
			args	: [
				{
					type	: 'integer',
					value	: node.ID
				},
				{
					type	: 'integer',
					value	: (node.gain || node.channelCount)
				}
			]
		})
	}
	
	connectNodes(source, destination, output, input){
		console.log('connect nodes', source, destination, output, input);
		sendOSC({
			address	: '/connect-nodes',
			args	: [
				{
					type	: 'integer',
					value	: source
				},
				{
					type	: 'integer',
					value	: destination
				},
				{
					type	: 'integer',
					value	: output
				},
				{
					type	: 'integer',
					value	: input
				}
			]
		});
	}
}

module.exports = new AudioContextOSC();

function parseMessage(msg, emitter){
	
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
		emitter.emit('setup');
		if (preSetupBuffer.elements.length)
			sendOSC(preSetupBuffer);
	} else if (address[1] === 'osc-timestamp'){
		time = performance.now();
		console.log(lastTime - time);
		lastTime = time;
	}
}

function sendOSC(msg){
	if (belaSetup){
		console.log('sending OSC', msg);
		var buffer = osc.toBuffer(msg);
		socket.send(buffer, 0, buffer.length, OSC_SEND, '127.0.0.1', function(err) {
			if (err) console.log(err);
		});
	} else {
		preSetupBuffer.elements.push(msg);
	}
}