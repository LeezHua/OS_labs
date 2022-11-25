cc = gcc
S  = filesystem
U  = user

object = $S/disk.o $S/file.o $S/fs.o
user = $U/ls $U/mkdir $U/rm $U/touch

$(object): %.o : %.c
	$(cc) -c $< -o $@

$(user): % : %.c
	$(cc) -o $@ $<

mkfs: mkfs.c $(object)
	$(cc) -o mkfs mkfs.c $(object)

main: main.o $S/main.h $(object) $(user)
	$(cc) -o main main.o $(object)

main.o: main.c $S/types.h $S/stat.h $S/fs.h $S/file.h $S/defs.h
	$(cc) -c main.c

clean:
	-rm -f main mkfs */*.o *.o $(user)