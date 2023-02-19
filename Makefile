CC	= /home/user/Downloads/gb-dev/gbdk/bin/lcc -Wm-yc

BINS	= cliks.gbc

all:	$(BINS)

%.o:	%.c
	$(CC) -c -o $@ $<

%.s:	%.c
	$(CC) -S -o $@ $<

%.o:	%.s
	$(CC) -c -o $@ $<

%.gbc:	%.o
	$(CC) -o $@ $<
