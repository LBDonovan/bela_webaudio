/***** AudioContext.js *****/
var EventEmitter = require('events').EventEmitter;
var spawn = require('child_process').spawn;
var osc = require('./AudioContextOSC');

var AudioNode = require('./AudioNode');
var AudioDestinationNode = require('./AudioDestinationNode');
var GainNode = require('./GainNode');
var ChannelSplitterNode = require('./ChannelSplitterNode');
var ChannelMergerNode = require('./ChannelMergerNode');

var IDcount = 0;

class AudioContext extends EventEmitter {
	
	constructor(){
		
		super();
		
		// start the osc connection to Bela
		osc.init( () => {
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
		});
		
		// create the AudioSourceNode
		Object.defineProperty(this, 'source', {
		    value: new AudioNode(this, IDcount++, 0, 1, 2, 'explicit'),
		    writable: false
		});
		
		// create the AudioDestinationNode
		Object.defineProperty(this, 'destination', {
		    value: new AudioDestinationNode(this, IDcount++),
		    writable: false
		});
		
	}
	
	createGain(gain){
		var node = new GainNode(this, IDcount++, gain);
		osc.createNode('GainNode', node);
		return node;
	}
	
	createChannelSplitter(numberOfOutputs){
		var node = new ChannelSplitterNode(this, IDcount++, numberOfOutputs);
		osc.createNode('ChannelSplitterNode', node);
		return node;
	}
	
	createChannelMerger(numberOfInputs){
		var node = new ChannelMergerNode(this, IDcount++, numberOfInputs);
		osc.createNode('ChannelMergerNode', node);
		return node;
	}
	
}

module.exports = AudioContext;