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
	
	namespace midi
	{
		
		MidiTool::MidiTool()
		{
			midiNotes.clear();
			calculateNoteValues();
		}

		MidiTool::~MidiTool()
		{

		}

		/// <summary>
		/// set the sample rate for calculation, recalc if needed
		/// </summary>
		void MidiTool::setSampleRate(double rate)
		{
			if (sampleRate != rate)
			{
				sampleRate = rate;
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
			midiSamples.wholeNote = round((long double(60 * sampleRate / midiBpm * 4));
			midiSamples.halfNote = round(midiSamples.wholeNote / 2);
			midiSamples.quarterNote = round(midiSamples.wholeNote / 4);
			midiSamples.eighthNote = round(midiSamples.wholeNote / 8);
			midiSamples.sixteenthNote = round(midiSamples.wholeNote / 16);
			midiSamples.thirtySecondthNote = round(midiSamples.wholeNote / 32);
			midiSamples.sixtyFourthNote = round(midiSamples.wholeNote / 64);
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

			if (midiPlayState == (tool::PlayState::Playing))
			{

				//only if midi in is processed maybe latr for cc events

				/*while (midiIterator.getNextEvent(currentMessage, samplePos))
				{
					if ((currentMessage.isNoteOn()))
					{
						for (int k = 0; k < rbRepeatCount; k++)
						{
							rbNotes.append(new RbNote(currentMessage.getNoteNumber() + noteStep * k,
								currentMessage.getVelocity() * pow(veloDrop, k),
								rbSamplesPassed + samplePos + round(rbNoteValues.rbWholeNote / noteDiv * k),
								rbSamplesPassed + samplePos + round(rbNoteValues.rbWholeNote / noteDiv * k) +
								(round(rbNoteValues.rbWholeNote / noteDiv * noteDur))));
						}
					}
				}*/


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

					
					/// <summary>
					/// Adding notes to MidiBuffer if necessary / turning off notes also
					/// </summary>
					/// <param name="midiMessages"></param>
					/// <param name="samples"></param>
					for (int l = 0; l < midiNotes.size(); l++)
					{

						midiNotes.size();
						

						if ((samplesPassed + i) == (midiNotes.operator[](l).get()->rbStartSample))
						{
							auto message = juce::MidiMessage::noteOn(1, rbNotes.operator[](l).get()->rbNoteNumber, rbNotes.operator[](l).get()->rbNoteVelocity);
							myBuffer.addEvent(message, i);
						}

						if ((rbSamplesPassed + i) == (rbNotes.operator[](l).get()->rbEndSample))
						{
							auto message = juce::MidiMessage::noteOff(1, rbNotes.operator[](l).get()->rbNoteNumber, rbNotes.operator[](l).get()->rbNoteVelocity);
							myBuffer.addEvent(message, i);
						}
					}
				}

				midiMessages.swapWith(myBuffer);
				rbSamplesPassed += numSamples;

				for (int m = 0; m < rbNotes.size(); m++)
				{
					if ((rbSamplesPassed) > (rbNotes.operator[](m).get()->rbEndSample))
					{
						rbNotes.remove(rbNotes.operator[](m).get());
					}
				}
			}

			if (rbPlayState == (RbPlayState::Stopped))
			{
				auto message = MidiMessage::allNotesOff(1);
				midiMessages.addEvent(message, numSamples - 1);
			}
		}
		void ReBeater::reSet()
		{
			rbTransport.rbBar = 0;
			rbTransport.rbQuarter = 0;
			rbTransport.rbEighth = 0;
			rbTransport.rbSixteenth = 0;
			rbTransport.rbThirtySecondth = 0;
			rbTransport.rbSixtyfourth = 0;
			rbSamplesPassed = 0;
			rbPlayState = RbPlayState::Stopped;
			noteOn = false;
			rbNotes.deleteAll();
		}

		void ReBeater::Play()
		{
			rbPlayState = RbPlayState::Playing;
		}

		void ReBeater::Stop()
		{
			rbPlayState = RbPlayState::Stopped;
			reSet();
		}


	}
}
