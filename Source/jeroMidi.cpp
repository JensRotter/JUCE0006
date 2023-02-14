/*
  ==============================================================================

	jeroMidi.cpp
	Created: 9 Feb 2023 1:07:16pm
	Author:  lil_jer

  ==============================================================================
*/

#include "jeroMidi.h"

namespace jero
{
	MidiSequence::MidiSequence(int bars, int stepsPerBar)
	{
		numBars = bars;
		numStepsBar = stepsPerBar;
		makeBars(bars, numStepsBar);

	}


	MidiSequence::~MidiSequence()
	{

	}

	void MidiSequence::dbgSequence()
	{
		for (auto i = 0; i < sequenceBars.size(); i++)
		{
			DBG(sequenceBars.at(i).numSteps);
			for (auto k = 0; k < sequenceBars.at(i).numSteps; k++)
			{
				DBG(sequenceBars.at(i).barSteps.at(k).pitch);
			}
		}
	}
	void MidiSequence::makeBars(int bars, int steps)
	{
		for (auto i = 0; i < bars; i++)
		{
			SequenceBar newBar;
			newBar.numSteps = i + 1;
			makeSteps(steps, &newBar);
			sequenceBars.push_back(newBar);
		}
	}

	void MidiSequence::makeSteps(int steps, SequenceBar* bar)
	{
		for (auto k = 0; k < bar->numSteps; k++)
		{
			SequenceStep newStep;
			newStep.duration = 0.5f;
			newStep.octave = 2;
			newStep.pattern = 0;
			newStep.pitch = 48;
			newStep.play = true;
			newStep.probability = 1;
			newStep.velocity = 100;

			bar->barSteps.push_back(newStep);
		}
	}

	MidiTool::MidiTool()
	{
		midiNotes.clear();
	}

	MidiTool::~MidiTool()
	{

	}

	/// <summary>
	/// set the sample rate for calculation, recalc if needed
	/// </summary>
	void MidiTool::prepareToPlay(double rate, bool initFirst)
	{
		if (sampleRate != rate)
		{
			sampleRate = rate;
			if (!initFirst)
				calculateNoteValues();
		}
	}

	/// <summary>
	/// set the bpm for calculation, recalc if needed
	/// </summary>
	void MidiTool::setBpm(double bpm)
	{
		if (midiBpm != bpm)
		{
			midiBpm = bpm;
			calculateNoteValues();
		}
	}

	/// <summary>
	/// Calculate samples per every Element we can think of
	/// </summary>
	void MidiTool::calculateNoteValues()
	{
		midiNoteValues.wholeNote = round((long double(60 * sampleRate / midiBpm * 4)));
		midiNoteValues.halfNote = round(midiNoteValues.wholeNote / 2);
		midiNoteValues.quarterNote = round(midiNoteValues.wholeNote / 4);
		midiNoteValues.eighthNote = round(midiNoteValues.wholeNote / 8);
		midiNoteValues.sixteenthNote = round(midiNoteValues.wholeNote / 16);
		midiNoteValues.thirtySecondthNote = round(midiNoteValues.wholeNote / 32);
		midiNoteValues.sixtyFourthNote = round(midiNoteValues.wholeNote / 64);
	}

