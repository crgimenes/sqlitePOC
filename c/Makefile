withlocallib:
	gcc -o sqlite_local main.c -lsqlite3

withamalgamation:
	gcc -o sqlite_amalg main.c sqlite3.c

linux:
	gcc -o sqlite_local main.c -lsqlite3 -ldl -pthread
	gcc -o sqlite_amalg main.c sqlite3.c -ldl -pthread


clean:
	rm -rf sqlite_local sqlite_amalg *.db

