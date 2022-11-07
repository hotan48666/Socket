#include <iostream>
#include <WinSock2.h>

#pragma commet(lib, "WS2_32.lib")

using namespace std;

//Server 
//Client 고객

int main()
{

	WSAData wsaData;

	//소켓 로드 
	//int Result = WSAStartup(MAKEWORD(2,2), &wsaData); 

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) < 0)
	{
		cout << "WinSocket init  Error" << GetLastError() << endl;
		exit(-1);
	}

	//소켓 생성 
	SOCKET ServerSocket =  socket(AF_INET, SOCK_STREAM, 0);

	if (ServerSocket == INVALID_SOCKET)
	{
		cout << "WinSocket Make  Error" << GetLastError () << endl;
		exit(-1);
	}

	SOCKADDR_IN ServerSocketAddr;

	memset(&ServerSocketAddr, 0, sizeof(SOCKADDR_IN)); //0으로 초기화 

	ServerSocketAddr.sin_family = PF_INET;
	ServerSocketAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); // host to network long
	ServerSocketAddr.sin_port = htons(12345); // host to network short

	int Status = bind(ServerSocket, (SOCKADDR*)&ServerSocketAddr, sizeof(ServerSocketAddr)); // IP에 소켓 바인딩 

	if (Status == SOCKET_ERROR)
	{
		cout << "WinSocket bind  Error" << GetLastError () << endl;
		exit(-1);
	}

	Status = listen(ServerSocket, 0);

	if (Status == SOCKET_ERROR)
	{
		cout << "WinSocket listen  Error" << GetLastError() << endl;
		exit(-1);
	}

	SOCKADDR_IN ClientAddrIn;
	memset(&ClientAddrIn, 0, sizeof(SOCKADDR_IN)); //0으로 초기화 
	int ClientLength = sizeof(ClientAddrIn);

	//Blocking Socket
	SOCKET ClientSocket =  accept(ServerSocket, (SOCKADDR*)&ClientAddrIn, &ClientLength);

	if (ClientSocket == INVALID_SOCKET)
	{
		cout << "WinSocket accept  Error" << GetLastError() << endl;
		exit(-1);
	}

	const char Message[] = "HelloWorld";
	int SendBytes =  send(ClientSocket, Message, strlen(Message)+1, 0);

	if (SendBytes <= 0)
	{
		cout << "WinSocket SendBytes  Error" << GetLastError() << endl;
		exit(-1);
	}

	char Buffer[1024] = {0,};

	int RecvBytes = recv(ClientSocket, Buffer, 1024, 0);

	if (RecvBytes <= 0)
	{
		cout << "WinSocket RecvBytes  Error" << GetLastError() << endl;
		exit(-1);
	}

	closesocket(ClientSocket);
	closesocket(ServerSocket);

	WSACleanup(); //소켓 언로드 
	return 0;
}