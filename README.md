# Impala
TTGO T-Beam Node 

## Configuring an impala node 
An impala node needs to be connected to the same wireless network as the [oryx](https://github.com/HSchreiberZA/oryx) gateway for the communication to work. This can be achieved by setting the following parameters in the [Makefile](https://github.com/HSchreiberZA/impala/blob/main/Makefile):

```
CFLAGS += -DESP_WIFI_SSID=\"your-ssid\" 
CFLAGS += -DESP_WIFI_PASS=\"your-password\"
```

After connecting the oryx node to the wireless network, it would have been assigned an ipv6 address. For the communication to take place between a node and the gateway, the ipv6 address of the gateway must be configured in [CoapClient.c](https://github.com/HSchreiberZA/impala/blob/f1559ccbc9a20d46b6e784c63d0634c5f507e388/network/CoapClient.c#L8)

Finally, to be able to uniquely identify the nodes between each other, a unique [node id](https://github.com/HSchreiberZA/impala/blob/57be130e156404960fd4d473f45491de944b1f81/Makefile#L52) must be configured for each node in the Makefile

## Building
For instructions on building the projects, see the [RIOT](https://doc.riot-os.org/getting-started.html#docker) page.
