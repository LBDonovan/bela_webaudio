/***** AudioNode.js *****/
var EventEmitter = require('events').EventEmitter;
var osc = require('./AudioContextOSC');

class AudioNode extends EventEmitter {
	
	constructor(context, ID, inputs, outputs, _channelCount, _channelCountMode){
		
		super();
		
		Object.defineProperty(this, 'context', {
		    value: context,
		    writable: false
		});
		
		Object.defineProperty(this, 'ID', {
		    value: ID,
		    writable: false
		});
		
		Object.defineProperty(this, 'numberOfInputs', {
		    value: inputs,
		    writable: false
		});
		
		Object.defineProperty(this, 'numberOfOutputs', {
		    value: outputs,
		    writable: false
		});
		
		this.channelCount = _channelCount;
		this.channelCountMode = _channelCountMode;
		
	}
	
	connect(destination, output, input){
		
		output = (output || 0);
		input = (input || 0);
		
		if (destination instanceof AudioNode){
			osc.connectNodes(this.ID, destination.ID, output, input);
		}
		
	}
	
	disconnect(destination, output, input){
		
	}
	
}

module.exports = AudioNode;