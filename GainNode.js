/***** GainNode.js *****/
var AudioNode = require('./AudioNode');

class GainNode extends AudioNode {
	
	constructor(context, ID, param){
		
		super(context, ID, 1, 1, 1, 'max');
		
		Object.defineProperty(this, 'gain', {
		    value: param,
		    writable: false
		});
		
	}
	
}

module.exports = GainNode;