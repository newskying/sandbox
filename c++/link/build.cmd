gcc -DLINKTIME -c mymalloc.c
gcc -c myprog.c
gcc -Wl,--wrap,malloc -Wl,--wrap,free -o myprog myprog.o mymalloc.o
