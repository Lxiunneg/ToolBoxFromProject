## 使用说明
本文件直接通过`#include`引入项目

### Example
```cpp
#include "SimpleWinSocket.hpp"

void ExampleCallback(char* revbuf,int len){
    //do something...
}

#pragma pack(1)
struct req{
    uint8_t data = 0x04;
    uint8_t type = 0x02;
    uint16_t data = 0x00;
}Example;


int main() {
	SimpleWinSocket Client;
	Client.InitSocket("127.0.0.1", 777);
	Client.ConnectToServer();
    //1. send char array.
    char sendbuf[4] = {0x04,0x02,0x00,0x00};
	Client.SendToServer(sendbuf,4);

    //2. send stcuct
    Client.SendToServer((char*)&(Example),sizeof(req));

	char *recvBuffer = nullptr;
	Client.RecvFromServer(recvBuffer, 1024,ExampleCallback);
	Client.Disconnect();
	delete[] recvBuffer;
	return 0;
}
```