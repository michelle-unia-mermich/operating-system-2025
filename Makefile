run_task2:
	gcc -Wall -Wextra -O2 -std=c17 minishell.c -o minishell.o
run_task1:
	gcc -Wall -Wextra -O2 -std=c17 even.c -o even.o
clear:
	rm -f *.o
test_task2: run_task2
	( echo echo timing1; \
	  echo 'sleep 2 &'; \
	  echo echo timing2; \
	  echo 'sleep 5'; ) | ./minishell.o
test_task3: run_task2
	( echo echo timing1; \
	  echo 'sleep 1 &'; \
	  echo echo timing2; \
	  echo 'sleep 2'; \
	  echo echo timing3; \
	  echo 'sleep 3 &'; \
	echo echo timing4; \
	  echo 'sleep 8'; )| ./minishell.o