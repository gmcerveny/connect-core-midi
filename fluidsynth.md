Install via homebrew

```
brew install fluidsynth
```

You'll also need some soundfonts: (http://musescore.org/en/handbook/soundfont)

I used FluidR3_GM.sf2. 

>  tar -C soundfonts -zxvf yourfile.tar.gz 

Then start the synth:

>  fluidsynth soundfonts/FluidR3\ GM2-2.SF2 

The program should now run and play the note on a piano.  You can use software like [midikeys](http://www.manyetas.com/creed/midikeys.html) to test independently.