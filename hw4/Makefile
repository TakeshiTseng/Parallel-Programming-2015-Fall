CC=nvcc

cuda:
	if [ -a ./bin/serial_wave ] ; \
	then \
		rm ./bin/serial_wave ; \
	fi;
	$(CC) serial_wave.cu -o ./bin/serial_wave

rc:
	./bin/serial_wave 10000 20

ori:
	if [ -a ./bin/serial_wave_ori ] ; \
	then \
		rm ./bin/serial_wave_ori ; \
	fi;
	$(CC) serial_wave_ori.c -o ./bin/serial_wave_ori

ro:
	./bin/serial_wave_ori 10000 20

