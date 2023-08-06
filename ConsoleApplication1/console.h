#pragma once
//출처 https://eskeptor.tistory.com/191
// Rectangle 구조체
struct stRect
{
	int nWidth;
	int nHeight;
};

// 콘솔 관련 설정 값을 가지고 있을 구조체
struct stConsole
{
	// Console Handler
	HANDLE hConsole;
	// Console Rect Data
	stRect rtConsole;
	// Console Buffer Handler
	HANDLE hBuffer[2];
	// Current Console Buffer Index
	int nCurBuffer;

	stConsole()
		: hConsole(nullptr), hBuffer{ nullptr, }, nCurBuffer(0)
	{}
};

// 전역 변수
stConsole console;

void InitGame(bool bInitConsole = true)
{
	// Initialize Console Data
	if (bInitConsole)
	{
		// 현재 콘솔의 핸들을 받아옵니다.
		console.hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		// 현재 콘솔 버퍼의 인덱스를 0으로 초기화
		console.nCurBuffer = 0;

		// 콘솔 관련 설정
		CONSOLE_CURSOR_INFO consoleCursor{ 1, FALSE };  // 콘솔의 커서 깜빡임을 제거합니다.
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo{ 0, };
		GetConsoleScreenBufferInfo(console.hConsole, &consoleInfo);
		consoleInfo.dwSize.X = 1000;    // 콘솔의 Width
		consoleInfo.dwSize.Y = 1000;    // 콘솔의 Height

		// 콘솔의 크기를 다시 계산 (나중에 그림 그릴때 사용)
		console.rtConsole.nWidth = consoleInfo.srWindow.Right - consoleInfo.srWindow.Left;
		console.rtConsole.nHeight = consoleInfo.srWindow.Bottom - consoleInfo.srWindow.Top;

		// 콘솔의 첫번째 화면 버퍼 생성
		console.hBuffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleScreenBufferSize(console.hBuffer[0], consoleInfo.dwSize);    // 화면 버퍼 크기 설정
		SetConsoleWindowInfo(console.hBuffer[0], TRUE, &consoleInfo.srWindow); // 콘솔 설정
		SetConsoleCursorInfo(console.hBuffer[0], &consoleCursor);              // 콘솔의 커서 설정

		// 콘솔의 두번째 화면 버퍼 생성
		console.hBuffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleScreenBufferSize(console.hBuffer[1], consoleInfo.dwSize);
		SetConsoleWindowInfo(console.hBuffer[1], TRUE, &consoleInfo.srWindow);
		SetConsoleCursorInfo(console.hBuffer[1], &consoleCursor);
	}
}

void DestroyGame()
{
	if (console.hBuffer[0] != nullptr)
	{
		CloseHandle(console.hBuffer[0]);
	}

	if (console.hBuffer[1] != nullptr)
	{
		CloseHandle(console.hBuffer[1]);
	}
}

// 화면 클리어
void ClearScreen()
{
	COORD pos{ 0, };
	DWORD dwWritten = 0;
	unsigned size = console.rtConsole.nWidth * console.rtConsole.nHeight;

	// 콘솔 화면 전체를 띄어쓰기를 넣어 빈 화면처럼 만듭니다.
	FillConsoleOutputCharacter(console.hConsole, ' ', size, pos, &dwWritten);
	SetConsoleCursorPosition(console.hConsole, pos);
}

// 버퍼 스왑
void BufferFlip()
{
	// 화면 버퍼 설정
	SetConsoleActiveScreenBuffer(console.hBuffer[console.nCurBuffer]);
	// 화면 버퍼 인덱스를 교체
	console.nCurBuffer = console.nCurBuffer ? 0 : 1;//0이면 false라 1로 1이면 true라 0으로 바꾸는 3항 연산자
}
