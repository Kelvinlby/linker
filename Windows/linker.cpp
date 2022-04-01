/* **********************************************************************************
 *                                  linker 
 *                       Copyright (C) 2022 Kelvin_LBY
 *
 *     This program is free software: you can redistribute it and/or modify it under
 *     the terms of the GNU General Public License as published bythe Free Software
 *     Foundation, either version 3 of the License, or any later version.
 *
 *     This program is distributed in the hope that it will be useful, but WITHOUT
 *     ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *     FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License
 *     along with this program. If not, see <https://www.gnu.org/licenses/>.
 * 
 *     License: https://github.com/Kelvinlby/linker/LICENSE
 * 
 * **********************************************************************************/

// --- includes
#include <iostream>
#include <thread>
#include <windows.h>
#include <winsock.h>
#include <csignal>
#include "./inc/color.h"
 
//#pragma comment (lib,"wsock32.lib")
#pragma comment (lib,"ws2_32.lib")

// --- defination and vars
#define VERSION "1.0.0"
#define MSG_WAITALL 0x8
char COMMAND_DISCONNECT[] = "linker_command__disconnect__Code_0xFF";
static volatile int keepRunning = 1;

// TODO 1.server端连接多个client
//      2.解决server端键盘中断需要client端发送消息才能结束socket的问题

// --- usings
using namespace std;

// --- declaration
void about();
int server( const char* ip );
int client( const char* ip );
void Send( SOCKET client, char send_buf[] );
void Recv( SOCKET client, char send_buf[] );

// --- set keyboard interrupt
void sig_handler( int sig )
{
    if( sig == SIGINT )
    {
        keepRunning = 0;
    }
}

// --- functions
int main( int argv, char* args[] )
{
    if( argv == 3 )
    {
        // get ip
        const char* ip = (char*) args[2];

        // get mode
        if( (string) (char*) args[1] == "server" )
        {
            return server( ip );
        }
        else if( (string) (char*) args[1] == "client" )
        {
            return client( ip );
        }
        else
        {
            about();
            return -1;
        }
    }
    else
    {
        about();
        return -1;
    }
}

int server( const char* ip )
{
    int len;
    char send_buf[100];
    char recv_buf[100];
    int timeout = 1000;  // 1s

    thread thread_send, thread_recv;
    WSADATA wsa_data;
    SOCKET server, client;
    SOCKADDR_IN server_addr, client_addr;

    WSAStartup( MAKEWORD( 2, 2 ), &wsa_data );

    server = socket(AF_INET, SOCK_STREAM, 0);

    memset( &server_addr, 0, sizeof( server_addr ) );
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr( (char*) ip );
    server_addr.sin_port = htons( 1234 );
    bind( server, (SOCKADDR*) &server_addr, sizeof( SOCKADDR ) );

    // connect
    listen( server, 10 );
    cout << YELLOW << "waiting for response ..." << RESET << endl;
    len = sizeof( SOCKADDR );
    client = accept( server, ( SOCKADDR* ) &client_addr, &len );
    cout << GREEN <<  "[DONE]" << RESET << "  connected" << endl;

    // start threads
    thread_send = thread( Send, client, send_buf );
    thread_recv = thread( Recv, client, recv_buf );

    // wait for threads to finish
    thread_send.join();
    thread_recv.join();

    // disconnect
    closesocket( server );
    closesocket( client );
    WSACleanup();

    return 0;
}

int client( const char* ip )
{
    char send_buf[100];
    char recv_buf[100];

    thread thread_send, thread_recv;
    WSADATA wsa_data;
    SOCKET client;
    SOCKADDR_IN client_addr;

    WSAStartup( MAKEWORD( 2, 2 ), &wsa_data );

    client = socket( AF_INET, SOCK_STREAM, 0 );

    memset( &client_addr, 0, sizeof( client_addr ) );
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr( ip );
    client_addr.sin_port = htons( 1234 );
    connect( client, (SOCKADDR*) &client_addr, sizeof( SOCKADDR ) );

    thread_send = thread( Send, client, send_buf );
    thread_recv = thread( Recv, client, recv_buf );

    thread_send.join();
    thread_recv.join();

    closesocket( client );
    WSACleanup();

    return 0;
}


void Send( SOCKET client, char send_buf[] )
{
    signal( SIGINT, sig_handler );

    while( keepRunning )
    {
        cout << YELLOW << "send: ";
        cin >> send_buf;
        cout << RESET << endl;
        send( client, send_buf, 100, 0 );
    }

    // keyboard interrupt
    send( client, COMMAND_DISCONNECT, 100, 0 );
    cout << BOLDRED << "  -+-+------------ sender disconnect ------------+-+-" << RESET << endl;
}

void Recv( SOCKET client, char recv_buf[] )
{
    int len = 0;
    signal( SIGINT, sig_handler );

    while( keepRunning )
    {
        len = recv( client, recv_buf, 100, MSG_WAITALL );
        
        if( strcmp( recv_buf, COMMAND_DISCONNECT ) == 0 )
        {
            keepRunning = 0;
        }
        else
        {
            cout << CYAN << "receive: " << recv_buf << RESET << endl;
        }
    }

    // keyboard interrupt
    cout << BOLDRED << "  -+-+------------ receive disconnect ------------+-+-" << RESET << endl;
}

void about()
{
    // about linker
    cout << RESET << BOLDCYAN << "   _ " << BOLDRED << " _" << BOLDCYAN << "         _" << endl;
    cout << RESET << BOLDCYAN << "  | |" << BOLDRED << "|#|" << BOLDCYAN << "       | | __" << RESET << CYAN << "           -+-+-+-------------------+-+-+-" << endl;
    cout << RESET << BOLDCYAN << "  | | _  _ ___ | |/ / __  _ __" << endl;
    cout << RESET << BOLDCYAN << "  | || || '_  || | // __ \\ ' _] " << RESET << CYAN << " COPYRIGHT (C) 2022 Kelvin_LBY" << endl;
    cout << RESET << BOLDCYAN << "  | || || | | ||   \\\\  __/| |   " << RESET << CYAN << " Version: " << VERSION << endl;
    cout << RESET << BOLDCYAN << "  |_||_||_| |_||_|\\_\\\\___||_|  " << RESET << CYAN << "  License: GPL-v3.0\n" << endl;
    cout << RESET << MAGENTA << "  Github: https://github.com/Kelvinlby/linker" << endl;
    cout << RESET << endl;

    // user guide
    cout << YELLOW << "Usage" << RESET << endl;
    cout << GREEN << "  linker <mode> <IP>" << RESET << endl;
    cout << "\n  mode:" << endl;
    cout << "    server  ------  start linker as server" << endl;
    cout << "    client  ------  start linker as client" << endl;
    cout << "  IP:" << endl;
    cout << "    server mode: your IP" << endl;
    cout << "    client mode: IP of server\n" << endl;
    
    // return code
    cout << YELLOW << "Return Code" << RESET << endl;
    cout << "  -1  argument is not correct" << endl;

    // return code
    cout << YELLOW << "Notice" << RESET << endl;
    cout << "  message over 100 words is not supported" << endl;

    cout << endl;
}
