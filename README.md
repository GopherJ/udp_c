# udp_c

udp example written in c, it remove special symbols in received messages and then return it to client.

## Build

```bash
cmake CMakeLists.txt
make

# make clean to remove binaries
```

## Usage

```bash
./udp_server 127.0.0.1 8088
./udp_client 127.0.0.1 8088 # enter messages
```


