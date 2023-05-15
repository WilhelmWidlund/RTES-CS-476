function out = customFilter(in)
out = filter(fir1(30, [0.25 0.5]),1,in);
end

