/***** AudioNodes.h *****/

#ifndef AUDIONODES_H_INC
#define AUDIONODES_H_INC

#include <AudioNode.h>
#include <Bela.h>

class AudioSourceNode : public AudioNode {
	public:
		AudioSourceNode(int id) : AudioNode(id, 0, 1, 2, EXPLICIT_CCM){}
		
		void receiveInterleavedInput(BelaContext* ctx){
			//rt_printf("recieveInterleavedInput\n");
			for (int n=0; n<ctx->audioFrames; n++){
				for (int ch=0; ch<ctx->audioInChannels; ch++){
					//rt_printf("from %i to %i\n", n * ctx->audioInChannels + ch, ch * ctx->audioFrames + n);
					//rt_printf("before: %f to %f\n", ctx->audioIn[n * ctx->audioInChannels + ch], context.outputBuffer[ch * ctx->audioFrames + n]);
					context.outputBuffer[ch * ctx->audioFrames + n] = ctx->audioIn[n * ctx->audioInChannels + ch];
					//rt_printf("after: %f to %f\n", ctx->audioIn[n * ctx->audioInChannels + ch], context.outputBuffer[ch * ctx->audioFrames + n]);
				}
			}
			
			for (auto connection : outputConnections){
				connection.node->receiveInput(connection.input, this);
			}
		}
};

class AudioDestinationNode : public AudioNode {
	public:
		AudioDestinationNode(int id) : AudioNode(id, 1, 0, 2, EXPLICIT_CCM){}
		
		void setInterleavedOutput(BelaContext* ctx){
			//rt_printf("setInterleavedOutput\n");
			for (int n=0; n<ctx->audioFrames; n++){
				for (int ch=0; ch<ctx->audioOutChannels; ch++){
					// rt_printf("from %i to %i\n", ch * ctx->audioFrames + n, n * ctx->audioOutChannels + ch);
					//rt_printf("before: %f to %f\n", context.inputBuffer[ch * ctx->audioFrames + n], ctx->audioOut[n * ctx->audioOutChannels + ch]);
					ctx->audioOut[n * ctx->audioOutChannels + ch] = context.inputBuffer[ch * ctx->audioFrames + n];
					//rt_printf("after: %f to %f\n", context.inputBuffer[ch * ctx->audioFrames + n], ctx->audioOut[n * ctx->audioOutChannels + ch]);
				}
			}
		}
};

class ChannelSplitterNode : public AudioNode {
	public:
		ChannelSplitterNode(int id) : AudioNode(id, 1, 1, 1, MAX_CCM){}
		
		void render(AudioNodeContext* context){
			for (int i = 0; i<context->numOutputs; i++){
				context->outputBuffer[]
			}
		}
		
	protected:
		void changeNumChannels(int numChannels){
			
			inputChannels = numChannels;
			context.numChannels = numChannels;
			
			outputChannels = 1;
			context.numOutputs = numChannels;
			
			rt_printf("%i change input channels to %i\n", ID, numChannels);
			createBuffers();
		}
};

class GainNode : public AudioNode {
	public:
		GainNode(int id, float gain) : AudioNode(id, 1, 1, 1, MAX_CCM), gain(gain){}
		
		void render(AudioNodeContext* context){
			for (int n=0; n<context->numAudioFrames; n++){
				for (int ch=0; ch<context->numChannels; ch++){
					context->outputBuffer[n + context->numAudioFrames*ch] = context->inputBuffer[n + context->numAudioFrames*ch] * gain;
				}
			}
		}
		
	private:
		float gain;
};

#endif