/*
  ==============================================================================

	jeroMidi.h
	Created: 9 Feb 2023 1:07:16pm
	Author:  lil_jer

  ==============================================================================
*/

#pragma once
#include "jeroheader.h"



namespace jero
{
	namespace tool
	{
		/// <summary>
		/// Transport Info
		/// </summary>
		struct Transport
		{ 
			int trBar{ 0 };
			int trQuarter{ 0 };
			int trEighth{ 0 };
			int trSixteenth{ 0 };
			int trThirtySecondth{ 1 };
			int trSixtyfourth{ 1 };
		};


		struct SamplesPerDivision
		{
			long wholeNote{ 0 };
			long halfNote{ 0 };
			long quarterNote{ 0 };
			long eighthNote{ 0 };
			long sixteenthNote{ 0 };
			long thirtySecondthNote{ 0 };
			long sixtyFourthNote{ 0 };
		};

		enum PlayState
		{
			Playing,
			Stopped
		};
	}

	namespace midi
	{
		class MidiTool 
		{
		public:
			typedef struct MidiNote
			{
			public:
				int noteNumber;
				juce::uint8 noteVelocity;
				long startSample;
				long endSample;

				//removed for now needs no constructor
				/*midiNote(int rbNn, uint8 rbNv, long rbSs, long rbEs) : rbNoteNumber(rbNn), rbNoteVelocity(rbNv), rbStartSample(rbSs), rbEndSample(rbEs)
				{

				}*/
			} MidiNote;

			/// <summary>
			/// defines a single step for sequence aka a note
			/// </summary>
			typedef struct SequenceStep
			{
				int pitch;
				int octave;
				int velocity;
				float duration; // duration * notelength = real duration
				bool play;

				/* for later use
				int probability; 
				int pattern; play one note of a pattern every occurance of this step */
			};

			/// <summary>
			/// define a bar of a sequence
			/// </summary>
			typedef struct SequenceBar
			{
				int numSteps;
				SequenceStep barSteps[8];
			};


			/// <summary>
			/// do i  need this??? or can I use notenumbers / length
			/// </summary>
			typedef struct SongPosition
			{
				int bar;
				int quarter;
			};

			tool::Transport midiTransport;
			tool::SamplesPerDivision midiSamples;

			MidiTool();
			~MidiTool();
			void prepareToPlay(double rate,int samplesPerBlock);
			void setBpm(double bpm);
			void Play();
			void Stop();
			void processNextBlock(juce::MidiBuffer& midiMessages, int samples);

		private:
			double midiBpm{ 120 };
			double sampleRate{ 44100 };
			double samplesPerBar{ 88200 };
			long samplesPassed = 0;
			bool noteOn = false;

			void reSet();	
			void calculateNoteValues();

			tool::SamplesPerDivision midiNoteValues;
			tool::PlayState midiPlayState = tool::PlayState::Stopped;
			SongPosition mySongPosition;
			SequenceBar myBar;
			std::vector<MidiNote> midiNotes;
			
		};

	}
}
