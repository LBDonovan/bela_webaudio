/***** AudioNode.h *****/

#ifndef AUDIONODE_H_INC
#define AUDIONODE_H_INC

#include <vector>
#include <stddef.h>

#define MAX_CCM 0
#define CLAMPED_MAX_CCM 1
#define EXPLICIT_CCM 2

struct OutputConnection;

class AudioNode{
	public:
		AudioNode(int id, int inputs, int outputs, int channelCount, int channelCountMode, int numParams);
		
		void connectTo(AudioNode* node, int output, int input);
		
		void receiveInput(AudioNode* node, const OutputConnection* connection);
		
		void addParam(AudioNode* param);
		void receiveParam();
		
		virtual void resetInputs();
		
		int getID();
		
		virtual float getValueAt(int index);
		
	protected:
		
		int inputs;
		int outputs;
		
		int inputChannels;
		int outputChannels;
		
		int audioFrames;
		int sampleRate;
		
		std::vector<std::vector<std::vector<float>>> inputBuffer;
		std::vector<std::vector<std::vector<float>>> outputBuffer;
		
		virtual void render(){};
		
		void addInputConnection(int numChannels);
		
		int inputConnections;
		int inputConnectionsReceived;
		int paramsReceived;
		
		std::vector<OutputConnection> outputConnections;
		
		int ID;
		
		int channelCount;
		int channelCountMode;

		virtual void changeNumChannels(int numChannels);
		
		void createBuffers();
		virtual void process();
		
		void resetOutputs();
		
		std::vector<AudioNode*> params;
		
	private:
		bool printedMixing;
		void processParams();
		
};

struct OutputConnection{
	int output;
	AudioNode* node;
	int input;
};

#endif