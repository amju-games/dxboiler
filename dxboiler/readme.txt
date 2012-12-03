HELLO!

DX BOILER

TODO

Auto-generate UVs on teapot

TEXCOORD0, TEXCOORD1, TEXCOORD0
SV_POSITION



lighting - moving lights. Use inv transpose mview ?
load .x model. 
Animation - MD3
Normal mapping, water effects, shadow map. 

Maybe rename DX11VertexBuffer to DX11TriList ?


Fixed
=====
There is a nasty leak. It looks like every frame I should be releasing something that I'm not! :-(
 - Was creating input layout every frame, only needs to be done once.
 Some other leaks but minor by comparison.

Make teapot a drawable

Events

Texturing

Why transpose matrices when we Update C. B. ?????
 - need to convert from row-major (C++ code) to column-major (shader). WTF!?


