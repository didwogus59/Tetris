#pragma once

struct double_buffer
{
	HANDLE console_now = GetStdHandle(STD_OUTPUT_HANDLE);
	int console_width, console_height;
	HANDLE buffers[2] = { nullptr, };
	int buffer_now = 0;
};

double_buffer console;

void init_buffer()
{
		CONSOLE_CURSOR_INFO consoleCursor{ 1, FALSE };
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo{ 0, };
		GetConsoleScreenBufferInfo(console.console_now, &consoleInfo);

		console.console_width = consoleInfo.srWindow.Right - consoleInfo.srWindow.Left;
		console.console_height = consoleInfo.srWindow.Bottom - consoleInfo.srWindow.Top;

		console.buffers[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleScreenBufferSize(console.buffers[0], consoleInfo.dwSize);
		SetConsoleWindowInfo(console.buffers[0], TRUE, &consoleInfo.srWindow);
		SetConsoleCursorInfo(console.buffers[0], &consoleCursor);         

		console.buffers[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleScreenBufferSize(console.buffers[1], consoleInfo.dwSize);
		SetConsoleWindowInfo(console.buffers[1], TRUE, &consoleInfo.srWindow);
		SetConsoleCursorInfo(console.buffers[1], &consoleCursor);
}

void destroy_buffer()
{
	CloseHandle(console.buffers[0]);
	CloseHandle(console.buffers[1]);
}

void ClearScreen()
{
	COORD pos{ 0, };
	DWORD dw = 0;
	unsigned size = console.console_width * console.console_height;

	FillConsoleOutputCharacter(console.console_now, ' ', size, pos, &dw);
	SetConsoleCursorPosition(console.console_now, pos);
}

void BufferFlip()
{
	SetConsoleActiveScreenBuffer(console.buffers[console.buffer_now]);
	console.buffer_now = console.buffer_now ? 0 : 1;
}
