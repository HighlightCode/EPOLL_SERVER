#include <stdio.h>
#include <typeinfo>
#include <string.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <vector>
#include <algorithm>

#include "../include/Session.h"
#include "../include/EpollService.h"

EpollService* GEpollService = nullptr ;

bool EpollService::Initialize(uint16 Port, uint32 MaxClient)
{
	mListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mListenSocket < 0)
		return false;

	int opt = 1;
	setsockopt(mListenSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(int));

	/// bind
	sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(sockaddr_in));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(Port);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int ret = bind(mListenSocket, (sockaddr*)&serveraddr, sizeof(serveraddr));
	if (ret < 0)
		return false;

	/// listen
	ret = listen(mListenSocket, SOMAXCONN);
	if (ret < 0)
		return false;

	mEpollFd = epoll_create(MaxClient);
	if (mEpollFd < 0)
		return false;

	epoll_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.events = EPOLLIN;
	ev.data.fd = mListenSocket;
	ret = epoll_ctl(mEpollFd, EPOLL_CTL_ADD, mListenSocket, &ev);
	if (ret < 0)
		return false;

    printf("[DEBUG] EPOLL SERVICE SUCCESSFULLY INITIALIZED \n");
	return true;

}

Session* EpollService::CreateSession(SOCKET sock)
{

	Session* client = new Session(sock) ;
	mClientList.insert(ClientList::value_type(sock, client)) ;

	return client ;
}

void EpollService::DeleteSession(Session* client)
{
	epoll_event ev;
	memset(&ev, 0, sizeof(ev));
	epoll_ctl(mEpollFd, EPOLL_CTL_DEL, client->GetSocket(), &ev);

	mClientList.erase(client->GetSocket());
	delete client;
}

void EpollService::EventLoop()
{
    epoll_event events[MAX_CONNECTION];
    
    while(true)
    {
       int nfd = epoll_wait(mEpollFd, events, MAX_CONNECTION, 100);
       
       for(int i = 0; i < nfd; ++i)
       {
            if(events[i].data.fd == mListenSocket)
            {
                sockaddr_in clientAddr;
				socklen_t clientAddrLen = sizeof(sockaddr_in);

				SOCKET client = accept(mListenSocket, (sockaddr *)&clientAddr, &clientAddrLen);
				if (client < 0) 
				{
					printf("[DEBUG] accept error: IP=%s, PORT=%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
					continue;
				}

				Session* newClient = CreateSession(client);
				newClient->OnConnect(clientAddr);

				epoll_event ev;
				memset(&ev, 0, sizeof(ev));
				ev.events = EPOLLIN | EPOLLET;
				ev.data.ptr = static_cast<void*>(newClient);
				epoll_ctl(mEpollFd, EPOLL_CTL_ADD, client, &ev);
            }
       }

    }

}