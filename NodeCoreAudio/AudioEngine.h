//////////////////////////////////////////////////////////////////////////////
//
// AudioEngine.cpp: Core audio functionality
//
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "portaudio.h"
#include <v8.h>
#include <vector>
#include <node_object_wrap.h>
#include <nan.h>
using namespace v8; using namespace std;

#define DEFAULT_SAMPLE_RATE         (44100)
#define DEFAULT_SAMPLE_FORMAT      paFloat32
#define DEFAULT_FRAMES_PER_BUFFER  (256)
#define DEFAULT_NUM_BUFFERS        (8)

//#include <complex.h>
#include <fftw3.h>
#include <string>
#include "WindowFunction.h"

namespace Audio {

	//////////////////////////////////////////////////////////////////////////
	//! Core audio functionality
	class AudioEngine : public node::ObjectWrap {
	public:

		AudioEngine( Local<Object> options );

		//! Initialize our node object
		static void Init(v8::Handle<v8::Object> target);
		//! Create a new instance of the audio engine
		//static v8::Handle<v8::Value> NewInstance(const v8::Arguments& args);
        static NAN_METHOD(NewInstance);

		Isolate* GetIsolate() { return m_pIsolate; }
		Locker* GetLocker() { return m_pLocker; }

		//! Run the main blocking audio loop
		void RunAudioLoop();

		~AudioEngine();		//!< OOL Destructor

	private:
		static v8::Persistent<v8::Function> constructor;
		//static v8::Handle<v8::Value> New( const v8::Arguments& args );	//!< Create a v8 object
        static NAN_METHOD(New);

		//! Returns whether the PortAudio stream is active
		//static v8::Handle<v8::Value> isActive( const v8::Arguments& args );
        static NAN_METHOD(isActive);
		//! Get the name of an audio device with a given ID number
		//static v8::Handle<v8::Value> getDeviceName( const v8::Arguments& args );
        static NAN_METHOD(getDeviceName);
        //! Get the maxInputChannels of an audio device with a given ID number
        static NAN_METHOD(getDeviceMaxInputChannels);
        //! Get the maxOutputChannels of an audio device with a given ID number
        static NAN_METHOD(getDeviceMaxOutputChannels);
		//! Get the number of available devices
		//static v8::Handle<v8::Value> getNumDevices( const v8::Arguments& args );
        static NAN_METHOD(getNumDevices);

		//! Closes and reopens the PortAudio stream
		void restartStream();

		//static v8::Handle<v8::Value> write( const v8::Arguments& args );		//!< Write samples to the current audio device
        static NAN_METHOD(write);
		//static v8::Handle<v8::Value> read( const v8::Arguments& args );			//!< Read samples from the current audio device
        static NAN_METHOD(read);
		//static v8::Handle<v8::Value> isBufferEmpty( const v8::Arguments& args );	//!< Returns whether the data buffer is empty
        static NAN_METHOD(isBufferEmpty);

		//static v8::Handle<v8::Value> setOptions( const v8::Arguments& args );	//!< Set options, restarts audio stream
        static NAN_METHOD(setOptions);
		//static v8::Handle<v8::Value> getOptions( const v8::Arguments& args );	//!< Gets options
        static NAN_METHOD(getOptions);

		static void afterWork(uv_work_t* handle, int status) {};

		void applyOptions( Local<Object> options );				//!< Sets the given options and restarts the audio stream if necessary
		void wrapObject( v8::Handle<v8::Object> object );		//!< Wraps a handle into an object

		void queueOutputBuffer( Handle<Array> result );			//!< Queues up an array to be sent to the sound card
		void setSample( int position, Handle<Value> sample );	//!< Sets a sample in the queued output buffer

		Local<Array> getInputBuffer();							//!< Returns a v8 array filled with input samples
		Handle<Number> getSample( int position );				//!< Returns a sound card sample converted to a v8 Number

		void setupFFT();
		void freeFFT();
		static NAN_METHOD(setFFTCallback);//void setFFTCallback(v8::Local<v8::Function> function);	//!< Sets the fft callback
		double getDoubleSample(int position);					//!< Returns a soundcard sample as double for fftw
		void fillWindows();										//!< Fills the windows with all available samples and tries to execute the plan on full windows.
		void runPlan(int iChannel);								//!< Runs the fftw plan for a specific channel

		PaStream *m_pPaStream;				//!< The PortAudio stream object
		PaStreamParameters m_inputParams,	//!< PortAudio stream parameters
						   m_outputParams;

		Local<Array> m_hInputBuffer;		//!< Our pre-allocated input buffer

		uv_thread_t ptStreamThread;			//!< Our stream thread

		uv_mutex_t m_mutex;					//!< A mutex for transferring data between the DSP and UI threads

		int m_uOutputChannels,		//!< Number of input channels
			m_uInputChannels,		//!< Number of output channels
			m_uInputDevice,			//!< Index of the current input device
			m_uOutputDevice,		//!< Index of the current output device
			m_uSampleRate,			//!< Current sample rate
			m_uSamplesPerBuffer,	//!< Number of sample frames per process buffers
			m_uNumBuffers,			//!< Number of sample buffers to keep ahead
			m_uSampleFormat,		//!< Index of the current sample format
			m_uSampleSize;			//!< Number of bytes per sample frame

		unsigned int m_uCurrentWriteBuffer, m_uCurrentReadBuffer;
		unsigned int* m_uNumCachedOutputSamples;		//!< Number of samples we've queued up, outgoing to the sound card

		bool m_bInputOverflowed,			//!< Set when our buffers have overflowed
			 m_bOutputUnderflowed,
			 m_bReadMicrophone,
			 m_bInterleaved;				// This will never actually be used as flag for portaudio and channels are always interleaved therefore internally //!< Set when we're processing interleaved buffers

		char* m_cachedInputSampleBlock,		//!< Temp buffer to hold buffer results
			** m_cachedOutputSampleBlock,
			* m_cachedOutputSampleBlockForWriting;

		Isolate* m_pIsolate;

		Locker* m_pLocker;

		// fftw stuff
		/** An indicator if the resources for the fft's are allocated. */
		bool fft_resources_allocated;
		/** The callback function */
		Nan::Callback fft_callback;
		/** The fft size */
		int fft_window_size;
		/** The relative overlap size of the windows (0 >= fft_overlap_size < 1). 0 means no overlap, 0.5 would mean 50%. */
		float fft_overlap_size;
		/** The window function to use on the fft windows */
		WindowFunction* fft_window_function;
		/** The type of window function to use */
		WindowFunctionType fft_window_function_type;
		/** The windows for each channel */
		double** fft_windows;
		/** The current window filling index for each channel */
		int* fft_window_idx;
		/** The fft_results for each channel */
		fftw_complex** fft_results;
		/** The fft plans for each channel */
		fftw_plan* fft_plans;

	}; // end class AudioEngine

} // end namespace Audio
