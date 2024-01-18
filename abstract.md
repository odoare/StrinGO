2 coupled strings.

Each sting has 1 input force at inPos and one output at outPos. Strings are coupled at end position (bridge coupling).

Parameters :
- inPos1, inPos2, outPos1, outPos2
- inputGain1, inputGain2
- coupling12, coupling21
- freq, freqCoarseFactor1, freqCoarseFactor2, freqFineFactor1, freqFineFactor2
- feedBackGainOn1, feedBackGainOn2
- feedBackFreqOn1, feedbackFreqOn2
- feedBackGainOff1, feedBackGainOff2
- feedBackFreqOff1, feedbackFreqOff2
- ADSR1
(- LFO1, LFO2 (osc type + freq))

Input parameters :
- Noise level (+ ADSRN)
- Noise properties
- Attack signal slection (possible to model both hammer&plucking ?)
- Attack signal level

Output parameters :
- Bridge/soundboard/radiation transfer function
- Guitar pickup
- Filter
