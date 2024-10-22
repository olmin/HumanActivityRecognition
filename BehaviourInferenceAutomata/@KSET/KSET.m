% [Authors]: Hasan Ibne Akram, Huang Xiao
% [Institute]: Munich University of Technology
% [Web]: http://code.google.com/p/gitoolbox/
% [Emails]: hasan.akram@sec.in.tum.de, huang.xiao@mytum.de
% Copyright © 2010
% 
% ****** This is a beta version ******
% [DISCLAIMER OF WARRANTY]
% This source code is provided "as is" and without warranties
% as to performance or merchantability. The author and/or 
% distributors of this source code may have made statements 
% about this source code. Any such statements do not constitute 
% warranties and shall not be relied on by the user in deciding
% whether to use this source code.
% 
% This source code is provided without any express or implied
% warranties whatsoever. Because of the diversity of conditions
% and hardware under which this source code may be used, no
% warranty of fitness for a particular purpose is offered. The 
% user is advised to test the source code thoroughly before relying
% on it. The user must assume the entire risk of using the source code.
% 
% -------------------------------------------------
% [Description]
% Here defines the class of K-testable language

function k = KSET(a, i, c, f, t)

    % This class deinfes the KSET structure
        dic.Alphabets = a;   % the set of alphabets
        dic.ISET = i;  % prefixes of length k-1 and suffixes (or finals) of length k-1   
        dic.CSET = c;  % short strings with length < k
        dic.FSET = f;  % prefixes of length k-1 and suffixes (or finals) of length k-1
        dic.TSET = t; % allowed segments
	k = dic;
endfunction


