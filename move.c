#include <curses.h>

static void run(void)
{
	int ch, x, y;

	/* (0, 0)に文字列"Push [Q] to quit."を印字する */
	mvaddstr(0, 0, "Push [Q] to quit.");

	/* initscr()によってstdscr(多分standard screenの略？)が
	確保され，その行数(LINES)と列数(COLS)がセットされている */
	x = COLS / 2;
	y = LINES / 2;
	mvaddch(y, x, '@');
	while ((ch = getch()) != 'Q') {
		/* (x, y)に' 'を印字する */
		mvaddch(y, x, ' ');

		/* キーボードから入力を得る */
		switch (ch) {
		case KEY_LEFT:
			x -= 1;
			break;
		case KEY_RIGHT:
			x += 1;
			break;
		case KEY_UP:
			y -= 1;
			break;
		case KEY_DOWN:
			y += 1;
			break;
		default:
			break;
		}

		/* 画面をはみ出さないようにする */
		if (x <= 0)
			x = 0;
		else if (x > COLS - 1)
			x = COLS - 1;
		else if (y <= 0)
			y = 0;
		else if (y >= LINES - 1)
			y = LINES - 1;

		/* (x, y)に'@'を印字する */
		mvaddch(y, x, '@');
	}
}

int main(void)
{
	initscr();	/* cursesライブラリの初期化 */
	noecho();	/* echoを切る */
	cbreak();	/* bufferingを切る */
	keypad(stdscr, TRUE);	/* 矢印キーを有効にする */

	run();

	endwin();	/* キーボード等の制御を終了する */
	return 0;
}
