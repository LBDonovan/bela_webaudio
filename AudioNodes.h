/***** AudioNodes.h *****/

#ifndef AUDIONODES_H_INC
#define AUDIONODES_H_INC

#include <AudioNode.h>
#include <Bela.h>

class AudioSourceNode : public AudioNode {
	public:
		AudioSourceNode(int id) : AudioNode(id, 0, 1, 2, EXPLICIT_CCM){}
		
		void receiveInterleavedInput(BelaContext* ctx){
			for (int n=0; n<ctx->audioFrames; n++){
				for (int ch=0; ch<ctx->audioInChannels; ch++){
					outputBuffer[0][ch][n] = ctx->audioIn[n * ctx->audioInChannels + ch];
				}
			}
			
			for (const auto connection : outputConnections){
				connection.node->receiveInput(this, &connection);
			}
		}
};

class AudioDestinationNode : public AudioNode {
	public:
		AudioDestinationNode(int id) : AudioNode(id, 1, 0, 2, EXPLICIT_CCM){}
		
		void setInterleavedOutput(BelaContext* ctx){
			for (int n=0; n<ctx->audioFrames; n++){
				for (int ch=0; ch<ctx->audioOutChannels; ch++){
					ctx->audioOut[n * ctx->audioOutChannels + ch] = inputBuffer[0][ch][n];
				}
			}
		}
};

// class ChannelSplitterNode : public AudioNode {
// 	public:
// 		ChannelSplitterNode(int id) : AudioNode(id, 1, 1, 1, MAX_CCM){}
		
// 		void render(){
// 			for (int i = 0; i<context->numOutputs; i++){
// 				context->outputBuffer[]
// 			}
// 		}
		
// 	protected:
// 		void changeNumChannels(int numChannels){
			
// 			inputChannels = numChannels;
// 			context.numChannels = numChannels;
			
// 			outputChannels = 1;
// 			context.numOutputs = numChannels;
			
// 			rt_printf("%i change input channels to %i\n", ID, numChannels);
// 			createBuffers();
// 		}
// };

class GainNode : public AudioNode {
	public:
		GainNode(int id, float gain) : AudioNode(id, 1, 1, 1, MAX_CCM), gain(gain){}
		
		void render(){
			for (int i = 0; i<inputs; i++){
				for (int ch = 0; ch<inputChannels; ch++){
					for (int f = 0; f<audioFrames; f++){
						outputBuffer[i][ch][f] = inputBuffer[i][ch][f] * gain;
					}
				}
			}
		}
		
	private:
		float gain;
};

#endif