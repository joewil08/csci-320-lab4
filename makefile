translate: address.o stat.o main.o
	$(CC) -o $@ $^ -lrt

address.o: address.c
	$(CC) -c address.c

stat.o: stat.c
	$(CC) -c stat.c

main.o: main.c
	$(CC) -c main.c

clean:
	-rm -f *.o time
	@echo "All clean!"