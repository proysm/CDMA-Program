#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
using namespace std;

// 일반 변수 ----------------------------------------------------------------------------

int t[4]; // 송신기(station)
int r[4]; // 수신기(receiver)

int chip_t[4][8]; // 칩 시퀀스(Chip Sequence)
int dataChip_t0[8], dataChip_t1[8], dataChip_t2[8], dataChip_t3[8]; // 각 송신기의 Chip Sequence + data
int sumChip[8]; // 네개의 신호를 결합한 것(수신기에 보낼 것)
int receive_t0 = 0, receive_t1 = 0, receive_t2 = 0, receive_t3 = 0; // 수신기가 받은 데이터

// pipeline 이용 변수 -------------------------------------------------------------------

int fd_t0[2], fd_t1[2], fd_t2[2], fd_t3[2]; // file discripter of transmitter
int fd_r0[2], fd_r1[2], fd_r2[2], fd_r3[2]; // file discripter of receiver
// pipe() 함수는 파이프를 생성하고 만들어진 discripter를 알려준다.
// fd[1]은 파이프에 write하는 discripter이고, fd[0]는 파이프에 read하는 discripter다.

int buf[10], buf1[10], buf2[10], buf3[10];  // buffer
int t0_pid, t1_pid, t2_pid, t3_pid;
int r0_pid, r1_pid, r2_pid, r3_pid;
int result0, result1, result2, result3;

// 함수 --------------------------------------------------------------------------------

void getData();
void getChipSequence();

