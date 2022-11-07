#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "WS2_32.lib") // DLL


using namespace std;

int main()
{
	WSAData wsaData;
	int Result = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (Result < 0)
	{
		cout << "winsock init error  :   " << GetLastError() << endl;
		exit(-1);
	}

	SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ServerSocket == INVALID_SOCKET)
	{
		cout << "make socket error  :  " << GetLastError() << endl;
		exit(-1);
	}

	// 주소 세팅
	SOCKADDR_IN ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(SOCKADDR_IN));
	ServerSockAddr.sin_family = PF_INET;
	ServerSockAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); // host to network long
	ServerSockAddr.sin_port = htons(12345); // host to network short

	// 연결
	int Status = bind(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));
	if (Status == SOCKET_ERROR)
	{
		cout << "bind error  :  " << GetLastError() << endl;
		exit(-1);
	}

	Status = listen(ServerSocket, 0);
	if (Status == SOCKET_ERROR)
	{
		cout << "listen error  :  " << GetLastError() << endl;
		exit(-1);
	}


	// 클라용 랜을 하나 더만듬
	SOCKADDR_IN ClientAddrIn;
	memset(&ClientAddrIn, 0, sizeof(SOCKADDR_IN));
	int ClientLength = sizeof(ClientAddrIn);

	// 연결 허용 크기, blocking socket(비효율)
	SOCKET ClientSocket = accept(ServerSocket, (SOCKADDR*)&ClientAddrIn, &ClientLength);
	// 연결 허용 체크
	if (ClientSocket == INVALID_SOCKET)
	{
		cout << "listen error  :  " << GetLastError() << endl;
		exit(-1);
	}

	// 보내기
	const char Message[] = "Hello World";
	// 보내기 에러체크
	int SendBytes = send(ClientSocket, Message, strlen(Message) + 1, 0); // +1 은 NULL 까지 보냄
	if (SendBytes <= 0)
	{
		cout << "send error  :  " << GetLastError() << endl;
		exit(-1);
	}

	// 받기
	char Buffer[1024] = { 0, };
	int RecvBytes = recv(ClientSocket, Buffer, 1024, 0);
	// 받기 에러체크
	if (RecvBytes <= 0)
	{
		cout << "recv error  :  " << GetLastError() << endl;
		exit(-1);
	}

	closesocket(ClientSocket);
	closesocket(ServerSocket);


	WSACleanup(); // 언로드

	return 0;
}