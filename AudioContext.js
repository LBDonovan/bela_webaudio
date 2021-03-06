/***** AudioContext.js *****/
var EventEmitter = require('events').EventEmitter;
var spawn = require('child_process').spawn;
var osc = require('./AudioContextOSC');
var present = require('present');

var AudioNode = require('./AudioNode');
var AudioParam = require('./AudioParam');
var AudioDestinationNode = require('./AudioDestinationNode');
var GainNode = require('./GainNode');
var ChannelSplitterNode = require('./ChannelSplitterNode');
var ChannelMergerNode = require('./ChannelMergerNode');
var OscillatorNode = require('./OscillatorNode');

var IDcount = 0;

var _state = 'suspended', _startTime = 0;

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
		
		osc.on('start', timestamp => {
			_state = 'running';
			_startTime = timestamp;
			this.onstatechange();
		});
		
		Object.defineProperties(this, {
			
			'source': {
			    value: new AudioNode(this, IDcount++, 0, 1, 2, 'explicit'),
			    writable: false
			},
			
			'destination': {
			    value: new AudioDestinationNode(this, IDcount++),
			    writable: false
			},
			
			'state': {
			    get: () => _state
			},
			
			'onstatechange': {
				value: function(){},
				writable: true
			},
			
			'currentTime': {
				get: () => (present() - _startTime)/1000
			}
			
		});
		
	}
	
	createGain(gain){
		var node = new GainNode(
			this, 
			IDcount++, 
			new AudioParam(this, IDcount++, gain)
		);
		osc.createNode('GainNode', node);
		osc.createParam(node, node.gain);
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
	
	createOscillator(){
		var node = new OscillatorNode(
			this, 
			IDcount++, 
			new AudioParam(this, IDcount++, 440),	// frequency
			new AudioParam(this, IDcount++, 0)		// detune
		);
		osc.createNode('OscillatorNode', node);
		osc.createParam(node, node.frequency);
		osc.createParam(node, node.detune);
		return node;
	}
	
}

module.exports = AudioContext;