/***** OscillatorNode.js *****/
var AudioNode = require('./AudioNode');
var osc = require('./AudioContextOSC');

class OscillatorNode extends AudioNode {
	
	constructor(context, ID, frequencyParam, detuneParam){
		
		super(context, ID, 0, 1, 1, 'max');
		
		Object.defineProperty(this, 'frequency', {
		    value: frequencyParam,
		    writable: false
		});
		
		Object.defineProperty(this, 'detune', {
		    value: detuneParam,
		    writable: false
		});
		
		var _type;
		Object.defineProperty(this, 'type', {
		    get: () => _type,
		    set: (val) => {
		    	switch (val) {
					case "sine":
						_type = 0;
						break;
		    		case "square":
						_type = 1;
						break;
					case "sawtooth":
						_type = 2;
						break;
					case "triangle":
						_type = 3;
						break;
					case "custom":
						_type = 4;
						break;
					default:
						console.warn('bad oscillator type', val);
						return;
		    	}
		    	osc.setOscillatorType(this, _type);
		    }
		});
		
	}
	
	start(){
		osc.setOscillatorState(this, 1);
	}
	
	stop(){
		osc.setOscillatorState(this, 0);
	}
	
}

module.exports = OscillatorNode;