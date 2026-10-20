#include <stdio.h>

const char* rebus = "SEND + MORE = MONEY";

char letters[10];
int  count = 0;
int  digit[26];
int  used[10];
char text[200];
int  found = 0;
long long checked = 0;

int is_letter(char c) { return c >= 'A' && c <= 'Z'; }
int is_digit(char c) { return c >= '0' && c <= '9'; }

int check(void)
{
    int i;
    checked++;

    for (i = 0; rebus[i] != '\0'; i++) {
        if (is_letter(rebus[i]))
            text[i] = '0' + digit[rebus[i] - 'A'];
        else
            text[i] = rebus[i];
    }
    text[i] = '\0';

    long long left = 0;
    long long right = 0;
    long long number = 0;
    int in_number = 0;
    int after_equal = 0;

    for (i = 0; ; i++) {
        char c = text[i];

        if (is_digit(c)) {
            if (!in_number && c == '0' && is_digit(text[i + 1]))
                return 0;
            number = number * 10 + (c - '0');
            in_number = 1;
        }
        else if (in_number) {
            if (after_equal)
                right = number;
            else
                left = left + number;
            number = 0;
            in_number = 0;
        }

        if (c == '=') after_equal = 1;
        if (c == '\0') break;
    }

    return left == right;
}

void assign(int i)
{
    if (found) return;

    if (i == count) {
        if (check()) found = 1;
        return;
    }

    for (int d = 0; d <= 9; d++) {
        if (found) return;
        if (used[d]) continue;

        used[d] = 1;
        digit[letters[i] - 'A'] = d;
        assign(i + 1);
        used[d] = 0;
    }
}

int main(void)
{
    int seen[26] = { 0 };
    for (int i = 0; rebus[i] != '\0'; i++) {
        char c = rebus[i];
        if (is_letter(c) && seen[c - 'A'] == 0) {
            seen[c - 'A'] = 1;
            letters[count] = c;
            count++;
        }
    }

    assign(0);

    printf("%s\n", rebus);
    if (found)
        printf("%s\n", text);
    else
        printf("No solution\n");
    printf("Checked: %lld\n", checked);

    return 0;
}