#include <stdio.h>
#include <typeinfo>
#include <string>
#include <string.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <vector>
#include <algorithm>

#include "../include/Session.h"
#include "../include/EpollService.h"
#include "../include/DBJobContext.h"
#include "../include/DBJobManager.h"
#include "../include/WorkerQueue.h"

EpollService *GEpollService = nullptr;

bool EpollService::Initialize(uint16 Port, uint32 MaxClient, ITcpSocketCallback *Callback)
{
	mListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mListenSocket < 0)
		return false;

	int opt = 1;
	setsockopt(mListenSocket, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(int));

	/// bind
	sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(sockaddr_in));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(Port);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int ret = bind(mListenSocket, (sockaddr *)&serveraddr, sizeof(serveraddr));
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

	if (Callback != nullptr)
		SetSockCallback(Callback);
	else
		return false;

	printf("[DEBUG] EPOLL SERVICE SUCCESSFULLY INITIALIZED . \n");
	return true;
}

Session *EpollService::CreateSession(SOCKET sock)
{

	Session *client = new Session(sock);
	client->SetService(this);
	mClientList.insert(ClientList::value_type(sock, client));

	return client;
}

void EpollService::DeleteSession(Session *client)
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

	while (bIsServerRun)
	{
		int nfd = epoll_wait(mEpollFd, events, MAX_CONNECTION, 100);

		for (int i = 0; i < nfd; ++i)
		{
			if (events[i].data.fd == mListenSocket)
			{
				sockaddr_in clientAddr;
				socklen_t clientAddrLen = sizeof(sockaddr_in);

				SOCKET client = accept(mListenSocket, (sockaddr *)&clientAddr, &clientAddrLen);
				if (client < 0)
				{
					printf("[DEBUG] accept error: IP=%s, PORT=%d . \n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
					continue;
				}

				Session *newClient = CreateSession(client);
				newClient->OnConnect(clientAddr);
				newClient->SetTcpSockCallback(mTcpCallback);

				printf("[INFO] ACCEPT SOCKET NUM : %d . \n", client);

				epoll_event ev;
				memset(&ev, 0, sizeof(ev));
				ev.events = EPOLLIN | EPOLLET;
				ev.data.ptr = static_cast<void *>(newClient);
				epoll_ctl(mEpollFd, EPOLL_CTL_ADD, client, &ev);

				const std::string InitSend = "\x1B[32m\r\nWELCOME TO SANGIN WORLD! \r\n\x1B[37mPlease enter your name, or \"new\" if you are new user: \x1B[0m";
				write(newClient->GetSocket(), InitSend.c_str(), InitSend.size());
			}
			else
			{
				GWorkerQueue->pushSignal(events[i]);
			}
		}
		FlushClientSend();

		DispatchDatabaseJobResults();
	}
}

void EpollService::BroadCast(std::string &pMessage)
{
	for (ClientList::const_iterator it = mClientList.begin(); it != mClientList.end(); ++it)
	{
		Session *client = it->second;
		client->OnSend(pMessage);
	}
}

void EpollService::ReleaseClient(Session *_session)
{
	epoll_event ev;
	memset(&ev, 0, sizeof(epoll_event));
	epoll_ctl(mEpollFd, EPOLL_CTL_DEL, _session->GetSocket(), &ev);

	delete _session;
}

bool EpollService::handleFd(struct epoll_event ev)
{
	Session *currentSession = reinterpret_cast<Session *>(ev.data.ptr);
	int sockFd = currentSession->GetSocket();

	if (mClientList.find(sockFd) == mClientList.end())
	{
		printf("[ERROR] ERROR IN FIND RECEIVE EVENT SESSION \n");
		return false;
	}

	currentSession->OnReceive();
	epoll_event newEV;
	memset(&newEV, 0, sizeof(newEV));
	newEV.events = EPOLLIN | EPOLLET;
	newEV.data.ptr = static_cast<void *>(currentSession);
	epoll_ctl(mEpollFd, EPOLL_CTL_ADD, sockFd, &newEV);

	return true;
}

void EpollService::FlushClientSend()
{
	for (auto &it : mClientList)
	{
		Session *client = it.second;
		if (false == client->SendFlush())
		{
			client->DisConnect();
		}
	}
}

void EpollService::DispatchDatabaseJobResults()
{
	DatabaseJobContext *dbResult = nullptr;
	while (GDatabaseJobManager->PopDatabaseJobResult(dbResult))
	{
		if (false == dbResult->mSuccess)
		{
			printf("DB JOB FAIL \n");
		}
		else
		{
			if (typeid(*dbResult) != typeid(FindPlayerDataContext))
			{
				printf("DB Create Player \n");
				//CreatePlayerDone(dbResult);
			}
			else
			{
				auto it = mClientList.find(dbResult->mSockKey);
				printf("DB FIND SOCKET SUCCESSFULLY \n");
				if (it != mClientList.end() && it->second->IsConnected())
				{
					it->second->DatabaseJobDone(dbResult);
				}
			}
		}

		DatabaseJobContext *toBeDelete = dbResult;
		delete toBeDelete;
	}
}