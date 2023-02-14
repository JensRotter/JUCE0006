/*
  ==============================================================================

	jeroMidi.h
	Created: 9 Feb 2023 1:07:16pm
	Author:  lil_jer

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace jero
{
	class MidiSequence
	{
	public:
		/// <summary>
		/// defines a single step for sequence aka a note
		/// </summary>
		struct SequenceStep
		{
			int pitch;
			int octave;
			int velocity;
			float duration;
			bool play;
			int probability;
			int pattern;
		};

		/// <summary>
		/// define a bar of a sequence
		/// </summary>
		struct SequenceBar
		{
			int numSteps;
			//SequenceStep barSteps[8];
			std::vector<SequenceStep> barSteps;
		};

		MidiSequence(int bars, int stepsPerBar);
		~MidiSequence();

		void makeBars(int bars, int steps);
		void makeSteps(int steps, SequenceBar* bar);
		void dbgSequence();

		int numBars = 4;
		int numStepsBar = 4;
		std::vector <SequenceBar> sequenceBars;
	};

	class MidiTool
	{
	public:
		struct MidiNote
		{
		public:
			int noteNumber;
			juce::uint8 noteVelocity;
			long startSample;
			long endSample;
		};

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

		/// <summary>
		/// Transport Info
		/// </summary>
		struct SequenceTransport
		{
			int sqBar = 0;
			int sqStep = 0;
		};

		/// <summary>
		/// storing samples per note division
		/// </summary>
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
			Stopped,
			Paused
		};

		Transport midiTransport;
		SequenceTransport sequenceTransport;
		SamplesPerDivision midiSamples;

		MidiTool();
		~MidiTool();

		void Play();
		void Stop();
		void Pause(int songPos);
		void setBpm(double bpm);
		void prepareToPlay(double rate, bool initFirst);
		void processNextBlock(juce::MidiBuffer& midiMessages, int samples);
		void cleanMidiNotes();

	private:
		double midiBpm{ 120 };
		double sampleRate{ 44100 };
		double samplesPerBar{ 88200 };
		long samplesPassed = 0;
		bool noteOn = false;
		int midiBars = 4;
		int midiSteps =4;
		
		SamplesPerDivision midiNoteValues;
		PlayState midiPlayState = PlayState::Stopped;
				
		jero::MidiSequence midiSequence = MidiSequence(midiBars, midiSteps);
		std::vector<MidiNote> midiNotes;

		void reSet();
		void calculateNoteValues();
	};

}

