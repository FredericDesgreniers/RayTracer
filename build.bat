@echo off


IF NOT EXIST f:\comp371_a3\build mkdir f:\comp371_a3\build
pushd f:\comp371_a3\build

cl -DENGINE_WIN32=1 -DENGINE_INTERNAL=1 -DENGINE_SLOW=1 /EHsc  -FC -Zi f:\comp371_a3\src\SceneCreator.cpp  user32.lib gdi32.lib 

popd
