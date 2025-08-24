run_task2:
	gcc -Wall -Wextra -O2 -std=c17 minishell.c -o minishell
run_task1:
	gcc -Wall -Wextra -O2 -std=c17 even.c -o even
clear:
	rm -f *.o
test_task2: run_task2
	( echo echo timing1; \
	  echo 'sleep 2 &'; \
	  echo echo timing2; \
	  echo 'sleep 5'; ) | ./minishell