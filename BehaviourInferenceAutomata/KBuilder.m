% [Authors]: Hasan Ibne Akram, Huang Xiao
% [Institute]: Munich University of Technology
% [Web]: http://code.google.com/p/gitoolbox/
% [Emails]: hasan.akram@sec.in.tum.de, huang.xiao@mytum.de
% Copyright ? 2010
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
% Read samples from file and convert it into a Ktestable language
% Input: filename,the path of sample file
%        k, the learning parameter k
% Output: return a K-testable language
% see also KSET, K2dfa

function kset = KBuilder(k, indata)
%KBUILDER Summary of this function goes here
%   Detailed explanation goes here
    alphabet = {};
    I = {};
    C = {};
    F = {};
    T = {};
    %fd = fopen(filename);
  %  s = getl(indata);
    
   % while (ischar(s))
        
     for result= strsplit(indata,"\n");    
       
         dataList=strsplit(result{1}," ");         	 
               
        if length(dataList) < k
            if isequal(exist(dataList, C), [1]) == 0
                C{1,length(C)+1} = dataList;
            end
        end
           
        if length(dataList) >= (k-1)        	
        	          
            begRes=dataList(1: k-1);
             endRes=dataList( length(dataList)-k+2 : length(dataList));    
                        
            if belongs(begRes, I)==0
                I{1,length(I)+1} = begRes; 
            end
            if belongs(endRes, F)== 0               
                F{1,length(F)+1} =endRes;
            end          
        end
        
        if length(dataList) >= k
            index = 1;       
             for index = 1:(length(dataList)-k+1)
                if belongs(dataList(index:(index+k-1)), T) == 0
                    T{1,length(T)+1} = dataList(index:(index+k-1));
                end
            end
        end
        
        for i = 1:length(dataList)        
            if isequal(ismember(dataList(i){1}, alphabet), [1]) == 0
                alphabet{1,length(alphabet)+1} = dataList(i){1};
            end
        end
      %  s = fgetl(fd);          
    endfor
    kset = KSET(alphabet, I, C, F, T);    
  %  disp(kset);
end
