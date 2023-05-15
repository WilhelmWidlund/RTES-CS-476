hdlcfg = coder.config('hdl');
hdlcfg.TestBenchName = 'temp';
hdlcfg.GenerateHDLTestBench = false;
shiftType = coder.typeof(10);
audioInType = coder.typeof((1:1031)',[inf,1], [1,0]);
fixptcfg = coder.config('fixpt');
fixptcfg.TestBenchName = 'temp';
%for generating c code:
% codegen -args {audioInType, shiftType} shiftAudio
%for generating vhdl: 
codegen -args {audioInType, shiftType} -config hdlcfg -float2fixed fixptcfg shiftAudio