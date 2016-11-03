/***** AudioParam.js *****/
var AudioNode = require('./AudioNode');
var osc = require('./AudioContextOSC');

class AudioParam extends AudioNode {
	
	constructor(context, ID, defaultVal){
		super(context, ID, 1, 0, 1, 'explicit');
		
		Object.defineProperty(this, 'defaultValue', {
		    value: defaultVal,
		    writable: false
		});
		
		var _value = defaultVal;
		Object.defineProperty(this, 'value', {
		    get: () => _value,
		    set: val => {
		    	_value = val;
		    	osc.setParamValue(this);
		    }
		});
		
	}
	
	connect(){
		console.log('cannot connect AudioParam');
	}
	disconnect(){
		console.log('cannot disconnect AudioParam');
	}
	
	setValueAtTime(value, time){
		osc.paramEvent(this, 0, value, time);
	}
	
}

module.exports = AudioParam;