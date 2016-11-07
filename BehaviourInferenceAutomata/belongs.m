function answer= belongs(member, array)
answer =0;	
 for i=1:length(array) 	
      if (isequal(member, array{i}))
      	answer=1;
      	return;
      endif
 endfor
endfunction