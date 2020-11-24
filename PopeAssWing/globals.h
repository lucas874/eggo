/*
 * Domain name
 */
#define DOMAIN_NAME "localhost"



/*
 * These "magic numbers" are the integer values of the sum of the ascii-values of lowercase
 * letters of the request.
 *
 */
#define SMTP_KEY_HELO 424
#define SMTP_KEY_MAIL 419
#define SMTP_KEY_RCPT 441
#define SMTP_KEY_DATA 410
#define SMTP_KEY_RSET 446
#define SMTP_KEY_NOOP 444
#define SMTP_KEY_QUIT 451


/*
 * Reply codes as defined in RFC-821
 *
 */
#define SYSTEM_STATUS 211
#define HELP_MESSAGE 214
#define SERVICE_READY 220
#define SERVICE_CLOSING 221
#define MAIL_ACTION_OK 250
#define USER_NOT_LOCAL_FORWARDED 251
#define START_MAIL_INPUT 354 //end with <CRLF>.<CRLF>
#define SERVICE_NA_CLOSING 421
#define FAIL_MAILBOX_UNAVAILABLE 450
#define ERROR_IN_PROCESSING 451
#define FAIL_INSUFFICIENT_STORAGE 452
#define SYNTAX_ERROR_COMMAND_NOT_RECOGNIZED 500
#define SYNTAX_ERROR_IN_ARGUMENTS 501
#define COMMAND_NOT_IMPLEMENTED 502
#define BAD_COMMAND_SEQUENCE 503
#define PARAMETER_NOT_IMPLEMENTED 504
#define MAILBOX_NOT_FOUND 550
#define USER_NOT_LOCAL 551
#define MAIL_ACTION_ABORT_STORAGE 552
#define MAILBOX_SYNTAX_ERROR
#define TRANSACTION_FAILED 554


/*
 * Maximum lengths
 */
#define MAX_USERNAME 64
#define MAX_DOMAIN 64
#define MAX_CMD 512
#define MAX_REPLY 512
#define MAX_TEXT_LINE 1000
#define MAX_RCPTS 100

/*
 *
 * Server states
 *
 */

#define SMTP_STATE_INIT 1
#define SMTP_STATE_HELO 2
#define SMTP_STATE_DATA 3
#define SMTP_STATE_DATA_END 4