	///<summary>
	///Process audio, count samples
	///</summary>
	void MidiTool::processNextBlock(juce::MidiBuffer& midiMessages, int samples)
	{
		juce::MidiBuffer::Iterator midiIterator(midiMessages);
		juce::MidiBuffer myBuffer;
		juce::MidiMessage currentMessage;
		int samplePos;
		int numSamples = samples;

		if (midiPlayState == (PlayState::Playing))
		{
			/// <summary>
			/// keeping track of bar and song position
			/// </summary>
			/// <param name="midiMessages"></param>
			/// <param name="samples"></param>
			for (int i = 0; i < numSamples; i++)
			{

				if (((samplesPassed + i) % midiNoteValues.wholeNote) == 0)
				{
					midiTransport.trBar++;
				}

				if (((samplesPassed + i) % midiNoteValues.quarterNote) == 0)
				{
					midiTransport.trQuarter++;
					if (midiTransport.trQuarter > 4)
					{
						midiTransport.trQuarter = 1;
					}
				}

				if (((samplesPassed + i) % midiNoteValues.eighthNote) == 0)
				{
					midiTransport.trEighth++;
					if (midiTransport.trEighth > 8)
					{
						midiTransport.trEighth = 1;
					}
				}

				if (((samplesPassed + i) % midiNoteValues.sixteenthNote) == 0)
				{
					midiTransport.trSixteenth++;
					if (midiTransport.trSixteenth > 16)
					{
						midiTransport.trSixteenth = 1;
					}
				}

				if (((samplesPassed + i) % midiNoteValues.thirtySecondthNote) == 0)
				{
					midiNoteValues.thirtySecondthNote++;
					if (midiTransport.trThirtySecondth > 32)
					{
						midiTransport.trThirtySecondth = 1;
					}
				}

				if (((samplesPassed + i) % midiNoteValues.sixtyFourthNote) == 0)
				{
					midiTransport.trSixtyfourth++;
					if (midiTransport.trSixtyfourth > 64)
					{
						midiTransport.trSixtyfourth = 1;
					}
				}

				auto midiDivision = round(midiSequence.sequenceBars[sequenceTransport.sqBar].numSteps);


				int midiStepLength = round(midiNoteValues.wholeNote / midiDivision);

				if (((samplesPassed + i) % (midiStepLength)) == 0)
				{
					if (midiSequence.sequenceBars[sequenceTransport.sqBar].barSteps[sequenceTransport.sqStep].play)
					{
						MidiNote newNote;
						newNote.noteNumber = midiSequence.sequenceBars[sequenceTransport.sqBar].barSteps[sequenceTransport.sqStep].pitch;
						newNote.noteVelocity = midiSequence.sequenceBars[sequenceTransport.sqBar].barSteps[sequenceTransport.sqStep].velocity;
						newNote.startSample = samplesPassed + i;
						newNote.endSample = samplesPassed + i + (round((midiStepLength) * midiSequence.sequenceBars[sequenceTransport.sqBar].barSteps[sequenceTransport.sqStep].duration));
						midiNotes.push_back(newNote);
					}

					sequenceTransport.sqStep++;

					if (sequenceTransport.sqStep > midiSequence.sequenceBars[sequenceTransport.sqBar].numSteps-1)
					{
						sequenceTransport.sqStep = 0;
						sequenceTransport.sqBar++;
						if (sequenceTransport.sqBar > midiSequence.numBars-1)
							sequenceTransport.sqBar = 0;
					}

					/*DBG("Bar");
					DBG(sequenceTransport.sqBar);
					DBG("Tr Bar");
					DBG(midiTransport.trBar);
					DBG("Step");
					DBG(sequenceTransport.sqStep);*/
				}

			
				/// <summary>
				/// Adding notes to MidiBuffer if necessary / turning off notes also
				/// </summary>
				/// <param name="midiMessages"></param>
				/// <param name="samples"></param>
				for (int l = 0; l < midiNotes.size(); l++)
				{
					if ((samplesPassed + i) == (midiNotes.at(l).startSample))
					{
						auto message = juce::MidiMessage::noteOn(1, midiNotes.at(l).noteNumber, midiNotes.at(l).noteVelocity);
						myBuffer.addEvent(message, i);
					/*	DBG("i");
						DBG(i);
						DBG("Samples passed:");
						DBG(samplesPassed);
						DBG("Startsample");
						DBG(midiNotes.at(l).startSample);
						DBG("end sample");
						DBG(midiNotes.at(l).endSample);
						DBG("pitch");
						DBG(midiNotes.at(l).noteNumber);*/
						
					}

					if ((samplesPassed + i) == (midiNotes.at(l).endSample))
					{
						auto message = juce::MidiMessage::noteOff(1, midiNotes.at(l).noteNumber, midiNotes.at(l).noteVelocity);
						myBuffer.addEvent(message, i);
					}
				}
			}

			midiMessages.swapWith(myBuffer);
			samplesPassed += numSamples;
		}

		if (midiPlayState != (PlayState::Playing))
		{
			auto message = juce::MidiMessage::allNotesOff(1);
			midiMessages.addEvent(message, numSamples - 1);
		}
	}

	void MidiTool::cleanMidiNotes()
	{
		for (auto x = 0; x < midiNotes.size(); x++)
		{
			if (samplesPassed > midiNotes.at(x).startSample)
				midiNotes.erase(midiNotes.begin() + x);

			if (samplesPassed > midiNotes.at(x).endSample)
				midiNotes.erase(midiNotes.begin() + x);
		}
	}

	void MidiTool::reSet()
	{
		midiTransport.trBar = 0;
		midiTransport.trQuarter = 0;
		midiTransport.trEighth = 0;
		midiTransport.trSixteenth = 0;
		midiTransport.trThirtySecondth = 0;
		midiTransport.trSixtyfourth = 0;
		samplesPassed = 0;
		noteOn = false;
		midiNotes.clear();
	}

	void MidiTool::Play()
	{
		if (midiPlayState == PlayState::Paused)
			midiPlayState = PlayState::Playing;

		if (midiPlayState == PlayState::Stopped)
			midiPlayState = PlayState::Playing;
	}

	void MidiTool::Stop()
	{
		midiPlayState = PlayState::Stopped;
		reSet();
	}

	void MidiTool::Pause(int songPos)
	{
		midiPlayState = PlayState::Paused;
	}
}
