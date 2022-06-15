#include "sws_config.h"

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

#include "sws_logger.h"

// Buffer for characters to make parsing easier.
typedef struct charbuff_t
{
    FILE* fp;
    int ch;
    int linenum;
} charbuff_t;

static void nextchar(charbuff_t* buff)
{
    if (buff->ch == '\n')
    {
        ++buff->linenum;
    }

    buff->ch = fgetc(buff->fp);

    // Skip carriage returns.
    if (buff->ch == '\r')
    {
        buff->ch = fgetc(buff->fp);
    }
}

static bool parse_comment_line(charbuff_t* buff)
{
    // Read until newline found.
    do
    {
        nextchar(buff);
    } while (buff->ch != EOF && buff->ch != '\n');


    // Read past newline.
    if (buff->ch == '\n')
    {
        nextchar(buff);
    }

    return true;
}

static bool read_four_digits(charbuff_t* buff, int* result)
{
    *result = 0;

    for (int i = 0; i < 4; ++i)
    {
        if (!isdigit(buff->ch))
        {
            return false;
        }

        *result *= 10;
        *result += buff->ch - '0';
        nextchar(buff);
    }

    return true;
}

static bool parse_data_line(charbuff_t* buff, SWS_schedule_t* result)
{
    result->day = -1;
    result->start_time = -1;
    result->end_time = -1;

    // First digit must be digit.
    if (!isdigit(buff->ch))
    {
        SWS_log("Error parsing line %d: Expected digit", buff->linenum);
        return false;
    }

    result->day = buff->ch - '0';
    nextchar(buff);

    // Followed by one space.
    if (buff->ch != ' ')
    {
        SWS_log("Error parsing line %d: Expected space", buff->linenum);
        return false;
    }
    nextchar(buff);

    // Followed by four digits.
    if (!read_four_digits(buff, &result->start_time))
    {
        SWS_log("Error parsing line %d: Expected four digits", buff->linenum);
        return false;
    }

    // Then one space
    if (buff->ch != ' ')
    {
        SWS_log("Error parsing line %d: Expected space", buff->linenum);
        return false;
    }
    nextchar(buff);

    // Then four more digits
    if (!read_four_digits(buff, &result->end_time))
    {
        SWS_log("Error parsing line %d: Expected four digits", buff->linenum);
        return false;
    }

    // Must end with newline.
    if (buff->ch != '\n')
    {
        SWS_log("Error parsing line %d: Expected new line", buff->linenum);
        return false;
    }

    // Consume end-line
    nextchar(buff);

    return true;
}

int SWS_parse_config(const char* filename, SWS_schedule_t* entries, int entries_size)
{
    FILE* fp = fopen(filename, "r");
    if (fp == NULL)
    {
        SWS_log("Failed to open config file");
        return -1;
    }

    charbuff_t buff;
    buff.fp = fp;
    buff.linenum = 1;

    int schedule_idx = 0;
    nextchar(&buff);

    while (buff.ch != EOF && schedule_idx < entries_size)
    {
        // Read to end of line on comment lines.
        if (buff.ch == ';')
        {
            parse_comment_line(&buff);
        }
        // Skip empty lines.
        else if (buff.ch == '\n')
        {
            nextchar(&buff);
        }
        // Parse line as data
        else
        {
            if (parse_data_line(&buff, entries + schedule_idx))
            {
                ++schedule_idx;
            }
            else
            {
                return -1;
            }
        }
    }

    // Too many case.
    if (schedule_idx >= entries_size)
    {
        return -1;
    }

    return schedule_idx;
}
