cc = gcc

C = cmd
S = system

object = $S/fs.o $S/user.o
cmd    = $C/close $C/create $C/delete $C/dir \
				 $C/open $C/read $C/write

$(object): %.o : %.c
	$(cc) -c $< -o $@

$(cmd): % : %.c $(object)
	$(cc) -o $@ $< $(object)

mkfs: mkfs.c $(object)
	$(cc) -o mkfs mkfs.c $S/user.o

main: main.c $(cmd) $(object) 
	$(cc) -o main main.c $(object)

clean: 
	-rm -rf main main.o mkfs $(object) $(cmd)