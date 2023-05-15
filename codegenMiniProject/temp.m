disp('Hello from the testbench')
audioIn = (1:1031)';
shift = 10;
shiftAudio(audioIn, shift);
shiftAudio(audioIn, shift);
shiftAudio(audioIn, shift);
disp('Function executed 3x')