// main -------------------------------------------------------------------------------
int main(){

    getData();
    getChipSequence();


    // 송신 시작 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // 송신기 t0
	if (pipe(fd_t0) < 0) 
	{
		perror("pipe error : ");
		exit(0);
	}
	if ((t0_pid = fork()) < 0)
	{
		perror("fork error : ");
		exit(0);
	}
    // 송신기 t1
	if (pipe(fd_t1) < 0)
	{
		perror("pipe error : ");
		exit(0);
	}
	if ((t1_pid = fork()) < 0)
	{
		perror("fork error : ");
		exit(0);
	}
    // 송신기 t2
	if (pipe(fd_t2) < 0)
	{
		perror("pipe error : ");
		exit(0);
	}
	if ((t2_pid = fork()) < 0)
	{
		perror("fork error : ");
		exit(0);
	}
    // 송신기 t3
	if (pipe(fd_t3) < 0)
	{
		perror("pipe error : ");
		exit(0);
	}
	if ((t3_pid = fork()) < 0)
	{
		perror("fork error : ");
		exit(0);
	}
    // 송신기 t0
	else if (t0_pid == 0)
	{
		close(fd_t0[0]); 
        // 자식 프로세스에서는 파이프의 읽는 쪽인 fd[0]를 닫고, 
        // 표준 출력이 fd[1] discipter가 가리키는 스트림을 가리키게 변경한다.

		for (int i = 0; i < 8; i++) dataChip_t0[i] = t[0] * chip_t[0][i];
		write(fd_t0[1], dataChip_t0, sizeof(dataChip_t0));
        // 전송할 신호 계산하고 fd[1]에 write 한다.
	}
    // 송신기 t1
	else if (t1_pid == 0)
	{
		close(fd_t1[0]);
		for (int i = 0; i < 8; i++) dataChip_t1[i] = t[1] * chip_t[1][i];
		write(fd_t1[1], dataChip_t1, sizeof(dataChip_t1));
	}
    // 송신기 t2
	else if (t2_pid == 0)
	{
		close(fd_t2[0]);
		for (int i = 0; i < 8; i++) dataChip_t2[i] = t[2] * chip_t[2][i];
		write(fd_t2[1], dataChip_t2, sizeof(dataChip_t2));
	}
    // 송신기 t3
	else if (t3_pid == 0)
	{
		close(fd_t3[0]);
		for (int i = 0; i < 8; i++) dataChip_t3[i] = t[3] * chip_t[3][i];
		write(fd_t3[1], dataChip_t3, sizeof(dataChip_t3));
	}
	else 
    {
		close(fd_t0[1]);
		close(fd_t1[1]);
		close(fd_t2[1]);
		close(fd_t3[1]);
        // 부모 프로세스에서는 쓰지 않기 때문에 파이프의 쓰는 쪽인 fd[1]을 닫는다.

		int temp1[8], temp2[8], temp3[8], temp4[8]; //송신된 신호들 임시 저장

		// t0
		read(fd_t0[0], buf, sizeof(buf));
        cout << "\ndataChip_t0 = ";
		for (int i = 0; i < 8; i++) cout << buf[i] << " ";

        // t1
		read(fd_t1[0], buf1, sizeof(buf1));
		printf("\ndataChip_t1 = ");
		for (int i = 0; i < 8; i++) cout << buf1[i] << " ";

        // t2
		read(fd_t2[0], buf2, sizeof(buf));
		printf("\ndataChip_t2 = ");
		for (int i = 0; i < 8; i++) cout << buf2[i] << " ";
		
        sleep(3);
        // t3
		read(fd_t3[0], buf3, sizeof(buf));
		printf("\ndataChip_t3 = ");
		for (int i = 0; i < 8; i++) cout << buf3[i] << " ";

        // 신호 결합
        cout << "\nsumChip = ";
		for (int i = 0; i < 8; i++)
        {
            sumChip[i] = buf[i] + buf1[i] + buf2[i] + buf3[i];
            cout << sumChip[i] << " ";
        }
        cout << "\n";
	}

    // 수신 시작 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // 수신기 r0
	if (pipe(fd_r0) < 0)
	{
		perror("pipe error : ");
		exit(0);
	}
	if ((r0_pid = fork()) < 0)
	{
		perror("fork error : ");
		exit(0);
	}
    // 수신기 r1
	if (pipe(fd_r1) < 0)
	{
		perror("pipe error : ");
		exit(0);
	}
	if ((r1_pid = fork()) < 0)
	{
		perror("fork error : ");
		exit(0);
	}
    // 수신기 r2
	if (pipe(fd_r2) < 0)
	{
		perror("pipe error : ");
		exit(0);
	}
	if ((r2_pid = fork()) < 0)
	{
		perror("fork error : ");
		exit(0);
	}
    // 수신기 r3
	if (pipe(fd_r3) < 0)
	{
		perror("pipe error : ");
		exit(0);
	}
	if ((r3_pid = fork()) < 0)
	{
		perror("fork error : ");
		exit(0);
	}

	if (r0_pid == 0)
	{
		sleep(10);
		for (int i = 0; i < 8; i++)
		{
			receive_t0 = receive_t0 + chip_t[0][i] * sumChip[i];
		}
		close(fd_r0[0]);
		write(fd_r0[1], &receive_t0, sizeof(int));
		exit(0);
	}
	else if (r1_pid == 0)
	{
		sleep(10);
		for (int i = 0; i < 8; i++)
		{
			receive_t1 = receive_t1 + chip_t[1][i] * sumChip[i];
		}
		close(fd_r1[0]);
		write(fd_r1[1], &receive_t1, sizeof(int));
		exit(0);
	}
	else if (r2_pid == 0)
	{
		sleep(10);
		for (int i = 0; i < 8; i++)
		{
			receive_t2 = receive_t2 + chip_t[2][i] * sumChip[i];
		}
		close(fd_r2[0]);
		write(fd_r2[1], &receive_t2, sizeof(int));
		exit(0);
	}
	else if (r3_pid == 0)
	{
		sleep(10);
		for (int i = 0; i < 8; i++)
		{
			receive_t3 = receive_t3 + chip_t[3][i] * sumChip[i];
		}
		close(fd_r3[0]);
		write(fd_r3[1], &receive_t3, sizeof(int));
		exit(0);
	}
	//결합된 신호를 수신기로 보낸다.
else{

	read(fd_r0[0], &result0, sizeof(int));
	read(fd_r1[0], &result1, sizeof(int));
	read(fd_r2[0], &result2, sizeof(int));
	read(fd_r3[0], &result3, sizeof(int));
	}
	cout << "------------------";
	if (result0 > 0) cout << "\nreceive_t0 = 1\n";
	else cout << "\nreceive_t0 = 0\n";

    if (result1 > 0) cout << "\nreceive_t1 = 1\n";
	else cout << "\nreceive_t1 = 0\n";

    if (result2 > 0) cout << "\nreceive_t2 = 1\n";
	else cout << "\nreceive_t2 = 0\n";

    if (result3 > 0) cout << "\nreceive_t3 = 1\n";
	else cout << "\nreceive_t3 = 0\n";


}
    
// -------------------------------------------------------------------------------------

void getData()
{
    for(int i=0; i<4; i++)
    {
        cout << "Enter data of t" << i << " = ";
        cin >> t[i];
        if(t[i] == 0) t[i] = -1;
    }
    for(int i=0; i<4; i++)
    {
        cout << "\nBipolar transform of t" << i << " = ";
        if(t[i] == 1) cout << "+";
        cout << t[i] << " "; 
    }
    cout << "\n\n";
    
}

void getChipSequence()
{
    for(int i=0; i<4; i++)
    {
        cout << "Enter Chip sequence of t" << i << " = ";
        for(int k=0; k<8; k++)
        {
            cin >> chip_t[i][k];
            if (chip_t[i][k] == 0) chip_t[i][k] = -1;
        }
    }
}