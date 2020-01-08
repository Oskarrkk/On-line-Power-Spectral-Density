## Online Power Spectral Density
It is an application with which it is possible to plot power spectral density on-line for a signal coming from a microphone.
	
## Technologies
Project is created with:
* FFTW3 - To run application you have to install FFTW library and follow instruction from http://www.fftw.org/
* QCustomPlot - Library used to plot PSD on-line
* QTMultimedia and QIODevice - modules that made it easy to operate the microphone and read samples from it.

## Presentation - How It Works

The concept of plotting GWM on-line was relatively simple. It assumed that a buffer of a certain size would be created, which would contain samples collected from the microphone. When new 
samples arrive, they are placed in the buffer in place of the old ones. Samples in the buffer are input samples for the FFT algorithm. 
After appropriate transformations, the output from FFT is plotted as PSD on the chart. This process is repeated all the time with the arrival of new samples.

![](video.gif)
