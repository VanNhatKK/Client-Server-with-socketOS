.PHONY: socketClient

socketClient:
	gcc -o Client socketClient.c libsocket.c -pthread
socketServer:
	gcc -o Server socketServer.c libsocket.c -pthread