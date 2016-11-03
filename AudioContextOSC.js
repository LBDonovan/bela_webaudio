/***** AudioContextOSC.js *****/
var EventEmitter = require('events').EventEmitter;
var dgram = require('dgram');
var osc = require('osc-min');
var present = require('present');

// port numbers
var OSC_RECEIVE = 3426;
var OSC_SEND = 3427;

var belaSetup = false;
var preSetupBuffer = {
	oscType 	: 'bundle',
	elements	: []
};

var socket;

var time = 0, lastTime = 0, startTime = 0;

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
		console.log('creating node', name, node.ID);
		sendOSC({
			address : '/create-node/'+name,
			args	: [
				{
					type	: 'integer',
					value	: node.ID
				},
				{
					type	: 'integer',
					value	: node.channelCount
				}
			]
		})
	}
	
	createParam(parent, param){
		console.log('creating param', param.ID, parent.ID);
		sendOSC({
			address: '/create-param',
			args: [
				{
					type	: 'integer',
					value	: param.ID
				},
				{
					type	: 'integer',
					value	: parent.ID
				},
				{
					type	: 'float',
					value	: param.value
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
	
	setParamValue(param){
		console.log('setting param', param.ID, 'to value', param.value);
		sendOSC({
			address	: '/set-param',
			args	: [
				{
					type	: 'integer',
					value	: param.ID
				},
				{
					type	: 'float',
					value	: param.value
				}
			]
		});
	}
	
	setOscillatorType(node, type){
		console.log('setting oscillator type to', type);
		sendOSC({
			address : '/set-osc-type',
			args	: [
				{
					type	: 'integer',
					value	: node.ID
				},
				{
					type	: 'integer',
					value	: type
				}
			]
		})
	}
	
	setOscillatorState(node, state){
		console.log('setting oscillator state to', state);
		sendOSC({
			address : '/set-osc-state',
			args	: [
				{
					type	: 'integer',
					value	: node.ID
				},
				{
					type	: 'integer',
					value	: state
				}
			]
		})
	}
	
	setParamValueAtTime(param, value, time){
		console.log('setting param', param.ID, 'to value', value, 'at time', time);
		sendOSC({
			address : '/set-param-value-at-time',
			args	: [
				{
					type	: 'integer',
					value	: param.ID
				},
				{
					type	: 'float',
					value	: value
				},
				{
					type	: 'float',
					value	: time
				}
			]
		})
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
		if (msg.args[0].value == 1){
			startTime = present();
			emitter.emit('start', startTime);
		} else if (Math.abs(msg.args[0].value/44100 - (present()-startTime)/1000) > 0.1){
			console.log('Timestamp latency:', msg.args[0].value/44100 - (present()-startTime)/1000);
		}
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