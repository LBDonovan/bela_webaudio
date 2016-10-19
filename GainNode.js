/***** GainNode.js *****/
var AudioNode = require('./AudioNode');

class GainNode extends AudioNode {
	
	constructor(context, ID, gain){
		
		super(context, ID, 1, 1, 1, 'max');
		
		this.gain = gain;
		
	}
	
}

module.exports = GainNode;