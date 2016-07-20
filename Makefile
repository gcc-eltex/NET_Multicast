all: bin/server bin/client

bin/server: src/server.c
	gcc src/server.c -o bin/server -g

bin/client: src/client.c
	gcc src/client.c -o bin/client -g

run_srv:
	./bin/server

run_clt:
	./bin/client

clean:
	rm -rf ./bin/*
