# StrinGO

_Synth based on two coupled waveguide delay lines._

![image info](./doc/StrinGO_screen.png)

Binary downloads for Linux and Windows can be found in the release section:
https://github.com/odoare/StrinGO/releases

Since v0.0.2, binaries are distributed as VST3i.

Presets are found in the Presets/VST3 directory. This directory will be regularly updated with new presets.

## Info

Refer to the schematic below for an illustrative signal path.

StrinGO is based around two waveguides that are fed at input position by four different sources:

- The effect audio input (StrinGO is indeed seen as an audio effect with midi input) (A)

- A basic waform oscillator (B)

- A noise generator (C)

- A crackling sound generator (D)

The signal at waveguides' outpout position are then mixed together and sent to the master output section (E). 

The waveguides are optionnally coupled in the following way (F):

- Output of waveguide 1 is sent to input of waveguide 2 if _coupling_ parameter is positive.

- Output of waveguide 2 is sent to input of waveguide 1 if _coupling_ parameter is negative.

Each waveguide has individual level and pitch tunings (G).

The waveguides' feedback loop gain and lowpass filter frequency depend on the voice state (on/off) (H). Transition between states is managed by an ADSR enveloppe (I).

![image info](./doc/signal_path.png)

## Development info

StrinGO relies on the C++ framework JUCE. Refer to the JUCE website for development info: https://juce.com/

If you want to compile the plugin by yourself it basically consists of loading the .jucer project with Projucer, saving it and compiling with the dedicated tool depending on your platform.

## History

- v0.0.1 : Initial release

- v0.0.2 : 3 LFO can now modulate some parameters

## Future work

Here are the main improvements that are planned. This will probably end in a new plugin instead of just a version bump.

- Synced LFOs, sequences, etc. This will greatly improve the sonic possibilities of the synth.

- New waveforms for the sampler

- Bridge/plate interactions between strings, soundboard radiation for some physically accurate and realistic sounds.

- Oversampling for better quality and stability of the waveguides.

## Contact

olivier.doare@ensta-paris.fr
