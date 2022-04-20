# BandLoop

BandLoop is a set of five bluetooth pedals (Arduino-based) that interacted with a digital audio workstation (built with J.U.C.E.) to sync a band's musicians to a global tempo.  It was recognized with a Santander Prototype Fund grant and an invitation to speak at the 2019 Audio Developer Conference.


Here a Diagram of how it works :


<img src="https://user-images.githubusercontent.com/22892458/164270393-93015c95-0d42-497c-859a-5a08cf7cb0bf.jpg" width="650" />


### Hardware


Here a photo of its first prototype :


<img width="500" alt="Screenshot_2022-04-20_at_17 58 31-removebg-preview" src="https://user-images.githubusercontent.com/22892458/164273105-c78144d0-b69a-46c7-87fa-bf5a8a21f454.png">


### Software

The executable files is in the in the following Directory : Build/MacOSX/build/Debug/Bandloop.app. 
The code I wrote is in the Source folder, but, by itself, it is quite useless, 
in fact it is dependent on the JUCE library. 
Luckily enough, the JUCE website provide us with Projucer.

Projucer is a customized IDE which enable the user to load .jucer project
It also allow you to export them into the desired IDE ( XCode, Visual Studio, Linux ) .

In order to do that so it is require do download the Projucer app from the following link :
https://shop.juce.com/get-juce/download

After having installed Projucer,  just open Recorder+Sampler.jucer ;   

Then, from the Projucer menu bar, click  "Show Exporter Panel" ;

by clicking on the "+" button at the bottom-left you can choose your favourite IDE to export the entire project.

GOOD LUCK !!!!

